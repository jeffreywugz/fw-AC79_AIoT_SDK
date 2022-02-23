#include "app_config.h"
#include "system/includes.h"
#include "device/device.h"
#include <time.h>
#include <sys/time.h>

#ifdef CONFIG_WIFI_ENABLE
#include "wifi/wifi_connect.h"
#endif

//当应用层不定义该函数时，系统默认时间为SDK发布时间，当RTC设置时间小于SDK发布时间则设置无效
void set_rtc_default_time(struct sys_time *t)
{
    t->year = 2020;
    t->month = 6;
    t->day = 1;
    t->hour = 8;
    t->min = 8;
    t->sec = 8;
}

static void time_print(void)//网络时间获取方法
{
    char time_str[64];
    struct tm timeinfo;
    time_t timestamp;

    timestamp = time(NULL) + 28800;
    localtime_r(&timestamp, &timeinfo);
    strftime(time_str, sizeof(time_str), "%Y-%m-%dT%H:%M:%S", &timeinfo);
    printf("RTC TIME [%s]\n", time_str);

    printf("system boot run time [%d sec] [%d msec]\n", timer_get_sec(), timer_get_ms());
    printf("jiffies run time [%d sec]\n", jiffies * 10);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("gettimeofday [%d sec+%d usec]\n", tv.tv_sec, tv.tv_usec);

    static u32 time_lapse_hdl = 0; //需要初始化为0,句柄在time_lapse使用过程中不可消亡
    u32 to = time_lapse(&time_lapse_hdl, 5000); //指定时间后返回真,并且返回比上一次执行的时间经过了多少毫秒
    if (to) {
        printf("time_lapse timeout %d msec\r\n", to);
    }
}
void user_set_rtc_time(void)//用户设置RTC时间
{
    struct sys_time st;
    void *fd = dev_open("rtc", NULL);
    if (fd) {
        st.year = 2020;
        st.month = 2;
        st.day = 2;
        st.hour = 2;
        st.min = 2;
        st.sec = 2;
        printf("user_set_rtc_time : %04d-%02d-%02d,%02d:%02d:%02d\n", st.year, st.month, st.day, st.hour, st.min, st.sec);
        dev_ioctl(fd, IOCTL_SET_SYS_TIME, (u32)&st);
        dev_close(fd);
    }
}
void user_get_rtc_time(void)//用户获取RTC时间
{
    struct sys_time st;
    void *fd = dev_open("rtc", NULL);
    if (fd) {
        dev_ioctl(fd, IOCTL_GET_SYS_TIME, (u32)&st);
        dev_close(fd);
        printf("user_get_rtc_time : %04d-%02d-%02d,%02d:%02d:%02d\n", st.year, st.month, st.day, st.hour, st.min, st.sec);
    }
}
static void user_get_rtc_time_cyc(void)//用户循环获取RTC时间
{
    struct sys_time st = {0};
    struct sys_time st1 = {0};
    void *fd = dev_open("rtc", NULL);
    if (!fd) {
        while (1) {
            printf("err in dev_open rtc\n");
        }
    }
    while (1) {
        os_time_dly(10);//因为本身os_time_dly(100) 1S就是不准确，所以改为10，100ms检测有没有发生变化，有变化才打印
        dev_ioctl(fd, IOCTL_GET_SYS_TIME, (u32)&st1);
        if (memcmp(&st, &st1, sizeof(struct sys_time))) { //有变化才打印
            memcpy(&st, &st1, sizeof(struct sys_time)); //复制读取的数据
            //此时，可以发送相关的数据数据到另外线程或此时显示UI等，使用的结构体为st

            printf("user_get_rtc_time : %04d-%02d-%02d,%02d:%02d:%02d\n", st.year, st.month, st.day, st.hour, st.min, st.sec);
        }
    }
    dev_close(fd);
}
static void time_rtc_test_task(void *p)
{
#ifndef CONFIG_OSC_RTC_ENABLE
    os_time_dly(100);//使用内部的RC32K，则等待1秒校准之后才能设置和读取
#endif
    //此处默认不使用网络时间，需要则打开
#if 0 //def CONFIG_WIFI_ENABLE //网络时间,当不需要网络时间则不需以下代码操作
    while (wifi_get_sta_connect_state() != WIFI_STA_NETWORK_STACK_DHCP_SUCC) {
        printf("Waitting STA Connected...\r\n");
        //当网络连接成功前, 获取的是同步网络时间前的RTC时间
        //当网络连接成功后并且连接远端NTP服务商成功, 执行time函数会获取网络时间同步本地RTC时间
        time_print();
        os_time_dly(100);
    }
    //联网成功后，系统自动把网络时间同步到系统RTC实际
    while (1) {
        time_print();  //当网络连接成功前, 获取的是同步网络时间前的RTC时间
        os_time_dly(300);
    }
#else //本地RTC获取
    user_get_rtc_time();
    user_set_rtc_time();
#if 1
    user_get_rtc_time_cyc();//100ms检测一次时间变化,
#else
    while (1) {
        os_time_dly(100);//1s更新时间一次，注意：不一定两次之间的秒数一定是间隔1
        user_get_rtc_time();
    }
#endif
#endif
}

static int c_main(void)
{
    os_task_create(time_rtc_test_task, NULL, 10, 1000, 0, "time_rtc_test_task");
    return 0;
}
late_initcall(c_main);
