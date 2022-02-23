#ifndef _CPU_CLOCK_
#define _CPU_CLOCK_

#include "typedef.h"


int clk_early_init();

#define     SYS_CLK_320M	320000000L
#define     SYS_CLK_240M	240000000L
#define     SYS_CLK_192M	192000000L
#define     SYS_CLK_160M	160000000L
#define     SYS_CLK_120M	120000000L
#define     SYS_CLK_96M		96000000L
#define     SYS_CLK_48M		48000000L
#define     SYS_CLK_40M		40000000L
#define     SYS_CLK_24M		24000000L
#define     SYS_CLK_12M		12000000L
#define     SYS_CLK_8M		8000000L
#define     SYS_CLK_4M	    4800000L
#define     SDRAM_CLK_MIN	16000000L

/*
 * system enter critical and exit critical handle
 * */
struct clock_critical_handler {
    void (*enter)();
    void (*exit)();
};

#define CLOCK_CRITICAL_HANDLE_REG(name, enter, exit) \
	const struct clock_critical_handler clock_##name \
		 SEC_USED(.clock_critical_txt) = {enter, exit};

extern struct clock_critical_handler clock_critical_handler_begin[];
extern struct clock_critical_handler clock_critical_handler_end[];

#define list_for_each_loop_clock_critical(h) \
	for (h=clock_critical_handler_begin; h<clock_critical_handler_end; h++)


int clk_get(const char *name);

int clk_set(const char *name, int clk);

int sys_clk_source_set(char *name);//"BT_OSC" "RTC_SOC" "PLL_OSC" "RC_16M" "RC_250K"
void sys_clk_source_resume(int value);
void sys_clk_source_rc_clk_on(int is_16M);//RC_16M / RC_250K
void sys_clk_source_rc_clk_off(void);
u32 clk_get_osc_cap();

//系统空闲模式时钟切换函数
void system_clock_set_idle(void);

//系统时钟切换函数
void system_clock_set(u32 clock);

//系统时钟+sdram时钟切换
void system_sdram_clock_set(u32 sys_clk, u32 sdram_clk);

//sdram时钟切换
void sdram_clock_set(u32 clock);

//恢复SDK默认系统+sdram时钟
void system_clock_set_default(void);

#endif

