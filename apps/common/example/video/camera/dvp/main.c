#include "app_config.h"
#include "device/device.h"//u8
#include "lcd_config.h"//LCD_h
#include "system/includes.h"//late_initcall
#include "yuv_soft_scalling.h"//YUV420p_Soft_Scaling
#include "lcd_te_driver.h"//set_lcd_show_data_mode
#include "get_yuv_data.h"//get_yuv_init

#ifdef USE_CAMERA_DVP_SHOW_TO_LCD_DEMO

int isc_log_en()//如果定义该函数丢帧信息屏蔽
{
    return 0;
}
/******数据流程******************/
/******yuv回调出数据后转为对应屏幕大小YUV交给下一个线程处理***********/
/******这样显示和yuv资源占用差不多才能同步均匀*************************************/
static void get_yuv(u8 *yuv_buf, u32 len, int yuv_in_w, int yuv_in_h)//YUV数据回调线程
{
    /*******将YUV输出数据转成屏幕大小的YUV*********************/
    /*YUV420p_Cut(yuv_buf, yuv_in_w, yuv_in_h, yuv_buf, len, 480, LCD_W+480, 240, LCD_H+240);//裁剪取屏大小数据*/
    YUV420p_Soft_Scaling(yuv_buf, NULL, yuv_in_w, yuv_in_h, LCD_W, LCD_H);
    lcd_show_frame(yuv_buf, LCD_YUV420_DATA_SIZE); //这里输出的是对应屏幕大小的YUV数据 发送到TE线程处理数据
}

static void camera_to_lcd_fps_task(void)
{
    static struct lcd_device *lcd_dev;
    /******ui_lcd_一起初始化数据***********/
    user_ui_lcd_init();

    /*****默认为UI模式*****/
    set_lcd_show_data_mode(camera);

    /******YUV数据回调初始化**********/
    get_yuv_init(get_yuv);
}

static int camera_to_lcd_fps_task_init(void)
{
    puts("camera_to_lcd_fps_task_init \n\n");
    return thread_fork("camera_to_lcd_fps_task", 11, 512, 32, 0, camera_to_lcd_fps_task, NULL);
}
late_initcall(camera_to_lcd_fps_task_init);

#endif
