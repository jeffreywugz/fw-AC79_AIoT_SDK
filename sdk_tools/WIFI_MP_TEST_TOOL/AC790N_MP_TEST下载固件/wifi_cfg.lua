
local wifi_cfg_bin_list = {};

--[[===================================================================================
============================= RF测试模式和测试打印串口选择 ==========================================
====================================================================================]]--
local rf_test_mode_list =  {
    [0] = "关闭"; 
    [1] = "WIFI非信令测试模式"; 
	[2] = "WIFI信令测试STA模式";
	[3] = "WIFI信令测试AP模式";
	[4] = "蓝牙DUT测试模式"; 
    [5] = "蓝牙FCC认证测试模式"; 
	[6] = "蓝牙BQB认证测试模式";
	[7] = "混合测试模式";
}; -- 带名字的枚举

local rf_test_mode_cfg_list = {
    cdfItemInit();
	rf_test_mode = comboBoxCfgItem("RF测试模式选择", 1, 0, rf_test_mode_list, "");
	debug_io     = strSpinCfgItem("RF测试串口IO选择", 4, "PC00", "");
	layout = cfg:vaBox(allCfgItemViews());
	cfg_items = allCfgItem();
}
-------------显示---------------------
local rf_test_mode_cfg_group_view = cfg:stGroup("RF测试模式和测试打印串口选择", rf_test_mode_cfg_list.layout);

----------------输出BIN----------------
local rf_test_mode_cfg_output_group = {};
table_extend(rf_test_mode_cfg_output_group, rf_test_mode_cfg_list.cfg_items);

local power_cfg_output_bin = cfg:group("rf test mode cfg bin",
    OTHERS_CFG.rf_test_mode_cfg.id,
    1,
    rf_test_mode_cfg_output_group
);
--[[===================================================================================
============================= WIFI名字 ==========================================
====================================================================================]]--
local ssid_list = {
    cdfItemInit();
	ssid = strSpinCfgItem("SSID", 32, "JL_WIFI", "");
	layout = cfg:vaBox(allCfgItemViews());
	cfg_items = allCfgItem();
}
-------------显示---------------------
local ssid_group_view = cfg:stGroup("WIFI名字", ssid_list.layout);

----------------输出BIN----------------
local ssid_output_group = {};
table_extend(ssid_output_group, ssid_list.cfg_items);

local ssid_output_bin = cfg:group("ssid io cfg bin",
    OTHERS_CFG.ssid.id,
    1,
    ssid_output_group
);
--[[===================================================================================
============================= WIFI密码 ==========================================
====================================================================================]]--
local pwd_list = {
    cdfItemInit();
	pwd = strSpinCfgItem("PWD", 64, "12345678", "");
	layout = cfg:vaBox(allCfgItemViews());
	cfg_items = allCfgItem();
}
-------------显示---------------------
local pwd_group_view = cfg:stGroup("WIFI密码", pwd_list.layout);
----------------输出BIN----------------
local pwd_output_group = {};
table_extend(pwd_output_group, pwd_list.cfg_items);

local pwd_output_bin = cfg:group("ssid io cfg bin",
    OTHERS_CFG.pwd.id,
    1,
    pwd_output_group
);
--[[===================================================================================
============================= RF参数配置 ==========================================
====================================================================================]]--												   
local rf_configuration_list = {
    xosc_list = {
	    cdfItemInit();
	    xosc_l = intSpinCfgItem ("xosc_l", 1, 11, 0, 15, "0-15");
		xosc_r = intSpinCfgItem ("xosc_r", 1, 11, 0, 15, "0-15");
		layout = cfg:vaBox(allCfgItemViews());
		cfg_items = allCfgItem();
	};
	
	pa_list = {
	    cdfItemInit();
	    pa0 = intSpinCfgItem ("pa0", 1, 0, 0, 7,  "0-7");
		pa1 = intSpinCfgItem ("pa1", 1, 0, 0, 7,  "0-7");
		pa2 = intSpinCfgItem ("pa2", 1, 0, 0, 7,  "0-7");
		pa3 = intSpinCfgItem ("pa3", 1, 0, 0, 7,  "0-7");
		pa4 = intSpinCfgItem ("pa4", 1, 0, 0, 15, "0-15");
		pa5 = intSpinCfgItem ("pa5", 1, 0, 0, 7,  "0-7");
		pa6 = intSpinCfgItem ("pa6", 1, 0, 0, 7,  "0-7");
		layout = cfg:vaBox(allCfgItemViews());
		cfg_items = allCfgItem();
	};
	
	wifi_transmitting_power_digital_gain_list = {
	    cdfItemInit();
	    b_1M   = intSpinCfgItem ("b_1M",   1, 32, 0, 128, "0-128");
		b_2M   = intSpinCfgItem ("b_2M",   1, 32, 0, 128, "0-128");
		b_5_5M = intSpinCfgItem ("b_5_5M", 1, 32, 0, 128, "0-128");
		b_11M  = intSpinCfgItem ("b_11M",  1, 32, 0, 128, "0-128");
		g_6M   = intSpinCfgItem ("g_6M",   1, 32, 0, 128, "0-128");
		g_9M   = intSpinCfgItem ("g_9M",   1, 32, 0, 128, "0-128");
		g_12M  = intSpinCfgItem ("g_12M",  1, 32, 0, 128, "0-128");
		g_18M  = intSpinCfgItem ("g_18M",  1, 32, 0, 128, "0-128");
		g_24M  = intSpinCfgItem ("g_24M",  1, 32, 0, 128, "0-128");
		g_36M  = intSpinCfgItem ("g_36M",  1, 32, 0, 128, "0-128");
		g_48M  = intSpinCfgItem ("g_48M",  1, 32, 0, 128, "0-128");
		g_54M  = intSpinCfgItem ("g_54M",  1, 32, 0, 128, "0-128");
		n_MCS0 = intSpinCfgItem ("n_MCS0", 1, 32, 0, 128, "0-128");
		n_MCS1 = intSpinCfgItem ("n_MCS1", 1, 32, 0, 128, "0-128");
		n_MCS2 = intSpinCfgItem ("n_MCS2", 1, 32, 0, 128, "0-128");
		n_MCS3 = intSpinCfgItem ("n_MCS3", 1, 32, 0, 128, "0-128");
		n_MCS4 = intSpinCfgItem ("n_MCS4", 1, 32, 0, 128, "0-128");
		n_MCS5 = intSpinCfgItem ("n_MCS5", 1, 32, 0, 128, "0-128");
		n_MCS6 = intSpinCfgItem ("n_MCS6", 1, 32, 0, 128, "0-128");
		n_MCS7 = intSpinCfgItem ("n_MCS7", 1, 32, 0, 128, "0-128");
		layout = cfg:vaBox(allCfgItemViews());
		cfg_items = allCfgItem();
	};
	
	cdfItemInit();
	analog_gain = intSpinCfgItem ("发射功率模拟增益", 1, 6, 0, 6, "0-6");
	
	force_update_vm = switchCfgItem ("强制覆盖VM保存的配置", 1, 0, "");
	layout = cfg:vaBox(allCfgItemViews());
	
	cfg_items = allCfgItem();
};
-------------显示---------------------
rf_configuration_list.layout = cfg:vBox{		
	cfg:stGroup("内部晶振参数", rf_configuration_list.xosc_list.layout), 
	cfg:stGroup("PA校准参数", rf_configuration_list.pa_list.layout), 
	cfg:stGroup("wifi发射功率数字增益",  rf_configuration_list.wifi_transmitting_power_digital_gain_list.layout),
	cfg:vaBox{rf_configuration_list.layout},
};
local  rf_configuration_group_view = cfg:stGroup("RF参数配置",  rf_configuration_list.layout );
----------------输出BIN----------------
local rf_cfg_output_group = {};
table_extend(rf_cfg_output_group, rf_configuration_list.xosc_list.cfg_items);
table_extend(rf_cfg_output_group, rf_configuration_list.pa_list.cfg_items);
table_extend(rf_cfg_output_group, rf_configuration_list.wifi_transmitting_power_digital_gain_list.cfg_items);
table_extend(rf_cfg_output_group, rf_configuration_list.cfg_items);

local rf_cfg_output_bin = cfg:group("rf analog cfg bin",
    OTHERS_CFG.rf_analog_cfg.id,
    1,
    rf_cfg_output_group
);
--[[===================================================================================
===================================== 保存bin和默认bin ================================
=====================================================================================]]--
cfg:addOutputGroups({
    power_cfg_output_bin,
	ssid_output_bin,
	pwd_output_bin,
	rf_cfg_output_bin,
});
local wifi_cfg_default_table = {};
insert_list_to_list(wifi_cfg_default_table, rf_test_mode_cfg_output_group);
insert_list_to_list(wifi_cfg_default_table, ssid_output_group);
insert_list_to_list(wifi_cfg_default_table, pwd_output_group);
insert_list_to_list(wifi_cfg_default_table, rf_cfg_output_group);
--[[===================================================================================
====================================== 总显示 =========================================
=====================================================================================]]--
local wifi_default_button_view = cfg:stButton2(" WIFI配置恢复默认值 ", ":/uires/icon_recovery.png", "", reset_to_default(wifi_cfg_default_table));

local view = cfg:vBox{
	rf_test_mode_cfg_group_view,
	ssid_group_view,
	pwd_group_view,
	rf_configuration_group_view,
};

local wifi_output_vbox_view = cfg:vBox{
    cfg:stHScroll(cfg:vBox{view}),
	wifi_default_button_view,
};

wifi_view = {"WIFI配置", wifi_output_vbox_view, ":/uires/icon_wifi_nol.png", ":/uires/icon_wifi_sel.png"};

--cfg:setLayout(layout); -- 设置界面