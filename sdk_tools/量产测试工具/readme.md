#   量产测试使用说明

> 本工程展示了量产测试接口使用方法。
>

---

## 适用平台

> 本工程适用以下芯片类型：
>
> 1. AC79系列芯片：AC790N、AC791N
>
> 杰理芯片和评估板的更多信息可在以下地址获取：[链接](https://shop321455197.taobao.com/?spm=a230r.7195193.1997079397.2.2a6d391d3n5udo)
## 依赖性
> * 量产测试依赖文件： [network_mssdp.c](..\..\apps\wifi_camera\wifi\network_mssdp.c) 、 [strm_video_rec.c](..\..\apps\wifi_camera\wifi\strm_video_rec.c) 
> * 量产测试依赖库： [stream_media_server.a](..\..\include_lib\liba\wl80\stream_media_server.a) 、 [wpasupplicant.a](..\..\include_lib\liba\wl80\wpasupplicant.a) 、 [wl_wifi_sta.a](..\..\include_lib\liba\wl80\wl_wifi_sta.a) （或 [wl_wifi_sta_sfc.a](..\..\include_lib\liba\wl80\wl_wifi_sta_sfc.a) ）

## 工程配置说明

在SDK选择[wifi_camera](../../../../apps/wifi_camera/board)主工程文件或者主工程Makefile。

> 量产模式：默认在apps/wifi_camera才具有的量产测试功能。
>
> 1、在 [app_config.h](..\..\apps\wifi_camera\include\app_config.h) 里。
> ①打开CONFIG_MASS_PRODUCTION_ENABLE宏。
> ②配置好量产测试的路由器名称和密码：ROUTER_SSID、ROUTER_PWD。
> ④如果开机上电IO触发进入量产则配置CONFIG_PRODUCTION_IO_PORT和CONFIG_PRODUCTION_IO_STATE（读取IO电平在network_mssdp.c的get_MassProduction函数）。
>
> 2、cbp工程修改库。
> ①打开cbp工程，单击工程，选择右键，build options，linker setting，other linker options，改wl_wifi_ap_sfc.a为wl_wifi_sta_sfc.a（本来是STA的库就不需要该步骤）。
> ②同时添加[stream_media_server.a](..\..\include_lib\liba\wl80\stream_media_server.a) 、 [wpasupplicant.a](..\..\include_lib\liba\wl80\wpasupplicant.a) 库。
>
> 3、PC端。
> ①PC和设备连上同一个路由器（由于PC电脑使用无线wifi连接路由器使得出图会变慢，因此推荐PC电脑端使用网线连接路由器）。
> ②运行： [cFW.exe](cFW-V2.0\cFW.exe) 。
>
> 详情文件：[量产工具V2-使用说明.pdf](cFW-V2.0\量产工具V2-使用说明.pdf) 
>
> **注意：**打开软件，看到图像则表示出图正常，电脑存在多网卡的情况时，有时候可能需要禁掉其他网卡。
## 操作说明

> 1. 使用串口线连接打印
> 2. 编译工程，烧录镜像，复位启动
> 3. 系统启动后，查看相应打印。
>
> JIELI SDK的编译、烧写等操作方式的说明可在以下文档获取：[文档](../../../../doc/stuff/usb updater.pdf)
---

## 常见问题

> * PC端扫描到设备，但是不出图，或者无法找到设备。
>
>   答：由于PC电脑使用无线wifi连接路由器使得出图会变慢，因此推荐PC电脑端使用网线连接路由器；电脑存在多网卡的情况时，有时候可能需要禁掉其他网卡。

## 参考文档

> * N/A