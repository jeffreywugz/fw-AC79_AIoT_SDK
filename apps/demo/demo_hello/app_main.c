#include "app_config.h"
#include "system/includes.h"
#include "os/os_api.h"

/*中断列表 */
const struct irq_info irq_info_table[] = {
#if CPU_CORE_NUM == 1
    { IRQ_SOFT5_IDX,      7,   0    }, //此中断强制注册到cpu0
    { IRQ_SOFT4_IDX,      7,   1    }, //此中断强制注册到cpu1
    { -2,     			-2,   -2   },//如果加入了该行, 那么只有该行之前的中断注册到对应核, 其他所有中断强制注册到CPU0
#endif

    { -1,     -1,   -1    },
};

/*任务列表 */
const struct task_info task_info_table[] = {
    {"app_core",            15,     2048,	  1024 },
    {"sys_event",           29,     512,	   0 },
    {"systimer",            14,     256, 	   0 },
    {"sys_timer",            9,     512,	  128 },

    {0, 0, 0, 0, 0},
};


/*
 * 应用程序主函数
 */
static void hello_demo_task(void *p)
{
    while (1) {
        os_time_dly(200);
    }
}

void app_main()
{
    printf("\r\n\r\n\r\n\r\n\r\n -----------hello demo run %s-------------\r\n\r\n\r\n\r\n\r\n", __TIME__);
    os_task_create(hello_demo_task, NULL, 10, 1000, 0, "hello_demo_task");
}

