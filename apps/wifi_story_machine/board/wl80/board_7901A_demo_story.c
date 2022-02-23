#include "app_config.h"

#ifdef CONFIG_BOARD_7901A_DEMO_STORY
#include "app_power_manage.h"
#include "system/includes.h"
#include "device/includes.h"
#include "device/iic.h"
#include "server/audio_dev.h"
#include "asm/includes.h"
#if TCFG_USB_SLAVE_ENABLE || TCFG_USB_HOST_ENABLE
#include "otg.h"
#include "usb_host.h"
#include "usb_storage.h"
#endif

// *INDENT-OFF*

UART0_PLATFORM_DATA_BEGIN(uart0_data)
    .baudrate = 115200,
    /* .baudrate = 1000000, */
    /* .port = PORTA_5_6, */
    /* .port = PORTB_1_0, */
    .port = PORTA_3_4,
    /* .port = PORTH_0_1, */
    /* .tx_pin = IO_PORTA_05, */
    /* .rx_pin = IO_PORTA_06, */
    /* .tx_pin = IO_PORTB_01, */
    /* .rx_pin = IO_PORTB_00, */
    .tx_pin = IO_PORTA_03,
    .rx_pin = IO_PORTA_04,
    /* .tx_pin = IO_PORTH_00, */
    /* .rx_pin = IO_PORTH_01, */
    .max_continue_recv_cnt = 1024,
    .idle_sys_clk_cnt = 500000,
    .clk_src = PLL_48M,
    .flags = UART_DEBUG,
UART0_PLATFORM_DATA_END();


UART1_PLATFORM_DATA_BEGIN(uart1_data)
#if !defined CONFIG_BT_TEST_ENABLE && (defined CONFIG_MP_TX_TEST_ENABLE || defined CONFIG_MP_RX_TEST_ENABLE || CONFIG_VOLUME_TAB_TEST_ENABLE)
    .baudrate = 115200,
    .disable_tx_irq = 1,
#else
    .baudrate = 115200,
#endif
    /* .port = PORT_REMAP, */
    /* .port = PORTH_6_7, */
    /* .port = PORTC_3_4, */
    /* .port = PORTUSB_B, */
    .port = PORTUSB_A,
    /* .tx_pin = IO_PORTH_06, */
    /* .rx_pin = IO_PORTH_07, */
    /* .tx_pin = IO_PORTC_03, */
    /* .rx_pin = IO_PORTC_04, */
    /* .tx_pin = IO_PORT_USB_DPB, */
    /* .rx_pin = IO_PORT_USB_DMB, */
    .tx_pin = IO_PORT_USB_DPA,
    .rx_pin = IO_PORT_USB_DMA,
    /* .tx_pin = IO_PORTB_06, */
    /* .rx_pin = IO_PORTB_07, */
    .max_continue_recv_cnt = 1024,
    .idle_sys_clk_cnt = 500000,
    .clk_src = PLL_48M,
    .flags = UART_DEBUG,
UART1_PLATFORM_DATA_END();


#if defined CONFIG_BT_TEST_ENABLE && defined CONFIG_MP_TX_TEST_ENABLE
struct uart_platform_data uart2_data = {
    .baudrate = 115200,
#else
UART2_PLATFORM_DATA_BEGIN(uart2_data)
    .baudrate = 1000000,
#endif
    /* .port = PORTH_2_3, */
    /* .port = PORTC_9_10, */
    /* .port = PORTB_6_7, */
    /* .port = PORTE_0_1, */
    /* .tx_pin = IO_PORTH_02, */
    /* .rx_pin = IO_PORTH_03, */
    /* .tx_pin = IO_PORTC_09, */
    /* .rx_pin = IO_PORTC_10, */
    /* .tx_pin = IO_PORTB_06, */
    /* .rx_pin = IO_PORTB_07, */
    /* .tx_pin = IO_PORTE_00,*/
    /* .rx_pin = IO_PORTE_01,*/
    .port = PORT_REMAP,
    .output_channel = OUTPUT_CHANNEL0,
    .tx_pin = IO_PORTC_03,
    .rx_pin = -1,
    .max_continue_recv_cnt = 1024,
    .idle_sys_clk_cnt = 500000,
    .clk_src = PLL_48M,
    .flags = UART_DEBUG,
UART2_PLATFORM_DATA_END();


const char *mp_test_get_uart(void)
{
    return "uart1";
}

/****************SD模块**********************************************************
 * 暂只支持1个SD外设，如需要多个SD外设，自行去掉下面SD0/1_ENABLE控制，另外定义即可
********************************************************************************/
#if TCFG_SD0_ENABLE
//sd0
#define SD_PLATFORM_DATA_BEGIN() SD0_PLATFORM_DATA_BEGIN(sd0_data)
#define SD_PLATFORM_DATA_END() SD0_PLATFORM_DATA_END()
#define SD_CLK_DETECT_FUNC		sdmmc_0_clk_detect
#elif TCFG_SD1_ENABLE
//sd1
#define SD_PLATFORM_DATA_BEGIN() SD1_PLATFORM_DATA_BEGIN(sd1_data)
#define SD_PLATFORM_DATA_END() SD1_PLATFORM_DATA_END()
#define SD_CLK_DETECT_FUNC		sdmmc_1_clk_detect
#endif

#if TCFG_SD0_ENABLE || TCFG_SD1_ENABLE

SD_PLATFORM_DATA_BEGIN()
	.port 					= TCFG_SD_PORTS,
	.priority 				= 3,
	.data_width 			= TCFG_SD_DAT_WIDTH,
	.speed 					= TCFG_SD_CLK,
	.detect_mode 			= TCFG_SD_DET_MODE,
#if (TCFG_SD_DET_MODE == SD_CLK_DECT)
	.detect_func 			= SD_CLK_DETECT_FUNC,
#elif (TCFG_SD_DET_MODE == SD_IO_DECT)
	.detect_func 			= sdmmc_0_io_detect,
#else
	.detect_func 			= NULL,
#endif
SD_PLATFORM_DATA_END()

#endif
/****************SD模块*********************/


HW_IIC0_PLATFORM_DATA_BEGIN(hw_iic1_data)
    /* .clk_pin = IO_PORTA_07, */
    /* .dat_pin = IO_PORTA_08, */
    .clk_pin = IO_PORTC_01,
    .dat_pin = IO_PORTC_02,
    /* .clk_pin = IO_PORTH_00, */
    /* .dat_pin = IO_PORTH_01, */
    .baudrate = 0x3f,//3f:385k
HW_IIC0_PLATFORM_DATA_END()


SW_IIC_PLATFORM_DATA_BEGIN(sw_iic0_data)
    .clk_pin = IO_PORTC_01,
    .dat_pin = IO_PORTC_02,
    .sw_iic_delay = 50,
SW_IIC_PLATFORM_DATA_END()


#if TCFG_ADKEY_ENABLE

#define ADKEY_UPLOAD_R  22

#define ADC_VDDIO (0x3FF)
#define ADC_09   (0x3FF)
#define ADC_08   (0x3FF)
#define ADC_07   (0x3FF)
#define ADC_06   (0x3FF)
#define ADC_05   (0x3FF)
#define ADC_04   (0x3FF)
#define ADC_03   (0x3FF * 15  / (15  + ADKEY_UPLOAD_R))
#define ADC_02   (0x3FF * 10  / (10  + ADKEY_UPLOAD_R))
#define ADC_01   (0x3FF * 33  / (33  + ADKEY_UPLOAD_R * 10))
#define ADC_00   (0)

#define ADKEY_V_9      	((ADC_09 + ADC_VDDIO)/2)
#define ADKEY_V_8 		((ADC_08 + ADC_09)/2)
#define ADKEY_V_7 		((ADC_07 + ADC_08)/2)
#define ADKEY_V_6 		((ADC_06 + ADC_07)/2)
#define ADKEY_V_5 		((ADC_05 + ADC_06)/2)
#define ADKEY_V_4 		((ADC_04 + ADC_05)/2)
#define ADKEY_V_3 		((ADC_03 + ADC_04)/2)
#define ADKEY_V_2 		((ADC_02 + ADC_03)/2)
#define ADKEY_V_1 		((ADC_01 + ADC_02)/2)
#define ADKEY_V_0 		((ADC_00 + ADC_01)/2)

const struct adkey_platform_data adkey_data = {
    .enable     = 1,
	.adkey_pin  = IO_PORTB_01,
    .extern_up_en = 1,
	.ad_channel = 3,
    .ad_value = {
        ADKEY_V_0,
        ADKEY_V_1,
        ADKEY_V_2,
        ADKEY_V_3,
        ADKEY_V_4,
        ADKEY_V_5,
        ADKEY_V_6,
        ADKEY_V_7,
        ADKEY_V_8,
        ADKEY_V_9,
    },
    .key_value = {
        KEY_CANCLE,
        KEY_DOWN,
        KEY_UP,
        KEY_MODE,
        NO_KEY,
        NO_KEY,
        NO_KEY,
        NO_KEY,
        NO_KEY,
        NO_KEY,
    },
};

#endif


/*
 * spi0接falsh
 */
SPI0_PLATFORM_DATA_BEGIN(spi0_data)
    .clk    = 40000000,
    .mode   = SPI_DUAL_MODE,
    .port   = 'A',
    .attr	= SPI_SCLK_L_UPL_SMPH | SPI_UPDATE_SAMPLE_SAME,
SPI0_PLATFORM_DATA_END()

SPI1_PLATFORM_DATA_BEGIN(spi1_data)
    .clk    = 20000000,
    .mode   = SPI_STD_MODE,
    .port   = 'B',
    .attr	= SPI_SCLK_L_UPH_SMPH | SPI_BIDIR_MODE,
SPI1_PLATFORM_DATA_END()

SPI2_PLATFORM_DATA_BEGIN(spi2_data)
    .clk    = 20000000,
    .mode   = SPI_STD_MODE,
    .port   = 'B',
    .attr	= SPI_SCLK_L_UPH_SMPH | SPI_BIDIR_MODE,
SPI2_PLATFORM_DATA_END()

static const struct spiflash_platform_data spiflash_data = {
    .name           = "spi0",
    .mode           = FAST_READ_OUTPUT_MODE,//FAST_READ_IO_MODE,
    .sfc_run_mode   = SFC_FAST_READ_DUAL_OUTPUT_MODE,
};

static const struct dac_platform_data dac_data = {
    .pa_mute_port = IO_PORTA_07,
    .pa_mute_value = 1,
    .differ_output = 0,
    .hw_channel = 0x03,
    .ch_num = 2,	//差分只需开一个通道
    .vcm_init_delay_ms = 1000,
#ifdef CONFIG_DEC_ANALOG_VOLUME_ENABLE
    .fade_enable = 1,
    .fade_delay_ms = 40,
#endif
};
static const struct adc_platform_data adc_data = {
#ifdef CONFIG_ALL_ADC_CHANNEL_OPEN_ENABLE
    .all_channel_open = 1,
    .mic_channel = LADC_CH_MIC1_P | LADC_CH_MIC3_P,
    .linein_channel = LADC_CH_AUX0,
    .mic_ch_num = 2,
#else	//demon v1.1
    .mic_channel = LADC_CH_MIC1_P,
    .linein_channel = LADC_CH_AUX0,
    .mic_ch_num = 1,
#endif
    .linein_ch_num = 1,
    .isel = 2,
};
static const struct iis_platform_data iis0_data = {
    .channel_in = BIT(0),
    .channel_out = 0,
    .port_sel = IIS_PORTA,
    .data_width = 0,
    .mclk_output = 0,
    .slave_mode = 0,
};
static const struct iis_platform_data iis1_data = {
    .channel_in = BIT(3),
    .channel_out = 0,
    .port_sel = IIS_PORTA,
    .data_width = 0,
    .mclk_output = 0,
    .slave_mode = 0,
};
static void plnk0_port_remap_cb(void)
{
    extern int gpio_plnk_rx_input(u32 gpio, u8 index, u8 data_sel);
    gpio_plnk_rx_input(IO_PORTH_04, 0, 0);
    gpio_direction_output(IO_PORTC_00, 0);
    gpio_output_channle(IO_PORTC_00, CH0_PLNK0_SCLK_OUT);	//SCLK0使用outputchannel0
    JL_IOMAP->CON3 |= BIT(18);
}
static void plnk0_port_unremap_cb(void)
{
    JL_IOMAP->CON3 &= ~BIT(18);
    gpio_clear_output_channle(IO_PORTC_00, CH0_PLNK0_SCLK_OUT);
}
//plnk的时钟和数据引脚都采用重映射的使用例子
static const struct plnk_platform_data plnk0_data = {
    .hw_channel = PLNK_CH_MIC_L,
    .clk_out = 1,
    .port_remap_cb = plnk0_port_remap_cb,
    .port_unremap_cb = plnk0_port_unremap_cb,
    .sample_edge = 0,   //在CLK的下降沿采样左MIC
    .share_data_io = 1, //两个数字MIC共用一个DAT脚
    .high_gain = 1,
    .dc_cancelling_filter = 14,
};
static const struct plnk_platform_data plnk1_data = {
    .hw_channel = PLNK_CH_MIC_R,
    /* .hw_channel = PLNK_CH_MIC_DOUBLE, */
    .clk_out = 1,
    .port_remap_cb = NULL,
    .dc_cancelling_filter = 14,
};
static const struct audio_pf_data audio_pf_d = {
    .adc_pf_data = &adc_data,
    .dac_pf_data = &dac_data,
    .iis0_pf_data = &iis0_data,
    .iis1_pf_data = &iis1_data,
    .plnk0_pf_data = &plnk0_data,
    .plnk1_pf_data = &plnk1_data,
};
static const struct audio_platform_data audio_data = {
    .private_data = (void *) &audio_pf_d,
};


PWM_PLATFORM_DATA_BEGIN(pwm_data0)
    .port	= 0,
    .pwm_ch = PWMCH0_H | PWMCH0_L,//初始化可选多通道,如:PWMCH0_H | PWMCH0_L | PWMCH1_H ... | PWMCH7_H | PWMCH7_L | PWM_TIMER2_OPCH2 | PWM_TIMER3_OPCH3 ,
    .freq   = 1000,//频率
    .duty   = 50,//占空比
    .point_bit = 0,//根据point_bit值调节占空比小数点精度位: 0<freq<=4K,point_bit=2;4K<freq<=40K,point_bit=1; freq>40K,point_bit=0;
PWM_PLATFORM_DATA_END()

PWM_PLATFORM_DATA_BEGIN(pwm_data1)
    .port	= IO_PORTA_03,//选择定时器的TIMER PWM任意IO，pwm_ch加上PWM_TIMER3_OPCH3或PWM_TIMER2_OPCH2有效,只支持2个PWM,占用output_channel2/3，其他外设使用output_channel需留意
    .pwm_ch = PWM_TIMER2_OPCH2,//初始化可选多通道,如:PWMCH0_H | PWMCH0_L | PWMCH1_H ... | PWMCH7_H | PWMCH7_L | PWM_TIMER2_OPCH2 | PWM_TIMER3_OPCH3 ,
    .freq   = 1000,//频率
    .duty   = 50,//占空比
    .point_bit = 0,//根据point_bit值调节占空比小数点精度位: 0<freq<=4K,point_bit=2;4K<freq<=40K,point_bit=1; freq>40K,point_bit=0;
PWM_PLATFORM_DATA_END()

#if TCFG_USB_SLAVE_ENABLE || TCFG_USB_HOST_ENABLE
static const struct otg_dev_data otg_data = {
    .usb_dev_en = 0x01,
#if TCFG_USB_SLAVE_ENABLE
    .slave_online_cnt = 10,
    .slave_offline_cnt = 10,
#endif
#if TCFG_USB_HOST_ENABLE
    .host_online_cnt = 10,
    .host_offline_cnt = 10,
#endif
    .detect_mode = OTG_HOST_MODE | OTG_SLAVE_MODE | OTG_CHARGE_MODE,
    .detect_time_interval = 50,
};
#endif

#if defined CONFIG_BT_ENABLE || defined CONFIG_WIFI_ENABLE
#include "wifi/wifi_connect.h"
const struct wifi_calibration_param wifi_calibration_param = {
    .xosc_l     = 0xb,// 调节左晶振电容
    .xosc_r     = 0xa,// 调节右晶振电容
    .pa_trim_data  ={7, 7, 1, 1, 5, 0, 7},// 根据MP测试生成PA TRIM值
	.mcs_dgain     ={
		64,//11B_1M
	  	64,//11B_2.2M
	  	64,//11B_5.5M
		64,//11B_11M

		64,//11G_6M
		64,//11G_9M
		64,//11G_12M
		64,//11G_18M
		64,//11G_24M
		38,//11G_36M
		34,//11G_48M
		27,//11G_54M

		64,//11N_MCS0
		64,//11N_MCS1
		64,//11N_MCS2
		64,//11N_MCS3
		40,//11N_MCS4
		34,//11N_MCS5
		29,//11N_MCS6
		25,//11N_MCS7
	}
};
#endif

REGISTER_DEVICES(device_table) = {
    { "pwm0",   &pwm_dev_ops,  (void *)&pwm_data0},
    { "pwm1",   &pwm_dev_ops,  (void *)&pwm_data1},

    { "iic0",  &iic_dev_ops, (void *)&sw_iic0_data },
    { "iic1",  &iic_dev_ops, (void *)&hw_iic1_data },

    { "audio", &audio_dev_ops, (void *)&audio_data },

#if TCFG_SD0_ENABLE
    { "sd0",  &sd_dev_ops, (void *)&sd0_data },
#endif

#if TCFG_SD1_ENABLE
    { "sd1",  &sd_dev_ops, (void *)&sd1_data },
#endif

#ifndef CONFIG_SFC_ENABLE
    { "spi0", &spi_dev_ops, (void *)&spi0_data },
    { "spiflash", &spiflash_dev_ops, (void *)&spiflash_data },
#endif
    { "spi1", &spi_dev_ops, (void *)&spi1_data },
    { "spi2", &spi_dev_ops, (void *)&spi2_data },

    {"rtc", &rtc_dev_ops, NULL},

    {"uart0", &uart_dev_ops, (void *)&uart0_data },
    {"uart1", &uart_dev_ops, (void *)&uart1_data },
    {"uart2", &uart_dev_ops, (void *)&uart2_data },

#if TCFG_USB_SLAVE_ENABLE || TCFG_USB_HOST_ENABLE
    { "otg", &usb_dev_ops, (void *)&otg_data},
#endif
#if TCFG_UDISK_ENABLE
    { "udisk0", &mass_storage_ops, NULL },
#endif
};


/************************** LOW POWER config ****************************/
static const struct low_power_param power_param = {
    .config         = TCFG_LOWPOWER_LOWPOWER_SEL,// | SLEEP_HW_EN,          //SLEEP_HW_EN:使能wifi和蓝牙硬件休眠，SLEEP_EN:使能系统休眠
    .btosc_disable  = TCFG_LOWPOWER_BTOSC_DISABLE,         //进入低功耗时BTOSC是否保持
    .vddiom_lev     = TCFG_LOWPOWER_VDDIOM_LEVEL,          //强VDDIO等级,可选：2.2V  2.4V  2.6V  2.8V  3.0V  3.2V  3.4V  3.6V
    .vddiow_lev     = TCFG_LOWPOWER_VDDIOW_LEVEL,          //弱VDDIO等级,可选：2.1V  2.4V  2.8V  3.2V
    .vdc14_dcdc 	= TRUE,	   							   //打开内部1.4VDCDC，关闭则用外部
    .vdc14_lev		= VDC14_VOL_SEL_LEVEL, 				   //VDD1.4V配置
    .sysvdd_lev		= SYSVDD_VOL_SEL_LEVEL,				   //内核、sdram电压配置
	.vlvd_enable	= TRUE,                                //TRUE电压复位使能
	.vlvd_value		= VLVD_SEL_25V,                        //低电压复位电压值
};

/************************** PWR config ****************************/
//#define PORT_VCC33_CTRL_IO		IO_PORTA_03					//VCC33 DCDC控制引脚,该引脚控制DCDC器件输出的3.3V连接芯片HPVDD、VDDIO、VDD33
#define PORT_WAKEUP_IO			IO_PORTB_01					//软关机和休眠唤醒引脚
#define PORT_WAKEUP_NUM			(PORT_WAKEUP_IO/IO_GROUP_NUM)//默认:0-7:GPIOA-GPIOH, 可以指定0-7组

static const struct port_wakeup port0 = {
    .edge       = FALLING_EDGE,                            //唤醒方式选择,可选：上升沿\下降沿
    .attribute  = BLUETOOTH_RESUME,                        //保留参数
    .iomap      = PORT_WAKEUP_IO,                          //唤醒口选择
    .low_power	= POWER_SLEEP_WAKEUP|POWER_OFF_WAKEUP,    //低功耗IO唤醒,不需要写0
};

static const struct long_press lpres_port = {
    .enable 	= FALSE,
    .use_sec4 	= TRUE,										//enable = TRUE , use_sec4: TRUE --> 4 sec , FALSE --> 8 sec
    .edge		= FALLING_EDGE,								//长按方式,可选：FALLING_EDGE /  RISING_EDGE --> 低电平/高电平
    .iomap 		= PORT_WAKEUP_IO,							//长按复位IO和IO唤醒共用一个IO
};

static const struct sub_wakeup sub_wkup = {
    .attribute  = BLUETOOTH_RESUME,
};

static const struct charge_wakeup charge_wkup = {
    .attribute  = BLUETOOTH_RESUME,
};

static const struct wakeup_param wk_param = {
    .port[PORT_WAKEUP_NUM] = &port0,
    .sub = &sub_wkup,
    .charge = &charge_wkup,
    .lpres = &lpres_port,
};


/*进软关机之前默认将IO口都设置成高阻状态，需要保留原来状态的请修改该函数*/
static void board_set_soft_poweroff(void)
{
    u32 IO_PORT;
    JL_PORT_FLASH_TypeDef *gpio[] = {JL_PORTA, JL_PORTB, JL_PORTC, JL_PORTD, JL_PORTE, JL_PORTF, JL_PORTG, JL_PORTH};

    for (u8 p = 0; p < 8; ++p) {
        //flash sdram PD PE PF PG口不能进行配置,由内部完成控制
        if (gpio[p] == JL_PORTD || gpio[p] == JL_PORTE || gpio[p] == JL_PORTF || gpio[p] == JL_PORTG) {
            continue;
        }
        for (u8 i = 0; i < 16; ++i) {
            IO_PORT = IO_PORTA_00 + p * 16 + i;
            gpio_set_pull_up(IO_PORT, 0);
            gpio_set_pull_down(IO_PORT, 0);
            gpio_set_direction(IO_PORT, 1);
            gpio_set_die(IO_PORT, 0);
            gpio_set_dieh(IO_PORT, 0);
            gpio_set_hd(IO_PORT, 0);
            gpio_set_hd1(IO_PORT, 0);
            gpio_latch_en(IO_PORT, 1);
        }
    }
#ifdef PORT_VCC33_CTRL_IO
    gpio_latch_en(PORT_VCC33_CTRL_IO, 0);
    gpio_direction_output(PORT_VCC33_CTRL_IO, 0);
    gpio_set_pull_up(PORT_VCC33_CTRL_IO, 0);
    gpio_set_pull_down(PORT_VCC33_CTRL_IO, 1);
    gpio_set_direction(PORT_VCC33_CTRL_IO, 1);
    gpio_set_die(PORT_VCC33_CTRL_IO, 0);
    gpio_set_dieh(PORT_VCC33_CTRL_IO, 0);
    gpio_latch_en(PORT_VCC33_CTRL_IO, 1);
#endif
}

static void sleep_exit_callback(u32 usec)
{
#ifdef PORT_VCC33_CTRL_IO
    gpio_direction_output(PORT_VCC33_CTRL_IO, 1);
    gpio_set_pull_up(PORT_VCC33_CTRL_IO, 1);
    gpio_set_pull_down(PORT_VCC33_CTRL_IO,0);
#endif
    /* putchar('-'); */
}

static void sleep_enter_callback(u8  step)
{
    /* 此函数禁止添加打印 */
    if (step == 1) {
        dac_power_off();
    } else {
#ifdef PORT_VCC33_CTRL_IO
        gpio_direction_output(PORT_VCC33_CTRL_IO, 0);
        gpio_set_pull_up(PORT_VCC33_CTRL_IO, 0);
        gpio_set_pull_down(PORT_VCC33_CTRL_IO, 1);
        gpio_set_direction(PORT_VCC33_CTRL_IO, 1);
        gpio_set_die(PORT_VCC33_CTRL_IO, 0);
#endif
	}
}

static void board_power_init(void)
{
#ifdef PORT_VCC33_CTRL_IO
    gpio_direction_output(PORT_VCC33_CTRL_IO, 1);
    gpio_set_pull_up(PORT_VCC33_CTRL_IO, 1);
    gpio_set_pull_down(PORT_VCC33_CTRL_IO,0);
#endif

    power_init(&power_param);

    power_set_callback(TCFG_LOWPOWER_LOWPOWER_SEL, sleep_enter_callback, sleep_exit_callback, board_set_soft_poweroff);

    power_keep_state(POWER_KEEP_RESET | POWER_KEEP_DACVDD);

    power_wakeup_init(&wk_param);

    extern void clean_wakeup_source_port(void);
    clean_wakeup_source_port();

    gpio_direction_output(IO_PORTA_08, 1);//打开外部电源
}

#ifdef CONFIG_DEBUG_ENABLE
void debug_uart_init()
{
#if !defined CONFIG_BT_TEST_ENABLE && (defined CONFIG_MP_TX_TEST_ENABLE || defined CONFIG_MP_RX_TEST_ENABLE || CONFIG_VOLUME_TAB_TEST_ENABLE || CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK0)
    uart_init(&uart1_data);
#else
    uart_init(&uart2_data);
#endif
}
#endif

void board_early_init()
{
    dac_early_init(1, 0x3, 1000);
    devices_init();
}

void board_init()
{
    board_power_init();
    adc_init();
    key_driver_init();
#ifdef CONFIG_AUTO_SHUTDOWN_ENABLE
    sys_power_init();
#endif
#ifdef CONFIG_BT_ENABLE
    void cfg_file_parse(void);
    cfg_file_parse();
#endif
}

#endif
