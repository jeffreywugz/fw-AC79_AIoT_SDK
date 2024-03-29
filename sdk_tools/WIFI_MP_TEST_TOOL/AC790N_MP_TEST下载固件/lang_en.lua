local lang_en = {
--
	--["BIN文件配置"] = "BIN File Config",
	--["提示音配置"] = "Tone File Config",
-- WIFI配置翻译
    ["WIFI配置"]="WIFI Configurion",
    ["RF测试模式和测试打印串口选择"] = "RF Test Mode and Test Debug IO",
    ["RF测试模式选择"] = "rf test mode",
    ["关闭"] = "OFF",
    ["WIFI非信令测试模式"] = "WIFI Non Signaling Test Mode",
    ["WIFI信令测试STA模式"] = "WIFI Signaling STA Test Mode",
    ["WIFI信令测试AP模式"] = "WIFI Signaling AP Test Mode",
    ["蓝牙DUT测试模式"] = "Bluetooth DUT Test Mode",
    ["蓝牙FCC认证测试模式"] = "Bluetooth FCC Certification Test Mode",
    ["蓝牙BQB认证测试模式"] = "Bluetooth BQB Certification Test Mode",
    ["混合测试模式"] = "Multiple Test Mode",
    ["RF测试串口IO选择"] = "debug io",
    ["WIFI名字"] = "WIFI Name",
    ["WIFI密码"] = "WIFI Password",
    ["RF参数配置"] = "RF Configuration Parameter",
    ["内部晶振参数"] = "Internal OSC Frequency Deviation",
    ["PA校准参数"] = "PA Calibration",
    ["wifi发射功率数字增益"] = "Digital Gain of WIFI Transmit Power",
    ["发射功率模拟增益"] = "Analog Gain of Transmit Power",
    ["强制覆盖VM保存的配置"] = "If Overwrite the Old Configuration in VM",
    [" WIFI配置恢复默认值 "]="WIFI Configuration Reset Defalut Value",
-- 蓝牙配置翻译
    ["蓝牙配置"]="BT Configurion",
    --["基本配置"]="Base Configurion",
    --["蓝牙配置使能开关:"]="BT Configure Enable",
    ["经典蓝牙"]="Bluetooth",
    ["经典蓝牙名字"]="Bluetooth Name",
    --["开关："]="Config Enable: ",
    ["经典蓝牙MAC地址："]="Bluetooth MAC Address:",
    ["经典蓝牙发射功率："]="Digital Gain of Bluetooth Transmit Power",
    ["(设置范围: 0 ~ 11)"]="Range: 0 ~ 11",
    ["低功耗蓝牙"]="BLE",
    ["低功耗蓝牙名字"]="BLE Name",
    ["低功耗蓝牙MAC地址："]="BLE MAC Address:",
    ["低功耗蓝牙发射功率："]="Digital Gain of BLE Transmit Power",
    --["没有连接自动关机时间配置: "]="Auto Shut Down Time If Not Connected",
    --["单位: 分钟(为0不打开)"]="Unit: Minutes(Config 0 Means Not Auto Shut Down)",
    ["功能配置"]="Functional Configuration",
    ["配对码：0x"]="Pincode: 0x",
    ["(2字节)"]="2 Bytes In Hex",
    ["lrc参数配置"]="Lrc Parameter Configuration",
    ["（lrc窗口步进值，单位：us，设置范围:480 ~ 600, 默认值480）"]="Unit: usec, Range: 480 ~ 600, Default Value: 480",
    ["（lrc窗口初始值，单位：us，设置范围:400 ~ 600, 默认值400）"]="Unit: usec, Range: 400 ~ 600, Default Value: 400",
    ["（btosc窗口步进值，单位：us，设置范围:480 ~ 600, 默认值480）"]="Unit: usec, Range: 480 ~ 600, Default Value: 480",
    ["（btosc窗口初始值，单位：us，设置范围:140 ~ 480, 默认值140）"]="Unit: usec, Range: 140 ~ 480, Default Value: 140",
    ["（lrc切换使能，默认值enable）"]="LRC Switchover Enabled, Default Value: Enabled",
    [" 蓝牙配置恢复默认值 "]="BT Configuration Reset Defalut Value",
-- 通话参数配置翻译
	--CALL
    ["通话参数配置"]="Call Configuration",
    ["(MIC增益，0 ~ 31, 步进：1，默认值：31)"] = "Range:0~31,Step:1,Default:31",
    ["(DAC增益，0 ~ 31, 步进：1，默认值：20)"] = "Range:0~31,Step:1,Default:20",
    ["(原音回音追踪等级， 设置范围: 1.0 ~ 5.0，步进：0.1，默认值：1.0)"] = "Range:1.0~5.0,Step:0.1,Default:1.0",
    ["dB (进入回音消除参考值， 设置范围: -90.0 ~ -60.0 dB，步进：0.1，默认值：-70.0 dB)"] = "Range:-90.0~-60.0,Step:0.1,Default:-70.0,Unit:dB",
    ["dB (单端讲话淡入步进，设置范围: 0.1 ~ 5 dB，步进：0.1，默认值：1.3 dB)"] = "Range:0.1~5.0,Step:0.1,Default:1.3,Unit:dB",
    ["dB (单端讲话淡出步进，设置范围: 0.1 ~ 5 dB，步进：0.1，默认值：0.7 dB)"] = "Range:0.1~5.0,Step:0.1,Default:0.7,Unit:dB",
    ["dB (双端讲话淡入步进，设置范围: 0.1 ~ 5 dB，步进：0.1，默认值：1.3 dB)"] = "Range:0.1~5.0,Step:0.1,Default:1.3,Unit:dB",
    ["dB (双端讲话淡出步进，设置范围: 0.1 ~ 5 dB，步进：0.1，默认值：0.7 dB)"] = "Range:0.1~5.0,Step:0.1,Default:0.7,Unit:dB",
    ["dB (单端讲话放大上限，设置范围: 0.0 ~ 24.0 dB，步进：0.1， 默认值：12.0 dB)"] = "Range:0~24.0,Step:0.1,Default:12.0,Unit:dB",
    ["dB (单端讲话放大下限，设置范围: -20.0 ~ 24.0 dB，步进：0.1， 默认值：0.0 dB)"] = "Range:-20.0~24.0,Step:0.1,Default:0,Unit:dB",
    ["dB (单端讲话放大阈值，设置范围: -70.0 ~ -40.0 dB，步进：0.1，默认值：-50.0 dB)"] = "Range:-70.0~-40.0,Step:0.1,Default:-50.0,Unit:dB",
    ["dB (双端讲话放大上限，设置范围: 0.0 ~ 24.0 dB，步进：0.1， 默认值：12.0 dB)"] = "Range:0~24.0,Step:0.1,Default:12.0,Unit:dB",
    ["dB (双端讲话放大下限，设置范围: -20.0 ~ 24.0 dB，步进：0.1， 默认值：0.0 dB)"] = "Range:-20.0~24.0,Step:0.1,Default:0,Unit:dB",
    ["dB (双端讲话放大阈值，设置范围: -70.0 ~ -40.0 dB，步进：0.1，默认值：-40.0 dB)"] = "Range:-70.0~-40.0,Step:0.1,Default:-40.0,Unit:dB",
    ["dB (单端双端讲话阈值，设置范围: -70.0 ~ -40.0 dB，步进：0.1，默认值：-70.0 dB)"] = "Range:-70.0~-40.0,Step:0.1,Default:-70.0,Unit:dB",
    ["(回音前级动态压制,越小越强，设置范围: -5.0 ~ -1.0，步进：0.1，默认值：-3.0)"] = "Range:-5.0~-1.0,Step:0.1,Default:-3.0",
    ["(回音后级静态压制,越大越强，设置范围: 0 ~ 10.0，   步进：0.1，默认值：4.0)"] = "Range:0~10.0,Step:0.1,Default:4.0",
    ["(噪声前级动态压制,越大越强，设置范围: 1 ~ 2.0，步进：0.01，默认值：1.25)"] = "Range:1.00~2.00,Step:0.01,Default:1.25",
    ["(噪声后级静态压制,越小越强，设置范围: 0 ~ 1.0，步进：0.01，默认值：0.09)"] = "Range:0~1.00,Step:0.01,Default:0.09",
    ["(增益最小值控制,设置范围: 0 ~ 1.0，步进：0.1，默认值：0.1)"] = "Range:0~1.0,Step:0.1,Default:0.1",
    ["(降噪强度,设置范围: 0 ~ 6.0，步进：0.1，默认值：1.0)"] = "Range:0~6.0,Step:0.1,Default:1.0",
    [" 通话参数配置恢复默认值 "] = "Call Configuration Reset Defalut Value",
--电源配置翻译
    ["电源配置"] = "Power Configuration",
    ["强VDDIO电压档位"] = "VDDIOM Voltage",
    ["弱VDDIO电压档位"] = "VDDIOW Voltage",
    ["VDC14电压档位"] = "VDC14 Voltage",
    ["SYSVDD内核电压档位"] = "SYSVDD Voltage",
    ["电压档位配置"] = "Power Configuration",
    ["自动关机时间配置"] = "Auto Shutdown Time",
    ["自动关机时间配置: "] = "Auto Shutdown Time: ",
    ["单位: 分钟(为0不打开)"] = "Unit: Minutes(Config 0 Means Not Auto Shut Down)",
    [" 电源配置恢复默认值 "] = "Power Configuration Reset Defalut Value",
--[[
	["mic 参数设置"]="Mic Configuration",
	["电容方案选择"]="capacitance scheme",
	["偏置电压选择:"]="bias voltage",
	["LDO电压选择:"]="LDO voltage",
    ["AEC 参数配置"]="AEC Parameter Configuration",
    ["(DAC 增益，设置范围: 0 ~ 31)"]="Range: 0 ~ 31",
    ["(MIC增益， 设置范围: 0 ~ 31)"]="Range: 0 ~ 31",
    ["(放大上限，设置范围: 0 ~ 2048)"]="Range: 0 ~ 2048",
    ["(放大下限，设置范围: 0 ~ 2048)"]="Range: 0 ~ 2048",
    ["(放大步进，设置范围: 0 ~ 2048)"]="Range: 0 ~ 2048",
    ["(放大阈值，设置范围: 0 ~ 1024)"]="Range: 0 ~ 1024",
    ["(前级压制，设置范围: 0 ~ 35)"]="Range: 0 ~ 35",
    ["(后级压制，设置范围: 0 ~ 10)"]="Range: 0 ~ 10",
    ["(模式)"]=" ",
    ["(上行 EQ 使能)"]=" ",
    ["MIC 类型配置"]="Mic Configuration",
    ["MIC 电容方案选择："]="Mic Capacitance Type: ",
    [" (硅 MIC 要选不省电容模式)"]="(MEMS Mic Type Should Select No Capless Mode)",
    ["MIC 省电容方案偏置电压选择："]="Mic Bias Voltage(Capless Mode Should Configure): ",
    ["MIC LDO 电压选择："]="Mic LDO Voltage: ",
    [" 不省电容模式 "]="No Capless Mode",
    [" 省电容模式 "]="Capless Mode",
-- 状态配置翻译
    ["状态配置"]="Status Sync Configuration",
    ["状态同步配置使能开关："]="Staus Sync Configuration Enable: ",
    ["不同状态下 LED 灯显示效果和提示音设置"]="LED Display & Tone Play In Different Status Configuration",
    ["开始充电：" .. "\t"]      ="Charging:         ",
    ["充电完成：" .. "\t"]      ="Charge Finish:    ",
    ["开机：" .. "\t\t"]        ="Charge Finish:    ",
    ["关机：" .. "\t\t"]        ="Power Off:        ",
    ["低电：" .. "\t\t"]        ="Low Power:        ",
    ["最大音量：" .. "\t"]      ="Max Volume:       ",
    ["来电：" .. "\t\t"]        ="Call In:          ",
    ["去电：" .. "\t\t"]        ="Call Out:         ",
    ["通话中：" .. "\t"]        ="On The Phone:     ",
    ["蓝牙初始化完成："]        ="BT Init OK:       ",
    ["蓝牙连接成功：" .. "\t"]  ="BT Connected:     ",
    ["蓝牙断开连接：" .. "\t"]  ="BT Disconnected:  ",
    ["对耳连接成功：" .. "\t"]  ="TWS Connected:    ",
    ["对耳断开连接：" .. "\t"]  ="TWS Disconnected: ",
    ["无LED灯显示效果"]="Not Change LED Status",
    ["蓝灯和红灯全灭"]="Blue & Red LED OFF",
    ["蓝灯和红灯全亮"]="Blue & Red LED ON",
    ["蓝灯亮"]="Blue LED ON",
    ["蓝灯灭"]="Blue LED OFF",
    ["蓝灯慢闪"]="Blue LED Slow flash",
    ["蓝灯快闪"]="Blue LED Fast Flash",
    ["蓝灯五秒内连续闪烁两下"]="Blue LED Flash 2 Times In 5 Second",
    ["蓝灯五秒内闪烁一下"]="Blue LED Flash 1 Time In 5 Second",
    ["红灯亮"]="Red LED ON",
    ["红灯灭"]="Red LED OFF",
    ["红灯慢闪"]="Red LED Slow flash",
    ["红灯快闪"]="Red LED Fast flash",
    ["红灯五秒内连续闪烁两下"]="Red LED Flash 2 Times In 5 Second",
    ["红灯五秒内闪烁一下"]="Red LED Flash 1 Time In 5 Second",
    ["红灯和蓝灯交替闪烁(快闪)"]="Red & Blue LED Interactive Fast Flash",
    ["红灯和蓝灯交替闪烁(慢闪)"]="Red & Blue LED Interactive Slow Flash",
    ["蓝灯呼吸状态"]="Blue LED Breathe Mode",
    ["红灯呼吸模式"]="Red LED Breathe Mode",
    ["红蓝交替呼吸模式"]="Red & Blue LED Interactive Breathe Mode",
    ["红灯闪烁三下"]="Red LED Flash 3 Times",
    ["蓝灯闪烁三下"]="Blue LED Flash 3 Times",
    ["无提示音"]="Tone NULL",
    ["数字0"]="Num 0 Tone",
    ["数字1"]="Num 1 Tone",
    ["数字2"]="Num 2 Tone",
    ["数字3"]="Num 3 Tone",
    ["数字4"]="Num 4 Tone",
    ["数字5"]="Num 5 Tone",
    ["数字6"]="Num 6 Tone",
    ["数字7"]="Num 7 Tone",
    ["数字8"]="Num 8 Tone",
    ["数字9"]="Num 9 Tone",
    ["蓝牙模式"]="BT Mode Tone",
    ["连接成功"]="BT Connected Tone",
    ["断开连接"]="BT DisConnected Tone",
    ["对耳连接成功"]="TWS Connected Tone",
    ["对耳断开连接"]="TWS DisConnected Tone",
    ["低电量"]="Low Power Tone",
    ["关机"]="Power Off Tone",
    ["开机"]="Power On Tone",
    ["来电"]="Call In Tone",
    ["最大音量"]="Max Volume Tone",
    [" 状态同步配置恢复默认值 "]="Status SYnc Configuration Reset Defalut Value",
--通用配置
    ["通用配置"]="General Configurion",
--按键消息配置
    ["按键消息配置"]="Key Msg Configurion",
    ["按键数量："]="Number Of Key: ",
    ["（1 ~ 10个）"]="Range: 1 ~ 10",
    ["KEY0 按键HOLD消息:"]="KEY0 Hold Msg:          ",
    ["KEY0 按键三击消息:"]="KEY0 Triple Click Msg:  ",
    ["KEY0 按键双击消息:"]="KEY0 Double Click Msg:  ",
    ["KEY0 按键抬按消息:"]="KEY0 Up Msg:            ",
    ["KEY0 按键短按消息:"]="KEY0 Click Msg:         ",
    ["KEY0 按键长按消息:"]="KEY0 Long Click Msg:    ",

    ["KEY1 按键HOLD消息:"]="KEY1 Hold Msg:          ",
    ["KEY1 按键三击消息:"]="KEY1 Triple Click Msg:  ",
    ["KEY1 按键双击消息:"]="KEY1 Double Click Msg:  ",
    ["KEY1 按键抬按消息:"]="KEY1 Up Msg:            ",
    ["KEY1 按键短按消息:"]="KEY1 Click Msg:         ",
    ["KEY1 按键长按消息:"]="KEY1 Long Click Msg:    ",

    ["KEY2 按键HOLD消息:"]="KEY2 Hold Msg:          ",
    ["KEY2 按键三击消息:"]="KEY2 Triple Click Msg:  ",
    ["KEY2 按键双击消息:"]="KEY2 Double Click Msg:  ",
    ["KEY2 按键抬按消息:"]="KEY2 Up Msg:            ",
    ["KEY2 按键短按消息:"]="KEY2 Click Msg:         ",
    ["KEY2 按键长按消息:"]="KEY2 Long Click Msg:    ",

    ["KEY3 按键HOLD消息:"]="KEY3 Hold Msg:          ",
    ["KEY3 按键三击消息:"]="KEY3 Triple Click Msg:  ",
    ["KEY3 按键双击消息:"]="KEY3 Double Click Msg:  ",
    ["KEY3 按键抬按消息:"]="KEY3 Up Msg:            ",
    ["KEY3 按键短按消息:"]="KEY3 Click Msg:         ",
    ["KEY3 按键长按消息:"]="KEY3 Long Click Msg:    ",

    ["KEY4 按键HOLD消息:"]="KEY4 Hold Msg:          ",
    ["KEY4 按键三击消息:"]="KEY4 Triple Click Msg:  ",
    ["KEY4 按键双击消息:"]="KEY4 Double Click Msg:  ",
    ["KEY4 按键抬按消息:"]="KEY4 Up Msg:            ",
    ["KEY4 按键短按消息:"]="KEY4 Click Msg:         ",
    ["KEY4 按键长按消息:"]="KEY4 Long Click Msg:    ",

    ["KEY5 按键HOLD消息:"]="KEY5 Hold Msg:          ",
    ["KEY5 按键三击消息:"]="KEY5 Triple Click Msg:  ",
    ["KEY5 按键双击消息:"]="KEY5 Double Click Msg:  ",
    ["KEY5 按键抬按消息:"]="KEY5 Up Msg:            ",
    ["KEY5 按键短按消息:"]="KEY5 Click Msg:         ",
    ["KEY5 按键长按消息:"]="KEY5 Long Click Msg:    ",

    ["KEY6 按键HOLD消息:"]="KEY6 Hold Msg:          ",
    ["KEY6 按键三击消息:"]="KEY6 Triple Click Msg:  ",
    ["KEY6 按键双击消息:"]="KEY6 Double Click Msg:  ",
    ["KEY6 按键抬按消息:"]="KEY6 Up Msg:            ",
    ["KEY6 按键短按消息:"]="KEY6 Click Msg:         ",
    ["KEY6 按键长按消息:"]="KEY6 Long Click Msg:    ",

    ["KEY7 按键HOLD消息:"]="KEY7 Hold Msg:          ",
    ["KEY7 按键三击消息:"]="KEY7 Triple Click Msg:  ",
    ["KEY7 按键双击消息:"]="KEY7 Double Click Msg:  ",
    ["KEY7 按键抬按消息:"]="KEY7 Up Msg:            ",
    ["KEY7 按键短按消息:"]="KEY7 Click Msg:         ",
    ["KEY7 按键长按消息:"]="KEY7 Long Click Msg:    ",

    ["KEY8 按键HOLD消息:"]="KEY8 Hold Msg:          ",
    ["KEY8 按键三击消息:"]="KEY8 Triple Click Msg:  ",
    ["KEY8 按键双击消息:"]="KEY8 Double Click Msg:  ",
    ["KEY8 按键抬按消息:"]="KEY8 Up Msg:            ",
    ["KEY8 按键短按消息:"]="KEY8 Click Msg:         ",
    ["KEY8 按键长按消息:"]="KEY8 Long Click Msg:    ",

    ["KEY9 按键HOLD消息:"]="KEY9 Hold Msg:          ",
    ["KEY9 按键三击消息:"]="KEY9 Triple Click Msg:  ",
    ["KEY9 按键双击消息:"]="KEY9 Double Click Msg:  ",
    ["KEY9 按键抬按消息:"]="KEY9 Up Msg:            ",
    ["KEY9 按键短按消息:"]="KEY9 Click Msg:         ",
    ["KEY9 按键长按消息:"]="KEY9 Long Click Msg:    ",

    [" 无作用"]="KEY NULL",
    [" 开机"]="Power ON",
    [" 关机"]="Power Off",
    [" 关机保持"]="Power Off Hold",
    [" 音乐暂停/播放"]="Music Play/Pause",
    [" 上一曲"]="Music Previous",
    [" 下一曲"]="Music Next",
    [" 音量加"]="Volume Up",
    [" 音量减"]="Volume Down",
    [" 回拨电话"]="Call Last Number",
    [" 挂断电话"]="Call Hang Up",
    [" 接听电话"]="Call Answer",
    [" 打开SIRI"]="Open SIRI",
    [" 拍照"]="HID Control",
    ["恢复按键消息配置默认值"]="Key Msg Configurion Reset Defalut Value",
--提示音配置
    ["提示音配置"]="Tone Configurion",
--音量配置
    ["音量配置"]="Volume Configurion",
    ["音量配置使能开关："]="Volume Configurion Enable",
    ["系统最大音量："]="System Max Volume Class",
    ["系统默认音量："]="System Defalut Volume Class",
    ["(设置范围: 0~31)"]="Range: 0 ~ 31",
    ["提示音音量："]="Tone Volume Class",
    ["(设置范围: 0~31, 配置为 0 将跟随系统音量)"]="Range: 0 ~ 31, Configure 0 will be same with System Volume Class",
    [" AUDIO 配置恢复默认值 "]="Volume Configurion Reset Defalut Value",
--充电配置
    ["充电配置使能开关："]="Charge Configurion Enable: ",
    ["充电配置"]="Charge Configurion: ",
    [" 配置恢复默认值 "]="Configurion Reset Defalut Value: ",
    ["开机充电使能开关："]="Power On Charge Enable: ",
    ["充电满电压："]="Voltage Of Fully Charged: ",
    ["充电满电流："]="Current Of Fully Charged: ",
    ["充电电流：  "]="Current Of Charging: ",
    ["低电关机电压设置："]="Voltage Class Of Auto Power Off: ",
    ["低电提醒电压设置："]="Voltage Class Of Lower Power Warning: ",
--TONE
    ["恢复默认提示音"]="Reset Default Tone",
    ["保存提示音文件"]="Save Tone File",
    ["sin (正弦波)"]="sin wave",
    ["wtg (低音质)"]="wtg (Low Quality)",
    ["msbc(中音质)"]="msbc (Medium Quality)",
    ["sbc (高音质)"]="sbc (High Quality)",
    ["mty (全音质) 请设置输出采样率和码率"]="mty (Full Quality), Please Configure SR & BR",
--]]
};

cfg:setTranslate("en", lang_en);


