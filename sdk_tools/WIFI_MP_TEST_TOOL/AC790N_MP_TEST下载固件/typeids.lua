--[[
Author: your name
Date: 2021-09-07 14:16:21
LastEditTime: 2021-09-07 15:34:53
LastEditors: your name
Description: In User Settings Edit
FilePath: \conf\source\typeids.lua
--]]

LUA_CFG_RESERVED_ID = 0;  --配置工具保留ID

-- 用户自定义类配置项 ID (1 ~ 49), 保留
USER_DEF_CFG = {};

-- 只存VM配置项 ID (50 ~ 99), 保留
VM_CFG = {};

-- 可存于VM, sys_cfg.bin, 和 BTIF 区域配置项ID (100 ~ 127)
MULT_AREA_CFG = {
    edr_name     = {id = 101},
    edr_mac_addr = {id = 102},
	ble_name     = {id = 103},
    ble_mac_addr = {id = 104},
};

-- 只存于sys_cfg.bin区域配置项 ID (512 ~ 700)
-- 硬件资源类键值(513 ~ 600)
HW_CFG = {
	uart =			{id = 513},
	hwi2c =			{id = 514},
	swi2c =			{id = 515},
	hwspi =			{id = 516},
	swspi =			{id = 517},
	sd =			{id = 518},
	usb =			{id = 519},
	lcd =			{id = 520},
	touch =			{id = 521},
	iokey =			{id = 522},
	adkey =			{id = 523},
	audio =			{id = 524},
	video =			{id = 525},
	wifi =			{id = 526},
	nic =			{id = 527},
	led = 			{id = 528},
	power_mang = 	{id = 529},
	irflt =			{id = 530},
	plcnt =			{id = 531},
	pwmled =		{id = 532},
	rdec =			{id = 533},
	charge_store =	{id = 534},   --充电仓
	charge =    	{id = 535},   --充电
	lowpower =      {id = 536},   --低电电压
    mic_type =      {id = 537},  -- mic类型配置
	sys_com_vol =   {id = 538},  -- 系统联合音量
	call_com_vol =  {id = 539},  -- 电话联合音量
    lp_touch_key =  {id = 540},  -- lp touch key配置
    anc_config   =  {id = 610},  -- ANC参数配置
	anc_coeff_config = {id = 611 }, -- ANC 系数配置
};


-- 蓝牙配置类键值(601 ~ 650)
BT_CFG = {
    edr_rf_power = {id = 601},
    tws_device_indicate = {id = 602},
    auto_off_time = {id = 603},
    aec = {id = 604},
    status = {id = 605},
    key_msg = {id = 606},
    lrc_cfg = {id = 607},
    ble_cfg = {id = 608},
    dms = {id = 609},
    aec_dns = {id = 612 },
    dms_dns = {id = 613 },
    dms_flexible = { id = 614 },
	ble_rf_power = { id = 615 },
};

BIN_ONLY_CFG = {
    ["HW_CFG"] = HW_CFG,
    ["BT_CFG"] = BT_CFG,
};

-- 其它配置键值(651 ~ 700)
OTHERS_CFG = {
	rf_test_mode_cfg	= {id = 651},
	power_cfg			= {id = 652},
	ssid				= {id = 653},
	pwd					= {id = 654},
	rf_analog_cfg		= {id = 655},
};





