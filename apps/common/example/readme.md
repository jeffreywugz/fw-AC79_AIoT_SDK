# EXAMPLE例程总览



## CPU

#### GPIO

>* GPIO输入,上下拉配置
>* GPIO输出,强驱配置
>* GPIO中断模式使用
>* 特殊引脚如USB/配置为GPIO使用方法
>* 双重绑定IO配置方法
>* 无互斥高速度要求操作GPIO方法以及需注意问题
>* 常见问题

---
#### TIMER

>* 除去系统已经占用的定时器外,用户可用定时器选择
>* 定时器中断/轮询方法使用
>* 开始/暂停/继续/停止 计时 功能使用
>* 定时器捕获功能使用
>* 常见问题

---

#### RTC

>* RTC软硬件配置方法
>* RTC走时,设置/读取时间
>* 设置闹钟
>* 读取系统运行时间
>* 常见问题

---

#### ADC

>* 芯片引脚配置方法,测量电压范围,精度
>* ADC读取方法
>* 测量VBAT电源电压方法
>* 常见问题

---

#### PWM

>* 芯片引脚配置,频率/占空比范围
>* 普通定时器PWM与正反转PWM选择方法
>* PWM控制运行,暂停,正反转,动态配置频率占空比
>* 常见问题

---

#### SPI

>* 芯片引脚配置,时钟范围,时序控制
>* 软件/硬件SPI选择
>* SPI1外挂FLASH例子使用方法
>* SPI硬件从机使用说明
>* 常见问题

---

#### IIC

>* 芯片引脚配置,时钟范围
>* 软件/硬件IIC选择
>* 外接EEPROM使用说明
>* 常见问题

---

#### [UART](uart/readme.md)

>* 打印口引脚,波特率配置
>* 通信串口引脚,波特率配置
>* 通信串口读写使用说明
>* 常见问题

---

#### INPUT_CHANNEL

>* 哪些硬件模块有可能使用,系统总计个数注意的情况说明

---

#### OUTPUT_CHANNEL

>* 哪些硬件模块有可能使用,系统总计个数注意的情况说明

---

#### PWM_LED

>* 呼吸灯引脚配置
>* 呼吸灯模式配置和使用方法

---

#### PAP推屏接口

>* 芯片引脚配置,时序控制
>* 读写接口使用方法
>* 中断使用方法
>* 常见问题

---

#### EMI推屏接口

>* 芯片引脚配置,时序控制
>* 读写接口使用方法
>* 中断使用方法
>* 常见问题

---

#### USB

>* USB模式配置
>* USB做masstorge主机方法
>* USB做masstorge从机方法
>* USB做UVC从机方法
>* USB做UVC主机方法
>* USB做HID从机方法
>* 常见问题

---

#### IIS

>* 
>* 常见问题

---



## 外设

#### flash

>* 芯片引脚配置,SPI接口配置,时钟范围,单/双/四线模式配置
>* 读,写,擦除,写保护的使用方法,以及实测性能数据
>* flash读取ID,UID, 读写OTP方法
>* 用户可使用的flash地址范围
>* 写保护的配置方法
>* 常见问题

---

#### SD卡

>* 芯片接口选择,引脚配置,单/四线配置,时钟配置,卡插拔检测方法配置
>
>* 常见问题

---

#### camera sensor

>* 芯片引脚配置,
>* 移植新sensor流程
>
>* 常见问题

---

#### dac

>* 芯片引脚配置
>* 常见问题

---

#### mic

>* 芯片引脚配置,单双mic配置
>* 常见问题

---

#### linein

>* 芯片引脚配置
>* 常见问题

---

#### plink

>* 芯片引脚配置
>* 常见问题

---

#### key

>* 芯片引脚配置
>* IO key,IR key,AD key,uart key,触摸 key,旋转编码器的配置方法
>* 常见问题

---

#### FM

>* 芯片引脚配置
>* 常见问题

---



## 系统

#### 文件系统

>* 文件系统接口的使用方法
>
>* 文件系统实测性能数据
>* 常见问题

---

#### 操作系统

>* 操作系统接口的使用方法
>
>* pthread接口的使用方法
>
>* 常见问题

---

#### flash存储系统配置项

>* VM系统配置项的使用方法与限制
>
>* 用户flash空间使用方法
>* 常见问题

---

#### cbuf

>* 循环buf使用方法
>
>* 常见问题

---

#### lbuf

>* 帧buf使用方法
>
>* 常见问题

---

#### log

>* 系统LOG使用方法
>* 开关LOG优化代码体积
>* 关闭系统LOG保留用户LOG方法
>* 网络云串口/USB虚拟串口/SD卡串口使用方法
>
>* 常见问题

---

#### 系统定时器timer

>* 系统定时器接口的使用说明
>
>* 常见问题

---

#### 固件升级

>* SD卡升级方法
>* 写flash升级方法
>* 串口升级方法
>* 无备份/双备份配置方法
>
>* 常见问题

---

#### BenchMark CPU算力测试

>* Dhrystone, LINPACK, Double Precision Whetstones算力测试方法
>
>* 常见问题

---

#### EVENT

>* 系统事件通知与接收处理
>* 事件类型
>* 常见问题

---

#### MATH

>* 常用数学库接口使用,包括FFT,矩阵运算等
>* 常见问题

---



## 音频

#### 录音

>* 单双mic,linein录音方法
>* 录制用户数据方法
>* 录音参数配置
>* 录音到文件的流程方法
>* 常见问题

---

#### 播歌

>* 播放SD卡文件方法
>* 播放网络歌曲方法
>* linein录音直出到dac播放方法
>* 常见问题

---

#### 打断唤醒

>* 
>* 常见问题

---



## 视频

#### 录像

>* 录像参数配置
>* 录像到SD卡文件方法
>* 录像回调出JPEG/YUV方法
>* 常见问题

---

#### 拍照

>* 
>* 常见问题

---



## 蓝牙

#### BLE配网

>* 
>* 常见问题

---

#### 经典蓝牙播歌

>* 
>* 常见问题

---

#### 蓝牙HID

>* 蓝牙做HID从机
>* 常见问题

---

#### 蓝牙发射

>* 蓝牙做HID从机
>* 常见问题

---



## WIFI

#### AP模式

>* AP模式配置方法
>* 常见问题

---

#### STA模式

>* STA模式配置方法
>* 常见问题

---

#### 配网模式

>* 配网模式配置方法
>* 常见问题

---

#### SOCKET

>* socket_api编程服务器与客户端使用方法
>* lwip socket 编程服务器与客户端使用方法
>* 常见问题

---

#### HTTP 客户端

>* 
>* 常见问题

---

#### HTTP OTA升级

>* 
>* 常见问题

---

#### MQTT

>* 
>* 常见问题

---

#### WEB SOCKET

>* 
>* 常见问题

---

#### mbedtls

>* mbedtls用作服务器方法
>* mbedtls用作客户端方法,证书等配置
>* 常见问题

---

#### 云端服务器接入

>* 
>* 常见问题

---



## UI

#### 显示屏驱动

>* 显示屏接口配置
>* 新增显示屏方法
>* 常见问题

---

#### ui

>* UI使用方法
>* 常见问题

---





## 第三方库

#### cJSON

>* cJSON使用方法
>* 常见问题

---



## SDK APP

#### 系统启动流程

>* 介绍SDK启动流程
>* 用户可加入代码环节
>* 常见问题

---

#### [app state_machine](./app_state_machine/readme.md)

>* 如何创建和使用app state_machine
>* 常见问题

---

#### 软关机

>* 软关机配置方法
>* 各种情况的关机功耗
>* 常见问题

---

#### 休眠

>* 系统休眠配置方法
>* 各种情况的休眠功耗
>* 常见问题

---

#### 唤醒

>* 系统唤醒配置方法
>* 常见问题

---

#### 不可屏蔽中断

>* 使用方法
>* 常见问题

---

#### 单核

>* 使用方法
>* 常见问题

---



## 其他

#### isd_config.ini

>* 配置项说明
>* 常见问题

---
