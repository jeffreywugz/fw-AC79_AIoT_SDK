/* coap-client -- simple CoAP client
 *
 * Copyright (C) 2010--2013 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#include "libcoap/coap_config.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "printf.h"

#include "libcoap/coap_net.h"
//#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "libcoap/coap.h"

#include "system/includes.h"
#include "wifi/wifi_connect.h"

#include "app_config.h"

#ifdef USE_COAP_CLIENT_TEST

#define perror printf

#define NI_MAXSERV 64

static int flags = 0;

static unsigned char _token_data[8];
static str the_token = { 0, _token_data };

#define FLAGS_BLOCK 0x01

static coap_list_t *optlist = NULL;
/* Request URI.
 * TODO: associate the resources with transaction id and make it expireable */
static coap_uri_t uri;
static str proxy = { 0, NULL };
static unsigned short proxy_port = COAP_DEFAULT_PORT;

/* reading is done when this flag is set */
static int ready = 0;

static str payload = { 0, NULL }; /* optional payload to send */

static unsigned char msgtype = COAP_MESSAGE_CON; /* usually, requests are sent confirmable */

typedef unsigned char method_t;
static method_t method = 1;		/* the method we are using in our requests */

static coap_block_t block = { .num = 0, .m = 0, .szx = 6 };

static unsigned int wait_seconds = 90;	/* default timeout in seconds */
static coap_tick_t max_wait;		/* global timeout (changed by set_timeout()) */

static unsigned int obs_seconds = 30;	/* default observe time */
static coap_tick_t obs_wait = 0;	/* timeout for current subscription */

#define min(a,b) ((a) < (b) ? (a) : (b))

static inline void
set_timeout(coap_tick_t *timer, const unsigned int seconds)
{
    coap_ticks(timer);
    *timer += seconds * COAP_TICKS_PER_SECOND;
}

static int
append_to_output(const unsigned char *data, size_t len)
{
    char *ptr;
    ptr = data;
    ptr[len] = '\0';
    printf("|append_to_output->%d, \n%s\n", (int)len, data);

    return 0;
}

static coap_pdu_t *
new_ack(coap_context_t  *ctx, coap_queue_t *node)
{
    coap_pdu_t *pdu = coap_new_pdu();

    if (pdu) {
        pdu->hdr->type = COAP_MESSAGE_ACK;
        pdu->hdr->code = 0;
        pdu->hdr->id = node->pdu->hdr->id;
    }

    return pdu;
}

static coap_pdu_t *
new_response(coap_context_t  *ctx, coap_queue_t *node, unsigned int code)
{
    coap_pdu_t *pdu = new_ack(ctx, node);

    if (pdu) {
        pdu->hdr->code = code;
    }

    return pdu;
}

static coap_pdu_t *
coap_new_request(coap_context_t *ctx, method_t m, coap_list_t *options)
{
    coap_pdu_t *pdu;
    coap_list_t *opt;

    if (!(pdu = coap_new_pdu())) {
        return NULL;
    }

    pdu->hdr->type = msgtype;
    pdu->hdr->id = coap_new_message_id(ctx);
    pdu->hdr->code = m;

    pdu->hdr->token_length = the_token.length;
    if (!coap_add_token(pdu, the_token.length, the_token.s)) {
        debug("cannot add token to request\n");
    }

    for (opt = options; opt; opt = opt->next) {
        coap_add_option(pdu, COAP_OPTION_KEY(*(coap_option *)opt->data),
                        COAP_OPTION_LENGTH(*(coap_option *)opt->data),
                        COAP_OPTION_DATA(*(coap_option *)opt->data));
    }

    if (payload.length) {
        if ((flags & FLAGS_BLOCK) == 0) {
            coap_add_data(pdu, payload.length, payload.s);
        } else {
            coap_add_block(pdu, payload.length, payload.s, block.num, block.szx);
        }
    }

    coap_show_pdu(pdu);

    return pdu;
}

static coap_tid_t
clear_obs(coap_context_t *ctx, const coap_address_t *remote)
{
    coap_pdu_t *pdu;
    coap_tid_t tid = COAP_INVALID_TID;

    /* create bare PDU w/o any option  */
    coap_log(LOG_INFO, "response code 7.31 is %d\n", COAP_RESPONSE_CODE(731));
    pdu = coap_pdu_init(msgtype, COAP_RESPONSE_CODE(731),
                        coap_new_message_id(ctx),
                        sizeof(coap_hdr_t) + the_token.length);

    if (!pdu) {
        return tid;
    }

    if (!coap_add_token(pdu, the_token.length, the_token.s)) {
        coap_log(LOG_CRIT, "cannot add token");
        coap_delete_pdu(pdu);
        return tid;
    }
    coap_show_pdu(pdu);

    if (pdu->hdr->type == COAP_MESSAGE_CON) {
        tid = coap_send_confirmed(ctx, remote, pdu);
    } else {
        tid = coap_send(ctx, remote, pdu);
    }

    if (tid == COAP_INVALID_TID) {
        debug("clear_obs: error sending new request");
        coap_delete_pdu(pdu);
    } else if (pdu->hdr->type != COAP_MESSAGE_CON) {
        coap_delete_pdu(pdu);
    }

    return tid;
}

static int resolve_address(const char *server, struct sockaddr *dst)
{

    struct addrinfo *res, *ainfo;
    struct addrinfo hints;
    int error, len = -1;

    memset((char *)&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_UNSPEC;

    error = getaddrinfo(server, NULL, &hints, &res);

    if (error != 0) {
        printf("getaddrinfo0: %s\n", strerror(error));
        return error;
    }

    for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
        switch (ainfo->ai_family) {
        case AF_INET6:
        case AF_INET:
            len = ainfo->ai_addrlen;
            memcpy(dst, ainfo->ai_addr, len);
            goto finish;
        default:
            ;
        }
    }

finish:
    freeaddrinfo(res);
    return len;
}

static inline coap_opt_t *
get_block(coap_pdu_t *pdu, coap_opt_iterator_t *opt_iter)
{
    coap_opt_filter_t f;

    assert(pdu);

    memset(f, 0, sizeof(coap_opt_filter_t));
    coap_option_setb(f, COAP_OPTION_BLOCK1);
    coap_option_setb(f, COAP_OPTION_BLOCK2);

    coap_option_iterator_init(pdu, opt_iter, f);
    return coap_option_next(opt_iter);
}

#define HANDLE_BLOCK1(Pdu)						\
  ((method == COAP_REQUEST_PUT || method == COAP_REQUEST_POST) &&	\
   ((flags & FLAGS_BLOCK) == 0) &&					\
   ((Pdu)->hdr->code == COAP_RESPONSE_CODE(201) ||			\
    (Pdu)->hdr->code == COAP_RESPONSE_CODE(204)))

static inline int
check_token(coap_pdu_t *received)
{
    return received->hdr->token_length == the_token.length &&
           memcmp(received->hdr->token, the_token.s, the_token.length) == 0;
}

//接收回调
static void
message_handler(struct coap_context_t  *ctx,
                const coap_address_t *remote,
                coap_pdu_t *sent,
                coap_pdu_t *received,
                const coap_tid_t id)
{

    coap_pdu_t *pdu = NULL;
    coap_opt_t *block_opt;
    coap_opt_iterator_t opt_iter;
    unsigned char buf[4];
    coap_list_t *option;
    size_t len;
    unsigned char *databuf;
    coap_tid_t tid;

#ifndef NDEBUG
    if (LOG_DEBUG <= coap_get_log_level()) {
        debug("** process incoming %d.%02d response:\n",
              (received->hdr->code >> 5), received->hdr->code & 0x1F);
        coap_show_pdu(received);
    }
#endif

    /* check if this is a response to our original request */
    if (!check_token(received)) {
        /* drop if this was just some message, or send RST in case of notification */
        if (!sent && (received->hdr->type == COAP_MESSAGE_CON ||
                      received->hdr->type == COAP_MESSAGE_NON)) {
            //发送RST报文
            coap_send_rst(ctx, remote, received);
        }
        return;
    }

    if (received->hdr->type == COAP_MESSAGE_RST) {
        /* acknowledge received response if confirmable (TODO: check Token) */
        info("got RST\n");
        return;
    }

    /* output the received data, if any */
    if (received->hdr->code == COAP_RESPONSE_CODE(205)) {

        /* set obs timer if we have successfully subscribed a resource */
        if (sent && coap_check_option(received, COAP_OPTION_SUBSCRIPTION, &opt_iter)) {
            debug("observation relationship established, set timeout to %d\n", obs_seconds);
            set_timeout(&obs_wait, obs_seconds);
        }

        /* Got some data, check if block option is set. Behavior is undefined if
         * both, Block1 and Block2 are present. */
        block_opt = get_block(received, &opt_iter);
        if (!block_opt) {
            /* There is no block option set, just read the data and we are done. */
            if (coap_get_data(received, &len, &databuf)) {

                //payload
                append_to_output(databuf, len);
            }
        } else {
            unsigned short blktype = opt_iter.type;

            /* TODO: check if we are looking at the correct block number */
            if (coap_get_data(received, &len, &databuf)) {
                append_to_output(databuf, len);
            }

            if (COAP_OPT_BLOCK_MORE(block_opt)) {
                /* more bit is set */
                debug("found the M bit, block size is %u, block nr. %u\n",
                      COAP_OPT_BLOCK_SZX(block_opt), coap_opt_block_num(block_opt));

                /* create pdu with request for next block */
                pdu = coap_new_request(ctx, method, NULL); /* first, create bare PDU w/o any option  */
                if (pdu) {
                    /* add URI components from optlist */
                    for (option = optlist; option; option = option->next) {
                        switch (COAP_OPTION_KEY(*(coap_option *)option->data)) {
                        case COAP_OPTION_URI_HOST :
                        case COAP_OPTION_URI_PORT :
                        case COAP_OPTION_URI_PATH :
                        case COAP_OPTION_URI_QUERY :
                            coap_add_option(pdu, COAP_OPTION_KEY(*(coap_option *)option->data),
                                            COAP_OPTION_LENGTH(*(coap_option *)option->data),
                                            COAP_OPTION_DATA(*(coap_option *)option->data));
                            break;
                        default:
                            ;			/* skip other options */
                        }
                    }

                    /* finally add updated block option from response, clear M bit */
                    /* blocknr = (blocknr & 0xfffffff7) + 0x10; */
                    debug("query block %d\n", (coap_opt_block_num(block_opt) + 1));
                    coap_add_option(pdu, blktype, coap_encode_var_bytes(buf,
                                    ((coap_opt_block_num(block_opt) + 1) << 4) |
                                    COAP_OPT_BLOCK_SZX(block_opt)), buf);

                    if (received->hdr->type == COAP_MESSAGE_CON) {
                        tid = coap_send_confirmed(ctx, remote, pdu);
                    } else {
                        tid = coap_send(ctx, remote, pdu);
                    }

                    if (tid == COAP_INVALID_TID) {
                        debug("message_handler: error sending new request");
                        coap_delete_pdu(pdu);
                    } else {
                        set_timeout(&max_wait, wait_seconds);
                        if (received->hdr->type != COAP_MESSAGE_CON) {
                            coap_delete_pdu(pdu);
                        }
                    }

                    return;
                }
            }
        }
    } else {			/* no 2.05 */

        /* check if an error was signaled and output payload if so */
        if (COAP_RESPONSE_CLASS(received->hdr->code) >= 4) {
            printf("%d.%02d",
                   (received->hdr->code >> 5), received->hdr->code & 0x1F);
            if (coap_get_data(received, &len, &databuf)) {
                printf(" ");
                while (len--) {
                    printf("%c", *databuf++);
                }
            }
            printf("\n");
        }

    }

    /* finally send new request, if needed */
    if (pdu && coap_send(ctx, remote, pdu) == COAP_INVALID_TID) {
        debug("message_handler: error sending response");
    }
    coap_delete_pdu(pdu);

    /* our job is done, we can exit at any time */
    ready = coap_check_option(received, COAP_OPTION_SUBSCRIPTION, &opt_iter) == NULL;
}

static void
usage(const char *program, const char *version)
{
    const char *p;

    p = strrchr(program, '/');
    if (p) {
        program = ++p;
    }

    printf("%s v%s -- a small CoAP implementation\n"
           "(c) 2010-2013 Olaf Bergmann <bergmann@tzi.org>\n\n"
           "usage: %s [-A type...] [-t type] [-b [num,]size] [-B seconds] [-e text]\n"
           "\t\t[-g group] [-m method] [-N] [-o file] [-P addr[:port]] [-p port]\n"
           "\t\t[-s duration] [-O num,text] [-T string] [-v num] URI\n\n"
           "\tURI can be an absolute or relative coap URI,\n"
           "\t-A type...\taccepted media types as comma-separated list of\n"
           "\t\t\tsymbolic or numeric values\n"
           "\t-t type\t\tcontent type for given resource for PUT/POST\n"
           "\t-b [num,]size\tblock size to be used in GET/PUT/POST requests\n"
           "\t       \t\t(value must be a multiple of 16 not larger than 1024)\n"
           "\t       \t\tIf num is present, the request chain will start at\n"
           "\t       \t\tblock num\n"
           "\t-B seconds\tbreak operation after waiting given seconds\n"
           "\t\t\t(default is %d)\n"
           "\t-e text\t\tinclude text as payload (use percent-encoding for\n"
           "\t\t\tnon-ASCII characters)\n"
           "\t-f file\t\tfile to send with PUT/POST (use '-' for STDIN)\n"
           "\t-g group\tjoin the given multicast group\n"
           "\t-m method\trequest method (get|put|post|delete), default is 'get'\n"
           "\t-N\t\tsend NON-confirmable message\n"
           "\t-o file\t\toutput received data to this file (use '-' for STDOUT)\n"
           "\t-p port\t\tlisten on specified port\n"
           "\t-s duration\tsubscribe for given duration [s]\n"
           "\t-v num\t\tverbosity level (default: 3)\n"
           "\t-O num,text\tadd option num with contents text to request\n"
           "\t-P addr[:port]\tuse proxy (automatically adds Proxy-Uri option to\n"
           "\t\t\trequest)\n"
           "\t-T token\tinclude specified token\n"
           "\n"
           "examples:\n"
           "\tcoap-client -m get coap://[::1]/\n"
           "\tcoap-client -m get coap://[::1]/.well-known/core\n"
           "\tcoap-client -m get -T cafe coap://[::1]/time\n"
           "\techo 1000 | coap-client -m put -T cafe coap://[::1]/time -f -\n"
           , program, version, program, wait_seconds);
}
#if LWIP_IPV6
static int join(coap_context_t *ctx, char *group_name)
{
    struct ipv6_mreq mreq;
    struct addrinfo   *reslocal = NULL, *resmulti = NULL, hints, *ainfo;
    int result = -1;

    /* we have to resolve the link-local interface to get the interface id */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    result = getaddrinfo("::", NULL, &hints, &reslocal);
    if (result < 0) {
        printf("join: cannot resolve link-local interface: %s\n",
               strerror(result));
        goto finish;
    }

    /* get the first suitable interface identifier */
    for (ainfo = reslocal; ainfo != NULL; ainfo = ainfo->ai_next) {
        if (ainfo->ai_family == AF_INET6) {
            mreq.ipv6mr_interface =
                ((struct sockaddr_in6 *)ainfo->ai_addr)->sin6_scope_id;
            break;
        }
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    /* resolve the multicast group address */
    result = getaddrinfo(group_name, NULL, &hints, &resmulti);

    if (result < 0) {
        printf("join: cannot resolve multicast address: %s\n",
               strerror(result));
        goto finish;
    }

    for (ainfo = resmulti; ainfo != NULL; ainfo = ainfo->ai_next) {
        if (ainfo->ai_family == AF_INET6) {
            mreq.ipv6mr_multiaddr =
                ((struct sockaddr_in6 *)ainfo->ai_addr)->sin6_addr;
            break;
        }
    }

    result = setsockopt(ctx->sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP,
                        (char *)&mreq, sizeof(mreq));
    if (result < 0) {
        perror("join: setsockopt");
    }

finish:
    freeaddrinfo(resmulti);
    freeaddrinfo(reslocal);

    return result;
}
#endif

static int
order_opts(void *a, void *b)
{
    if (!a || !b) {
        return a < b ? -1 : 1;
    }

    if (COAP_OPTION_KEY(*(coap_option *)a) < COAP_OPTION_KEY(*(coap_option *)b)) {
        return -1;
    }

    return COAP_OPTION_KEY(*(coap_option *)a) == COAP_OPTION_KEY(*(coap_option *)b);
}


static coap_list_t *
new_option_node(unsigned short key, unsigned int length, unsigned char *data)
{
    coap_option *option;
    coap_list_t *node;

    option = coap_malloc(sizeof(coap_option) + length);
    if (!option) {
        goto error;
    }

    COAP_OPTION_KEY(*option) = key;
    COAP_OPTION_LENGTH(*option) = length;
    memcpy(COAP_OPTION_DATA(*option), data, length);

    /* we can pass NULL here as delete function since option is released automatically  */
    node = coap_new_listnode(option, NULL);

    if (node) {
        return node;
    }

error:
    perror("new_option_node: malloc");
    coap_free(option);
    return NULL;
}

typedef struct {
    unsigned char code;
    char *media_type;
} content_type_t;

static void
cmdline_content_type(char *arg, unsigned short key)
{
    static content_type_t content_types[] = {
        {  0, "plain" },
        {  0, "text/plain" },
        { 40, "link" },
        { 40, "link-format" },
        { 40, "application/link-format" },
        { 41, "xml" },
        { 42, "binary" },
        { 42, "octet-stream" },
        { 42, "application/octet-stream" },
        { 47, "exi" },
        { 47, "application/exi" },
        { 50, "json" },
        { 50, "application/json" },
        { 255, NULL }
    };
    coap_list_t *node;
    unsigned char i, value[10];
    int valcnt = 0;
    unsigned char buf[2];
    char *p, *q = arg;

    while (q && *q) {
        p = strchr(q, ',');

        if (isdigit(*q)) {
            if (p) {
                *p = '\0';
            }
            value[valcnt++] = atoi(q);
        } else {
            for (i = 0; content_types[i].media_type &&
                 strncmp(q, content_types[i].media_type, p ? p - q : strlen(q)) != 0 ;
                 ++i)
                ;

            if (content_types[i].media_type) {
                value[valcnt] = content_types[i].code;
                valcnt++;
            } else {
                warn("W: unknown content-type '%s'\n", arg);
            }
        }

        if (!p || key == COAP_OPTION_CONTENT_TYPE) {
            break;
        }

        q = p + 1;
    }

    for (i = 0; i < valcnt; ++i) {
        node = new_option_node(key, coap_encode_var_bytes(buf, value[i]), buf);
        if (node) {
            coap_insert(&optlist, node, order_opts);
        }
    }
}

static void
cmdline_uri(char *arg)
{
    unsigned char portbuf[2];
#define BUFSIZE 40
    unsigned char _buf[BUFSIZE];
    unsigned char *buf = _buf;
    size_t buflen;
    int res;

    if (proxy.length) {		/* create Proxy-Uri from argument */
        size_t len = strlen(arg);
        while (len > 270) {
            coap_insert(&optlist,
                        new_option_node(COAP_OPTION_PROXY_URI,
                                        270, (unsigned char *)arg),
                        order_opts);
            len -= 270;
            arg += 270;
        }

        coap_insert(&optlist,
                    new_option_node(COAP_OPTION_PROXY_URI,
                                    len, (unsigned char *)arg),
                    order_opts);
    } else {			/* split arg into Uri-* options */
        coap_split_uri((unsigned char *)arg, strlen(arg), &uri);

        if (uri.port != COAP_DEFAULT_PORT) {
            coap_insert(&optlist,
                        new_option_node(COAP_OPTION_URI_PORT,
                                        coap_encode_var_bytes(portbuf, uri.port),
                                        portbuf),
                        order_opts);
        }

        if (uri.path.length) {
            buflen = BUFSIZE;
            res = coap_split_path(uri.path.s, uri.path.length, buf, &buflen);

            while (res--) {
                coap_insert(&optlist, new_option_node(COAP_OPTION_URI_PATH,
                                                      COAP_OPT_LENGTH(buf),
                                                      COAP_OPT_VALUE(buf)),
                            order_opts);

                buf += COAP_OPT_SIZE(buf);
            }
        }

        if (uri.query.length) {
            buflen = BUFSIZE;
            buf = _buf;
            res = coap_split_query(uri.query.s, uri.query.length, buf, &buflen);

            while (res--) {
                coap_insert(&optlist, new_option_node(COAP_OPTION_URI_QUERY,
                                                      COAP_OPT_LENGTH(buf),
                                                      COAP_OPT_VALUE(buf)),
                            order_opts);

                buf += COAP_OPT_SIZE(buf);
            }
        }
    }
}

static int
cmdline_blocksize(char *arg)
{
    unsigned short size;

again:
    size = 0;
    while (*arg && *arg != ',') {
        size = size * 10 + (*arg++ - '0');
    }

    if (*arg == ',') {
        arg++;
        block.num = size;
        goto again;
    }

    if (size) {
        block.szx = (coap_fls(size >> 4) - 1) & 0x07;
    }

    flags |= FLAGS_BLOCK;
    return 1;
}

/* Called after processing the options from the commandline to set
 * Block1 or Block2 depending on method. */
static void
set_blocksize()
{
    static unsigned char buf[4];	/* hack: temporarily take encoded bytes */
    unsigned short opt;

    if (method != COAP_REQUEST_DELETE) {
        opt = method == COAP_REQUEST_GET ? COAP_OPTION_BLOCK2 : COAP_OPTION_BLOCK1;

        coap_insert(&optlist, new_option_node(opt,
                                              coap_encode_var_bytes(buf, (block.num << 4 | block.szx)), buf),
                    order_opts);
    }
}

static int
cmdline_proxy(char *arg)
{
    char *proxy_port_str = strrchr((const char *)arg, ':'); /* explicit port ? */
    if (proxy_port_str) {
        char *ipv6_delimiter = strrchr((const char *)arg, ']');
        if (!ipv6_delimiter) {
            if (proxy_port_str == strchr((const char *)arg, ':')) {
                /* host:port format - host not in ipv6 hexadecimal string format */
                *proxy_port_str++ = '\0'; /* split */
                proxy_port = atoi(proxy_port_str);
            }
        } else {
            arg = strchr((const char *)arg, '[');
            if (!arg) {
                return 0;
            }
            arg++;
            *ipv6_delimiter = '\0'; /* split */
            if (ipv6_delimiter + 1 == proxy_port_str++) {
                /* [ipv6 address]:port */
                proxy_port = atoi(proxy_port_str);
            }
        }
    }

    proxy.length = strlen(arg);
    if ((proxy.s = coap_malloc(proxy.length + 1)) == NULL) {
        proxy.length = 0;
        return 0;
    }

    memcpy(proxy.s, arg, proxy.length + 1);
    return 1;
}

static inline void
cmdline_token(char *arg)
{
    strncpy((char *)the_token.s, arg, min(sizeof(_token_data), strlen(arg)));
    the_token.length = strlen(arg);
}

static void
cmdline_option(char *arg)
{
    unsigned int num = 0;

    while (*arg && *arg != ',') {
        num = num * 10 + (*arg - '0');
        ++arg;
    }
    if (*arg == ',') {
        ++arg;
    }

    coap_insert(&optlist, new_option_node(num,
                                          strlen(arg),
                                          (unsigned char *)arg), order_opts);
}

extern int  check_segment(const unsigned char *s, size_t length);
extern void decode_segment(const unsigned char *seg, size_t length, unsigned char *buf);

static int
cmdline_input(char *text, str *buf)
{
    int len;
    len = check_segment((unsigned char *)text, strlen(text));

    if (len < 0) {
        return 0;
    }

    buf->s = (unsigned char *)coap_malloc(len);
    if (!buf->s) {
        return 0;
    }

    buf->length = len;
    decode_segment((unsigned char *)text, strlen(text), buf->s);
    return 1;
}

static method_t
cmdline_method(char *arg)
{
    static char *methods[] =
    { 0, "get", "post", "put", "delete", 0};
    unsigned char i;

    for (i = 1; methods[i] && strcasecmp(arg, methods[i]) != 0 ; ++i)
        ;

    return i;	     /* note that we do not prevent illegal methods */
}

static coap_context_t *get_context(const char *node, const char *port)
{
    coap_context_t *ctx = NULL;
    int s;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Coap uses UDP */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST | AI_NUMERICSERV | AI_ALL;

    s = getaddrinfo(node, port, &hints, &result);
    if (s != 0) {
        printf("getaddrinfo2: %s\n", strerror(s));
        return NULL;
    }

    /* iterate through results until success */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        coap_address_t addr;

        if (rp->ai_addrlen <= sizeof(addr.addr)) {
            coap_address_init(&addr);
            addr.size = rp->ai_addrlen;
            memcpy(&addr.addr, rp->ai_addr, rp->ai_addrlen);

            ctx = coap_new_context(&addr);
            if (ctx) {
                /* TODO: output address:port for successful binding */
                goto finish;
            }
        }
    }

    printf("no context available for interface '%s'\n", node);

finish:
    freeaddrinfo(result);
    return ctx;
}

int coap_client_test(char *server_ip)
{
    coap_context_t  *ctx = NULL;
    coap_address_t dst;
    char addr[46];
    void *addrptr = NULL;
    fd_set readfds;
    struct timeval tv;
    int result;
    coap_tick_t now;
    coap_queue_t *nextpdu;
    coap_pdu_t  *pdu;
    str server;
    unsigned short port = COAP_DEFAULT_PORT;
    int res;
    char *group = NULL;

    coap_log_t log_level = LOG_DEBUG;
    coap_tid_t tid = COAP_INVALID_TID;

    char server_url[NI_MAXSERV];

    sprintf(server_url, "coap://%s", server_ip);

    //coap_set_log_level(log_level);

    //解析uri
    cmdline_uri(server_url);

    if (proxy.length) {
        server = proxy;
        port = proxy_port;
    } else {
        server = uri.host;
        server.s[server.length] = '\0';
        port = uri.port;
    }

    /* resolve destination address where server should be sent */
    res = resolve_address((const char *)(server.s), &dst.addr.sa);
    if (res < 0) {
        printf("failed to resolve address\n");
        return (-1);
    }

    dst.size = res;
    dst.addr.sin.sin_port = htons(port);

    /* add Uri-Host if server address differs from uri.host */

    switch (dst.addr.sa.sa_family) {
    case AF_INET:
        addrptr = &dst.addr.sin.sin_addr;

        /* create context for IPv4 */
        ctx = get_context(NULL, COAP_DEFAULT_PORT_STR);
        break;
#if LWIP_IPV6
    case AF_INET6:
        addrptr = &dst.addr.sin6.sin6_addr;

        /* create context for IPv6 */
        ctx = get_context("::", COAP_DEFAULT_PORT_STR);
        break;
#endif
    default:
        coap_log(LOG_EMERG, "sa_family error\n");
    }

    if (!ctx) {
        coap_log(LOG_EMERG, "cannot create context\n");
        return -1;
    }

    //块传输
    coap_register_option(ctx, COAP_OPTION_BLOCK2);

    //注册响应回调函数
    coap_register_response_handler(ctx, message_handler);

#if LWIP_IPV6
    /* join multicast group if requested at command line */
    if (group) {
        join(ctx, group);
    }
#endif

    /* construct CoAP message */
    if (!proxy.length && addrptr
        && (inet_ntop(dst.addr.sa.sa_family, addrptr, addr, sizeof(addr)) != 0)
        && (strlen(addr) != uri.host.length
            || memcmp(addr, uri.host.s, uri.host.length) != 0)) {

        /* add Uri-Host */
        coap_insert(&optlist, new_option_node(COAP_OPTION_URI_HOST,
                                              uri.host.length, uri.host.s),
                    order_opts);
    }

    /* set block option if requested at commandline */
    if (flags & FLAGS_BLOCK) {
        set_blocksize();
    }

    //创建coap报文
    if (!(pdu = coap_new_request(ctx, method, optlist))) {
        return -1;
    }

#ifndef NDEBUG
    if (LOG_DEBUG <= coap_get_log_level()) {
        debug("sending CoAP request:\n");
        coap_show_pdu(pdu);
    }
#endif

    if (pdu->hdr->type == COAP_MESSAGE_CON) {
        //发送confirmable报文
        tid = coap_send_confirmed(ctx, &dst, pdu);
    } else {
        //发送Non-confirmable报文
        tid = coap_send(ctx, &dst, pdu);
    }

    //如果报文为Non-confirmable或报文无效就删除报文
    if (pdu->hdr->type != COAP_MESSAGE_CON || tid == COAP_INVALID_TID) {
        coap_delete_pdu(pdu);
    }

    //设置超时时间
    set_timeout(&max_wait, wait_seconds);
    debug("timeout is set to %d seconds\n", wait_seconds);

    while (!(ready && coap_can_exit(ctx))) {
        FD_ZERO(&readfds);
        FD_SET(ctx->sockfd, &readfds);

        nextpdu = coap_peek_next(ctx);

        coap_ticks(&now);
        while (nextpdu && nextpdu->t <= now - ctx->sendqueue_basetime) {
            coap_retransmit(ctx, coap_pop_next(ctx));
            nextpdu = coap_peek_next(ctx);
        }

        if (nextpdu && nextpdu->t < min(obs_wait ? obs_wait : max_wait, max_wait) - now) {
            /* set timeout if there is a pdu to send */
            tv.tv_usec = ((nextpdu->t) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
            tv.tv_sec = (nextpdu->t) / COAP_TICKS_PER_SECOND;
        } else {
            /* check if obs_wait fires before max_wait */
            if (obs_wait && obs_wait < max_wait) {
                tv.tv_usec = ((obs_wait - now) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
                tv.tv_sec = (obs_wait - now) / COAP_TICKS_PER_SECOND;
            } else {
                tv.tv_usec = ((max_wait - now) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
                tv.tv_sec = (max_wait - now) / COAP_TICKS_PER_SECOND;
            }
        }

        result = select(ctx->sockfd + 1, &readfds, 0, 0, &tv);
        if (result < 0) {		/* error */
            perror("select");
        } else if (result > 0) {	/* read from socket */
            if (FD_ISSET(ctx->sockfd, &readfds)) {
                coap_read(ctx);	/* read received data */
                coap_dispatch(ctx);	/* and dispatch PDUs from receivequeue */
            }
        } else { /* timeout */
            coap_ticks(&now);
            if (max_wait <= now) {
                info("timeout\n");
                break;
            }
            if (obs_wait && obs_wait <= now) {
                debug("clear observation relationship\n");
                clear_obs(ctx, &dst); /* FIXME: handle error case COAP_TID_INVALID */

                /* make sure that the obs timer does not fire again */
                obs_wait = 0;
                obs_seconds = 0;
            }
        }
    }

    coap_free_context(ctx);

    if (optlist) {
        //删除options链表
        coap_delete_list(optlist);
        optlist = NULL;
    }

    return 0;
}

//coap服务器url
#define COAP_TEST_URL "117.60.157.137"

static void coap_client_start(void *priv)
{
    enum wifi_sta_connect_state state;
    while (1) {
        printf("Connecting to the network...\n");
        state = wifi_get_sta_connect_state();
        if (WIFI_STA_NETWORK_STACK_DHCP_SUCC == state) {
            printf("Network connection is successful!\n");
            break;
        }

        os_time_dly(20);
    }

    while (1) {
        coap_client_test(COAP_TEST_URL);
        os_time_dly(500);
    }
}

//应用程序入口,需要运行在STA模式下
void c_main(void)
{
    if (thread_fork("coap_client_start", 10, 2 * 1024, 0, NULL, coap_client_start, NULL) != OS_NO_ERR) {
        printf("thread fork fail\n");
    }
}

late_initcall(c_main);

#endif//USE_COAP_CLIENT_TEST
