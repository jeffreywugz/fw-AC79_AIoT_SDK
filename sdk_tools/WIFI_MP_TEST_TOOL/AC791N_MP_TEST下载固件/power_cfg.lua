
power_cfg_output_vbox_view = {};

--[[===================================================================================
============================= 电压档位 ==========================================
====================================================================================]]--
local power_cfg_list = {
    cdfItemInit();
    VDDIOM_Voltage = dbfSpinCfgItem("强VDDIO电压档位", 4, 3.2, 2.2, 3.6, 0.1, "(设置范围: 2.2 ~ 3.6V，步进：0.1，默认值：3.2)");
	VDDIOW_Voltage = dbfSpinCfgItem("弱VDDIO电压档位", 4, 2.1, 2.1, 3.2, 0.1, "(设置范围: 2.1 ~ 3.2V，步进：0.1，默认值：2.1)");
    VDC14_Voltage = dbfSpinCfgItem("VDC14电压档位", 4, 1.4, 1.2, 1.6, 0.05, "(设置范围: 1.2 ~ 1.6V，步进：0.05，默认值：1.40)");
    SYSVDD_Voltage = dbfSpinCfgItem("SYSVDD内核电压档位", 4, 1.26, 0.87, 1.38, 0.03, "(设置范围: 0.87 ~ 1.38V，步进：0.03，默认值：1.26)");
	layout = cfg:vaBox(allCfgItemViews());
    cfg_items = allCfgItem();
}
-------------显示---------------------
local power_cfg_group_view = cfg:stGroup("电压档位配置",  power_cfg_list.layout );

----------------输出BIN----------------
local power_cfg_output_group = {};
table_extend(power_cfg_output_group, power_cfg_list.cfg_items);

local power_cfg_output_bin = cfg:group("power cfg bin",
    OTHERS_CFG.power_cfg.id,
    1,
    power_cfg_output_group
);
--[[===================================================================================
============================= 自动关机时间配置 ================================
====================================================================================]]--
local auto_off_time = cfg:i32("自动关机时间配置: ", 5);
auto_off_time:setOSize(1);

-------------显示---------------------
local auto_off_time_group_view = cfg:stGroup("自动关机时间配置",
    cfg:hBox {
        cfg:stLabel(auto_off_time.name),
        cfg:ispinView(auto_off_time, 0, 255, 1),
        cfg:stLabel2("单位: 分钟(为0不打开)", "help"),
        cfg:stSpacer(),
    }
);


----------------输出BIN----------------
local auto_off_time_output_bin = cfg:group("auto off time bin",
    BIN_ONLY_CFG["BT_CFG"].auto_off_time.id,
	1,
	{auto_off_time}
);
--[[===================================================================================
===================================== 保存bin和默认bin ================================
=====================================================================================]]--

cfg:addOutputGroups({
    power_cfg_output_bin, 
	auto_off_time_output_bin, 
});

local power_cfg_default_table = {};
insert_list_to_list(power_cfg_default_table, power_cfg_output_group);
table.insert(power_cfg_default_table, auto_off_time);

--[[===================================================================================
====================================== 总显示 =========================================
=====================================================================================]]--
local power_cfg_default_button_view = cfg:stButton2(" 电源配置恢复默认值 ", ":/uires/icon_recovery.png", "", reset_to_default(power_cfg_default_table));

local view = cfg:vBox{
    power_cfg_group_view,
	auto_off_time_group_view,
}

local power_cfg_output_vbox_view = cfg:vBox{
    cfg:stHScroll(cfg:vBox{view}),
	power_cfg_default_button_view,
};

power_cfg_view = {"电源配置", power_cfg_output_vbox_view, ":/uires/icon_cell_nol.png", ":/uires/icon_cell_sel.png"};

--cfg:setLayout(layout); -- 设置界面
