#include "system/init.h"
#include "lwip.h"
#include "app_config.h"
#include "event/event.h"
#include "event/device_event.h"
#include "lte_module.h"
#include "log.h"

#define     __LOG_LEVEL     __LOG_DEBUG

static char ip_addr[32];
static char gw_addr[32];
static void *dev = NULL;
static int lte_app_task_pid;


int lwip_event_cb(void *lwip_ctx, enum LWIP_EVENT event)
{
    switch (event) {
    case LWIP_LTE_DHCP_BOUND_TIMEOUT:
        break;

    case LWIP_LTE_DHCP_BOUND_SUCC:
        Get_IPAddress(LTE_NETIF, ip_addr);
        get_gateway(LTE_NETIF, gw_addr);
        log_i("LTE DHCP SUCC, IP:[%s] \r\n, GW:[%s]", ip_addr, gw_addr);
        /* log_i("PBUF_POOL_BUFSIZE = %d\n", PBUF_POOL_BUFSIZE); */
        void lte_network_test(void);
        lte_network_test();
        break;

    default:
        break;
    }

    return 0;
}


char *get_lte_ip(void)
{
    return &ip_addr[0];
}


static void lte_app_task(void *arg)
{
    //添加网络应用程序在这里

    while (1) {
        msleep(10 * 1000);

        extern u32 lte_get_upload_rate(void);
        extern u32 lte_get_download_rate(void);
        log_d("ETH U= %d KB/s, D= %d KB/s\r\n", lte_get_upload_rate() / 1024, lte_get_download_rate() / 1024);
    }
}


static int lte_state_cb(void *priv, int on)
{
    if (on) {
        log_i("lte on \r\n");
        dev_ioctl(dev, LTE_NETWORK_START, NULL);
        if (lte_app_task_pid == 0) {
            thread_fork("lte_app_task", 20, 0x1000, 0, &lte_app_task_pid, lte_app_task, NULL);
        }
    } else {
        log_i("lte off \r\n");
    }
    return 0;
}


static int lte_net_init(void)
{
    dev = dev_open("lte", NULL);
    dev_ioctl(dev, LTE_DEV_SET_CB, (u32)lte_state_cb);

    if (dev) {
        log_i("lte early init succ\n\n\n\n\n\n");
    } else {
        log_e("lte early init fail\n\n\n\n\n\n");
    }

    return 0;
}

#ifdef CONFIG_LTE_PHY_ENABLE
late_initcall(lte_net_init);
#endif


