#include <unistd.h>
#include "py/mpconfig.h"
#include <stdlib.h>
#include <string.h>
#include "device/device.h"
#include "device/uart.h"
#include "init.h"
#include "fs/fs.h"

/*
 * 这里采用uart0
 */

static u8 buf[1 * 1024] __attribute__((aligned(32)));
static void *mp_hdl;

static void uart_test_task(void *priv)
{
    int len = 0;
    len = sizeof(buf);
    int cnt = 0;
    mp_hdl = dev_open("uart0", NULL);

    if (!mp_hdl) {
        printf("open uart err !!!\n");
        return ;
    }

    dev_ioctl(mp_hdl, UART_SET_CIRCULAR_BUFF_ADDR, (int)buf);

    dev_ioctl(mp_hdl, UART_SET_CIRCULAR_BUFF_LENTH, len);

    dev_ioctl(mp_hdl, UART_SET_RECV_BLOCK, 1);

    dev_ioctl(mp_hdl, UART_START, 0);
}

int mp_hal_stdin_rx_chr(void)
{
    unsigned char c = 0;
    int len = 0;
    while (1) {
        len = dev_read(mp_hdl, &c, 1);
        if (len <= 0) {
            if (len == -1) {
                dev_ioctl(mp_hdl, UART_FLUSH, 0);
            }
            os_time_dly(1);
            continue;
        } else {
            return c;
        }
    }
}

late_initcall(uart_test_task);

static int mp_start = 1;
void mp_hal_stdout_tx_strn(const char *str, int len)
{
    if (mp_start) {
        os_time_dly(1);
        mp_start = 0;
    }

    while (len--) {
        dev_write(mp_hdl, str++, 1);
    }
}

void mp_hal_stdout_tx_str(const char *str)
{
    mp_hal_stdout_tx_strn(str, strlen(str));
}

void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len)
{
    const char *last = str;
    while (len--) {
        if (*str == '\n') {
            if (str > last) {
                mp_hal_stdout_tx_strn(last, str - last);
            }
            mp_hal_stdout_tx_strn("\r\n", 2);
            ++str;
            last = str;
        } else {
            ++str;
        }
    }
    if (str > last) {
        mp_hal_stdout_tx_strn(last, str - last);
    }
}

