/*
 *  TCP/IP or UDP/IP networking functions
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/mbedtls_config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_NET_C)
#if !defined(unix) && !defined(__unix__) && !defined(__unix) && \
    !defined(__APPLE__) && !defined(_WIN32)
#warning "This module only works on Unix and Windows, see MBEDTLS_NET_C in config.h"
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_time_t    time_t
#endif
#include "mbedtls/net.h"
#include "lwip.h"
#include <string.h>

#if (defined(_WIN32) || defined(_WIN32_WCE)) && !defined(EFIX64) && \
    !defined(EFI32)

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
/* Enables getaddrinfo() & Co */
#define _WIN32_WINNT 0x0501
#include <ws2tcpip.h>

#include <winsock2.h>
#include <windows.h>

#if defined(_MSC_VER)
#if defined(_WIN32_WCE)
#pragma comment( lib, "ws2.lib" )
#else
#pragma comment( lib, "ws2_32.lib" )
#endif
#endif /* _MSC_VER */

#define read(fd,buf,len)        recv(fd,(char*)buf,(int) len,0)
#define write(fd,buf,len)       send(fd,(char*)buf,(int) len,0)
#define close(fd)               closesocket(fd)

static int wsa_init_done = 0;

#else /* ( _WIN32 || _WIN32_WCE ) && !EFIX64 && !EFI32 */

#include "sys/types.h"
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include "sys/time.h"
//#include <unistd.h>
//#include <signal.h>
//#include <fcntl.h>
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include <errno.h>
#include "sock_api.h"
#endif /* ( _WIN32 || _WIN32_WCE ) && !EFIX64 && !EFI32 */

/* Some MS functions want int and MSVC warns if we pass size_t,
 * but the standard fucntions use socklen_t, so cast only for MSVC */
#if defined(_MSC_VER)
#define MSVC_INT_CAST   (int)
#else
#define MSVC_INT_CAST
#endif

//#include <stdio.h>

#include "time.h"

//#include <stdint.h>

/*
 * Prepare for using the sockets interface
 */
static int net_prepare(void)
{
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
    WSADATA wsaData;

    if (wsa_init_done == 0) {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
            return (MBEDTLS_ERR_NET_SOCKET_FAILED);
        }

        wsa_init_done = 1;
    }

#else
#if !defined(EFIX64) && !defined(EFI32)
    //signal( SIGPIPE, SIG_IGN );
#endif
#endif
    return (0);
}

/*
 * Initialize a context
 */
void mbedtls_net_init(mbedtls_net_context *ctx)
{
    ctx->hdl = NULL;
    ctx->fd_priv = NULL;
    ctx->cb_func = NULL;
    ctx->priv = NULL;
    ctx->send_to_ms = 0;
    ctx->recv_to_ms = 0;
    ctx->handshake_ok = 0;
}
void mbedtls_net_set_cb(mbedtls_net_context *ctx, int (*cb_func)(enum sock_api_msg_type type, void *priv), void *priv)
{
    ctx->cb_func = cb_func;
    ctx->priv = priv;
}
void mbedtls_net_set_fd_priv(mbedtls_net_context *ctx, void *fd_priv)
{
    ctx->fd_priv = fd_priv;
}

void mbedtls_net_set_timeout(mbedtls_net_context *ctx, int send_to_ms, int recv_to_ms)
{
    ctx->send_to_ms = send_to_ms;
    ctx->recv_to_ms = recv_to_ms;
}
int mbedtls_net_connect_bind(mbedtls_net_context *ctx, int domain, int socktype, int ai_protocol, char *ipaddr, u16 port)
{
    struct sockaddr_in local_addr;
    ctx->hdl = sock_reg(AF_UNSPEC, socktype, ai_protocol, ctx->cb_func, ctx->priv);
    if (ctx->hdl == NULL) {
        return -1;
    }


    sock_set_reuseaddr(ctx->hdl);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = port;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (sock_bind(ctx->hdl, (struct sockaddr *)&local_addr, sizeof(struct sockaddr)) < 0) {
        sock_unreg(ctx->hdl);
        ctx->hdl = NULL;
        return -1;
    }


    if (ctx->send_to_ms) {
        sock_set_send_timeout((void *)ctx->hdl, ctx->send_to_ms);
    }
    if (ctx->recv_to_ms) {
        sock_set_recv_timeout((void *)ctx->hdl, ctx->recv_to_ms);
    }

    if (ctx->connect_to_ms) {
        sock_set_connect_to((void *)ctx->hdl, ctx->connect_to_ms / 1000);
    }

    return 0;
}

/*
 * Initiate a TCP connection with host:port and the given protocol
 */
int mbedtls_net_connect(mbedtls_net_context *ctx, const char *host, const char *port, int proto)
{
    int ret;
    struct addrinfo hints, *addr_list, *cur;
    struct sockaddr_in romte_addr;

    if ((ret = net_prepare()) != 0) {
        return (ret);
    }

    /* Do name resolution with both IPv6 and IPv4 */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = proto == MBEDTLS_NET_PROTO_UDP ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_protocol = proto == MBEDTLS_NET_PROTO_UDP ? IPPROTO_UDP : IPPROTO_TCP;


    if (ctx->hdl == NULL) {

        if ((ret = mbedtls_net_connect_bind(ctx, AF_INET, hints.ai_socktype, 0, 0, 0)) != 0) {
            return ret;
        }
    }


    if (getaddrinfo(host, port, &hints, &addr_list) != 0) {
        return (MBEDTLS_ERR_NET_UNKNOWN_HOST);
    }

    /* Try the sockaddrs until a connection succeeds */
    ret = MBEDTLS_ERR_NET_UNKNOWN_HOST;

    for (cur = addr_list; cur != NULL; cur = cur->ai_next) {

        if (sock_connect(ctx->hdl, cur->ai_addr, MSVC_INT_CAST cur->ai_addrlen) == 0) {
            ret = 0;
            break;
        }

        ret = MBEDTLS_ERR_NET_CONNECT_FAILED;
    }

    freeaddrinfo(addr_list);


    return (ret);
}

/*
 * Create a listening socket on bind_ip:port
 */
int mbedtls_net_bind(mbedtls_net_context *ctx, const char *bind_ip, const char *port, int proto)
{
    int ret;
    struct addrinfo hints, *addr_list, *cur;

    if ((ret = net_prepare()) != 0) {
        return (ret);
    }

    /* Bind to IPv6 and/or IPv4, but only in the desired protocol */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = proto == MBEDTLS_NET_PROTO_UDP ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_protocol = proto == MBEDTLS_NET_PROTO_UDP ? IPPROTO_UDP : IPPROTO_TCP;

    if (bind_ip == NULL) {
        hints.ai_flags = AI_PASSIVE;
    }

    if (getaddrinfo(bind_ip, port, &hints, &addr_list) != 0) {
        return (MBEDTLS_ERR_NET_UNKNOWN_HOST);
    }

    /* Try the sockaddrs until a binding succeeds */
    ret = MBEDTLS_ERR_NET_UNKNOWN_HOST;

    for (cur = addr_list; cur != NULL; cur = cur->ai_next) {

        ctx->hdl = sock_reg(cur->ai_family, cur->ai_socktype, cur->ai_protocol, ctx->cb_func, ctx->priv);

        if (ctx->hdl == 0) {
            ret = MBEDTLS_ERR_NET_SOCKET_FAILED;
            continue;
        }

        if (ctx->send_to_ms) {
            sock_set_send_timeout((void *)ctx->hdl, ctx->send_to_ms);
        }
        if (ctx->recv_to_ms) {
            sock_set_recv_timeout((void *)ctx->hdl, ctx->recv_to_ms);
        }

        if (sock_set_reuseaddr(ctx->hdl) != 0) {
            printf("%s :::: %d quit : %d\n", __func__, __LINE__, ctx->hdl->quit);
            sock_unreg(ctx->hdl);
            ctx->hdl = NULL;
            ret = MBEDTLS_ERR_NET_SOCKET_FAILED;
            continue;
        }

        if (sock_bind(ctx->hdl, cur->ai_addr, MSVC_INT_CAST cur->ai_addrlen) != 0) {
            printf("%s :::: %d quit : %d\n", __func__, __LINE__, ctx->hdl->quit);
            sock_unreg(ctx->hdl);
            ctx->hdl = NULL;
            ret = MBEDTLS_ERR_NET_BIND_FAILED;
            continue;
        }

        /* Listen only makes sense for TCP */
        if (proto == MBEDTLS_NET_PROTO_TCP) {
            if (sock_listen(ctx->hdl, MBEDTLS_NET_LISTEN_BACKLOG) != 0) {
                printf("%s :::: %d quit : %d\n", __func__, __LINE__, ctx->hdl->quit);
                sock_unreg(ctx->hdl);
                ctx->hdl = NULL;
                ret = MBEDTLS_ERR_NET_LISTEN_FAILED;
                continue;
            }
        }

        /* I we ever get there, it's a success */
        ret = 0;
        break;
    }

    freeaddrinfo(addr_list);

    return (ret);

}

#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
/*
 * Check if the requested operation would be blocking on a non-blocking socket
 * and thus 'failed' with a negative return value.
 */
static int net_would_block(const mbedtls_net_context *ctx)
{
    ((void) ctx);
    return (WSAGetLastError() == WSAEWOULDBLOCK);
}
#else
/*
 * Check if the requested operation would be blocking on a non-blocking socket
 * and thus 'failed' with a negative return value.
 *
 * Note: on a blocking socket this function always returns 0!
 */
static int net_would_block(const mbedtls_net_context *ctx)
{
    /*
     * Never return 'WOULD BLOCK' on a non-blocking socket
     */
    if ((sock_fcntl(ctx->hdl, F_GETFL, 0) & O_NONBLOCK) != O_NONBLOCK) {
        return (0);
    }

    switch (errno) {
#if defined EAGAIN

    case EAGAIN:
#endif
#if defined EWOULDBLOCK && EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
#endif
        return (1);
    }

    return (0);
}
#endif /* ( _WIN32 || _WIN32_WCE ) && !EFIX64 && !EFI32 */

/*
 * Accept a connection from a remote client
 */
int mbedtls_net_accept(mbedtls_net_context *bind_ctx,
                       mbedtls_net_context *client_ctx,
                       void *client_ip, size_t buf_size, size_t *ip_len)
{
    int ret;
    int type;

    struct sockaddr_storage client_addr;

#if defined(__socklen_t_defined) || defined(_SOCKLEN_T) ||  \
    defined(_SOCKLEN_T_DECLARED) || defined(__DEFINED_socklen_t)
    socklen_t n = (socklen_t) sizeof(client_addr);
    socklen_t type_len = (socklen_t) sizeof(type);
#else
    int n = (int) sizeof(client_addr);
    int type_len = (int) sizeof(type);
#endif
    /* Is this a TCP or UDP socket? */
    if (sock_getsockopt(bind_ctx->hdl, SOL_SOCKET, SO_TYPE,
                        (void *) &type, (socklen_t *)(&type_len)) != 0 ||
        (type != SOCK_STREAM && type != SOCK_DGRAM)) {
        return (MBEDTLS_ERR_NET_ACCEPT_FAILED);
    }

    if (type == SOCK_STREAM) {
        /* TCP: actual accept() */
        client_ctx->hdl =  sock_accept(bind_ctx->hdl, (struct sockaddr *) &client_addr, (socklen_t *)(&n), bind_ctx->cb_func, bind_ctx->priv);
        ret = (int)client_ctx->hdl;

        if (ret == 0) {
            if (net_would_block(bind_ctx) != 0) {
                return (MBEDTLS_ERR_SSL_WANT_READ);
            }

            return (MBEDTLS_ERR_NET_ACCEPT_FAILED);
        }
    } else {
        /* UDP: wait for a message, but keep it in the queue */
        char buf[1] = { 0 };

        ret =  sock_recvfrom(bind_ctx->hdl, buf, sizeof(buf), MSG_PEEK,
                             (struct sockaddr *) &client_addr, (socklen_t *)(&n));

#if defined(_WIN32)

        if (ret == SOCKET_ERROR &&
            WSAGetLastError() == WSAEMSGSIZE) {
            /* We know buf is too small, thanks, just peeking here */
            ret = 0;
        }

#endif

        if (ret < 0) {
            if (net_would_block(bind_ctx) != 0) {
                return (MBEDTLS_ERR_SSL_WANT_READ);
            }

            return (MBEDTLS_ERR_NET_ACCEPT_FAILED);
        }
    }

    /* UDP: hijack the listening socket to communicate with the client,
     * then bind a new socket to accept new connections */
    if (type != SOCK_STREAM) {
        struct sockaddr_storage local_addr;

        if (sock_connect(bind_ctx->hdl, (struct sockaddr *) &client_addr, n) != 0) {
            return (MBEDTLS_ERR_NET_ACCEPT_FAILED);
        }

        client_ctx->hdl = bind_ctx->hdl;
        bind_ctx->hdl   = NULL; /* In case we exit early */

        n = sizeof(struct sockaddr_storage);

        if (sock_getsockname(client_ctx->hdl, (struct sockaddr *) &local_addr, (socklen_t *)(&n)) == 0) {
            bind_ctx->hdl = sock_reg(local_addr.ss_family, SOCK_DGRAM, IPPROTO_UDP, bind_ctx->cb_func, bind_ctx->priv);
            if (bind_ctx->hdl) {
                if (sock_set_reuseaddr(bind_ctx->hdl)) {
                    sock_unreg(bind_ctx->hdl);
                    bind_ctx->hdl = NULL;
                    return (MBEDTLS_ERR_NET_SOCKET_FAILED);
                }
            }
        } else {
            return (MBEDTLS_ERR_NET_SOCKET_FAILED);
        }

        if (sock_bind(bind_ctx->hdl, (struct sockaddr *) &local_addr, n) != 0) {
            return (MBEDTLS_ERR_NET_BIND_FAILED);
        }

        if (bind_ctx->send_to_ms) {
            sock_set_send_timeout((void *)bind_ctx->hdl, bind_ctx->send_to_ms);
        }
        if (bind_ctx->recv_to_ms) {
            sock_set_recv_timeout((void *)bind_ctx->hdl, bind_ctx->recv_to_ms);
        }
    }

    if (client_ip != NULL) {
        if (client_addr.ss_family == AF_INET) {
            struct sockaddr_in *addr4 = (struct sockaddr_in *) &client_addr;
            *ip_len = sizeof(struct sockaddr_in);

            if (buf_size < *ip_len) {
                return (MBEDTLS_ERR_NET_BUFFER_TOO_SMALL);
            }

            memcpy(client_ip, addr4, *ip_len);
        } else {
#if LWIP_IPV6
            struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) &client_addr;
            *ip_len = sizeof(struct sockaddr_in6);

            if (buf_size < *ip_len) {
                return (MBEDTLS_ERR_NET_BUFFER_TOO_SMALL);
            }

            memcpy(client_ip, addr6, *ip_len);
#endif
        }
    }

    return (0);
}

/*
 * Set the socket blocking or non-blocking
 */
int mbedtls_net_set_block(mbedtls_net_context *ctx)
{
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
    u_long n = 0;
    return (ioctlsocket(ctx->hdl, FIONBIO, &n));
#else
    return (sock_fcntl(ctx->hdl, F_SETFL, sock_fcntl(ctx->hdl, F_GETFL, 0) & ~O_NONBLOCK));    //MODIFY BY SHUNJIAN
#endif
}

int mbedtls_net_set_nonblock(mbedtls_net_context *ctx)
{
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
    u_long n = 1;
    return (ioctlsocket(ctx->hdl, FIONBIO, &n));
#else
    return (sock_fcntl(ctx->hdl, F_SETFL, sock_fcntl(ctx->hdl, F_GETFL, 0) | O_NONBLOCK));    //MODIFY BY SHUNJIAN
#endif
}

/*
 * Portable usleep helper
 */
void udelay(unsigned int t);

void mbedtls_net_usleep(unsigned long usec)
{
#if defined(_WIN32)
    Sleep((usec + 999) / 1000);
#else
    struct timeval tv;
    tv.tv_sec  = usec / 1000000;
#if defined(__unix__) || defined(__unix) || \
    ( defined(__APPLE__) && defined(__MACH__) )
    tv.tv_usec = (suseconds_t) usec % 1000000;
#else
    tv.tv_usec = usec % 1000000;
#endif
    udelay(usec);
#endif
}

/*
 * Read at most 'len' characters
 */
int mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len)
{
    int ret;
    mbedtls_net_context *_ctx = (mbedtls_net_context *)ctx;
    void *fd = ((mbedtls_net_context *) ctx)->hdl;

    int flags = (int)(((mbedtls_net_context *) ctx)->fd_priv);

    if (fd == 0) {
        return (MBEDTLS_ERR_NET_INVALID_CONTEXT);
    }

    if (_ctx->handshake_ok) {
        /*printf(" ||| %s ,,, %d \r\n", __FUNCTION__, __LINE__);*/
        os_mutex_post(&_ctx->mutex);
    }
    if (flags == MSG_DONTWAIT) {
        ret = (int) sock_recv(fd, (char *)buf, len, MSG_DONTWAIT);
    } else {
        ret = (int) sock_recv(fd, (char *)buf, len, 0);
    }

    if (ret < 0) {
        if (net_would_block(ctx) != 0) {
            if (_ctx->handshake_ok) {
                os_mutex_pend(&_ctx->mutex, 0);
            }

            return (MBEDTLS_ERR_SSL_WANT_READ);
        }

#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)

        if (WSAGetLastError() == WSAECONNRESET) {
            if (_ctx->handshake_ok) {
                os_mutex_pend(&_ctx->mutex, 0);
            }
            if (_ctx->handshake_ok) {
                os_mutex_pend(&_ctx->mutex, 0);
            }

            return (MBEDTLS_ERR_NET_CONN_RESET);
        }

#else

        if (errno == EPIPE || errno == ECONNRESET) {
            if (_ctx->handshake_ok) {
                os_mutex_pend(&_ctx->mutex, 0);
            }

            return (MBEDTLS_ERR_NET_CONN_RESET);
        }

        if (errno == EINTR) {
            if (_ctx->handshake_ok) {
                os_mutex_pend(&_ctx->mutex, 0);
            }

            return (MBEDTLS_ERR_SSL_WANT_READ);
        }

#endif
        if (_ctx->handshake_ok) {
            os_mutex_pend(&_ctx->mutex, 0);
        }

        return (MBEDTLS_ERR_NET_RECV_FAILED);
    }
    if (_ctx->handshake_ok) {
        os_mutex_pend(&_ctx->mutex, 0);
    }

    return (ret);
}

int mbedtls_net_recv2(void *ctx, unsigned char *buf, size_t len)
{
    int ret;
    mbedtls_net_context *_ctx = (mbedtls_net_context *)ctx;
    void *fd = ((mbedtls_net_context *) ctx)->hdl;


    if (fd < 0) {
        return (MBEDTLS_ERR_NET_INVALID_CONTEXT);
    }

    ret = (int) read(fd, buf, len);

    if (ret < 0) {
        if (net_would_block(ctx) != 0) {
            return (MBEDTLS_ERR_SSL_WANT_READ);
        }

#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
        if (WSAGetLastError() == WSAECONNRESET) {
            return (MBEDTLS_ERR_NET_CONN_RESET);
        }
#else
        if (errno == EPIPE || errno == ECONNRESET) {
            return (MBEDTLS_ERR_NET_CONN_RESET);
        }

        if (errno == EINTR) {
            return (MBEDTLS_ERR_SSL_WANT_READ);
        }
#endif

        return (MBEDTLS_ERR_NET_RECV_FAILED);
    }

    return (ret);
}

/*
 * Read at most 'len' characters, blocking for at most 'timeout' ms
 */
int mbedtls_net_recv_timeout(void *ctx, unsigned char *buf, size_t len,
                             uint32_t timeout)
{
    int ret;
    mbedtls_net_context *_ctx = (mbedtls_net_context *)ctx;
    struct timeval tv;
    fd_set read_fds;
    void *fd = ((mbedtls_net_context *) ctx)->hdl;

    if (fd == 0) {
        return (MBEDTLS_ERR_NET_INVALID_CONTEXT);
    }

    if (_ctx->handshake_ok) {
        /*printf(" ||| %s ,,, %d \r\n", __FUNCTION__, __LINE__);*/
        os_mutex_post(&_ctx->mutex);
    }

    FD_ZERO(&read_fds);
    FD_SET(sock_get_socket(fd), &read_fds);

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    ret = sock_select(fd, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv);

    /* Zero fds ready means we timed out */
    if (ret == 0) {

        if (_ctx->handshake_ok) {
            os_mutex_pend(&_ctx->mutex, 0);
        }

        return (MBEDTLS_ERR_SSL_TIMEOUT);
    }

    if (ret < 0) {
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)

        if (WSAGetLastError() == WSAEINTR) {

            if (_ctx->handshake_ok) {
                os_mutex_pend(&_ctx->mutex, 0);
            }
            return (MBEDTLS_ERR_SSL_WANT_READ);
        }

#else

        if (errno == EINTR) {

            if (_ctx->handshake_ok) {
                os_mutex_pend(&_ctx->mutex, 0);
            }

            return (MBEDTLS_ERR_SSL_WANT_READ);
        }

#endif

        if (_ctx->handshake_ok) {
            os_mutex_pend(&_ctx->mutex, 0);
        }
        return (MBEDTLS_ERR_NET_RECV_FAILED);
    }

    if (_ctx->handshake_ok) {
        os_mutex_pend(&_ctx->mutex, 0);
    }
    /* This call will not block */
    return (mbedtls_net_recv(ctx, buf, len));
}

/*
 * Write at most 'len' characters
 */
int mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len)
{
    int ret;
    void *fd = ((mbedtls_net_context *) ctx)->hdl;
    mbedtls_net_context *_ctx = (mbedtls_net_context *)ctx;

    if (fd == 0) {
        return (MBEDTLS_ERR_NET_INVALID_CONTEXT);
    }
    ret =  sock_send(fd, (char *)buf, len, 0);

    if (ret < 0) {
        if (net_would_block(ctx) != 0) {

            return (MBEDTLS_ERR_SSL_WANT_WRITE);
        }

#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)

        if (WSAGetLastError() == WSAECONNRESET) {

            return (MBEDTLS_ERR_NET_CONN_RESET);
        }

#else

        if (errno == EPIPE || errno == ECONNRESET) {

            return (MBEDTLS_ERR_NET_CONN_RESET);
        }

        if (errno == EINTR) {

            return (MBEDTLS_ERR_SSL_WANT_WRITE);
        }

#endif

        return (MBEDTLS_ERR_NET_SEND_FAILED);
    }
    return (ret);
}

int mbedtls_net_send2(void *ctx, const unsigned char *buf, size_t len)
{
    int ret;
    mbedtls_net_context *_ctx = (mbedtls_net_context *)ctx;
    void *fd = ((mbedtls_net_context *) ctx)->hdl;


    if (fd < 0) {
        return (MBEDTLS_ERR_NET_INVALID_CONTEXT);
    }

    printf("len=>%d\n", len);
    ret = (int) write(fd, buf, len);

    if (ret < 0) {
        if (net_would_block(ctx) != 0) {
            return (MBEDTLS_ERR_SSL_WANT_WRITE);
        }

#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
        if (WSAGetLastError() == WSAECONNRESET) {
            return (MBEDTLS_ERR_NET_CONN_RESET);
        }
#else
        if (errno == EPIPE || errno == ECONNRESET) {
            return (MBEDTLS_ERR_NET_CONN_RESET);
        }

        if (errno == EINTR) {
            return (MBEDTLS_ERR_SSL_WANT_WRITE);
        }
#endif

        return (MBEDTLS_ERR_NET_SEND_FAILED);
    }

    return (ret);
}

/*
 * Gracefully close the connection
 */
void mbedtls_net_free(mbedtls_net_context *ctx)
{
    if (ctx->hdl == NULL) {
        return;
    }

    sock_unreg(ctx->hdl);

    ctx->hdl = NULL;
}
int mbedtls_ssl_read_ext(mbedtls_ssl_context *ssl, unsigned char *buf, size_t len)
{
    int ret;
    unsigned int already_read;
    int flags = (int)(((mbedtls_net_context *) ssl->p_bio)->fd_priv);

    if (flags == MSG_WAITALL) {
        already_read = 0;

        do {
            ret = mbedtls_ssl_read(ssl, buf + already_read, len - already_read);

            if (ret <= 0) {
                return ret;
            }

            already_read += ret;
        } while (already_read < len);
    } else {
        already_read = mbedtls_ssl_read(ssl, buf, len);
    }

    return already_read;
}
#endif /* MBEDTLS_NET_C */
