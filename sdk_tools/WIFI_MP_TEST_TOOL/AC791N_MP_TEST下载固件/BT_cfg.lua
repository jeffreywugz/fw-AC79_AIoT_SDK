bt_output_vbox_view = {};

--[[===================================================================================
============================= 经典蓝牙配置项 ==========================================
====================================================================================]]--
local switch_cfgs = {};
local bluename_view = {};
local bluename_view2 = {};
local bluenames_bin_cfgs = {};
local bluenames_num = 1;

local function bluename_Hide_set(num, view)
	for i = 1, num do
		--view[i].switch:setHide(false);
		view[i].name:setHide(false);
		view[i].input:setHide(false);
	end;
	for i = num+1, 20 do
	    --view[i].switch:setHide(true);
		view[i].name:setHide(true);
		view[i].input:setHide(true);
	end;
end

local bluenames_add = cfg:stButton2("新增蓝牙", "", "dotty", function ()
    bluenames_num = bluenames_num + 1 ;
	if bluenames_num >= 20 then bluenames_num = 20 end;
    bluename_Hide_set(bluenames_num, bluename_view);
	cfg:set(switch_cfgs[bluenames_num], 1);
end);

local bluenames_sub = cfg:stButton2("删除蓝牙", "", "dotty", function ()
    bluenames_num = bluenames_num - 1 ;
	if bluenames_num <= 1 then bluenames_num = 1 end;
    bluename_Hide_set(bluenames_num, bluename_view);
    cfg:set(switch_cfgs[bluenames_num + 1], 0);
end);

local function variation_bluename()
    for i = 1, 1 do
    	local edr_name = cfg:str("经典蓝牙名字", "JL_AC79XX");
		edr_name:setOSize(32); -- 最长 32
		local on = 0;
		if i == 1 then on = 1; end;
		local onc = cfg:i32("蓝牙名字开关" .. i, on);
        onc:setOSize(1);
		--local switchView = cfg:checkBoxView(onc);
		local nameView = cfg:stLabel(edr_name.name);
		local inputView = cfg:inputMaxLenView(edr_name, 32);	
    	local layouts = cfg:hBox{ --[[switchView,--]] nameView, inputView };
    	--print(i);
    	--print(bluenames_num);
    	
		table.insert(switch_cfgs, onc);
		
		if (i > 1) then layouts:setHide(true); end;
    	
    	-- bluenames[#bluenames+1] = c;
    	-- bluename_switchs[#bluename_switchs+1] = onc;
    	bluename_view[#bluename_view+1] = {--[[switch = switchView,--]]  name = nameView, input = inputView };
		bluename_view2[#bluename_view2+1] = layouts;
    	
    	bluenames_bin_cfgs[#bluenames_bin_cfgs+1] = onc;
    	bluenames_bin_cfgs[#bluenames_bin_cfgs+1] = edr_name;
    end
end

variation_bluename();
local edr_mac_addr = cfg:mac("经典蓝牙MAC地址:", {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF});

local edr_rf_power = cfg:i32("经典蓝牙发射功率：", 8);
edr_rf_power:setOSize(1);

-------------显示---------------------

local edr_bluenames_group_view = cfg:vaBox(bluename_view2);

local edr_bluetooth_view = cfg:stGroup("经典蓝牙",
    cfg:vBox{
	    edr_bluenames_group_view;
		--[[cfg:hBox{
		    cfg:stLabel("  "),
            bluenames_add, 
			cfg:stLabel("  "),
	        bluenames_sub,
	        cfg:stSpacer(),
        }; --]]
	    cfg:hBox {
            cfg:stLabel("经典蓝牙MAC地址："),
	        cfg:macAddrView(edr_mac_addr),
            cfg:stSpacer(),
        };
        cfg:hBox {
        	cfg:stLabel(edr_rf_power.name),
        	cfg:ispinView(edr_rf_power, 0, 11, 1),
            cfg:stLabel2("(设置范围: 0 ~ 11)", "help");
            cfg:stSpacer(),
        };
	}
);

----------------输出BIN----------------

local edr_name_output_bin = cfg:group("edr name bin",
	MULT_AREA_CFG.edr_name.id,
	1,
	bluenames_bin_cfgs
);

local edr_mac_addr_output_bin = cfg:group("edr mac addr bin",
    MULT_AREA_CFG.edr_mac_addr.id,
	1,
	{edr_mac_addr}
);

local edr_rf_power_output_bin = cfg:group("edr rf power bin",
    BIN_ONLY_CFG["BT_CFG"].edr_rf_power.id,
	1,
	{edr_rf_power}
);

--[[===================================================================================
============================= 低功耗蓝牙配置项 ========================================
====================================================================================]]--
local ble_name = cfg:str("低功耗蓝牙名字" ,"JL_AC79XX_BLE");
ble_name:setOSize(32); -- 最长 32

local ble_mac_addr = cfg:mac("低功耗蓝牙MAC地址:", {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF});

local ble_rf_power = cfg:i32("低功耗蓝牙发射功率：", 6);
ble_rf_power:setOSize(1);

-------------显示---------------------
local ble_bluetooth_view = cfg:stGroup("低功耗蓝牙",
    cfg:vBox{
        cfg:hBox {
            cfg:stLabel(ble_name.name),
            cfg:inputMaxLenView(ble_name, 32);
            cfg:stSpacer(),
        };   
        cfg:hBox {
            cfg:stLabel("低功耗蓝牙MAC地址："),
        	cfg:macAddrView(ble_mac_addr),
            cfg:stSpacer(),
        };     
        cfg:hBox {
        	cfg:stLabel(ble_rf_power.name),
        	cfg:ispinView(ble_rf_power, 0, 11, 1),
            cfg:stLabel2("(设置范围: 0 ~ 11)", "help");
            cfg:stSpacer(),
        };
	}
);

----------------输出BIN----------------
local ble_name_output_bin = cfg:group("ble name bin",
    MULT_AREA_CFG.ble_name.id,
	1,
	{ble_name}
);

local ble_mac_addr_output_bin = cfg:group("ble mac addr bin",
    MULT_AREA_CFG.ble_mac_addr.id,
	1,
	{ble_mac_addr}
);

local ble_rf_power_output_bin = cfg:group("ble rf power bin",
    BIN_ONLY_CFG["BT_CFG"].ble_rf_power.id,
	1,
	{ble_rf_power}
);
--[[===================================================================================
============================= 没有连接自动关机时间配置 ================================
====================================================================================]]--
local auto_off_time = cfg:i32("没有连接自动关机时间配置: ", 3);
auto_off_time:setOSize(1);

-------------显示---------------------
local auto_off_time_group_view = cfg:hBox {
    cfg:stLabel(auto_off_time.name),
    cfg:ispinView(auto_off_time, 0, 99999, 1),
    cfg:stLabel2("单位: 分钟(为0不打开)", "help"),
    cfg:stSpacer(),
};


----------------输出BIN----------------
local auto_off_time_output_bin = cfg:group("auto off time bin",
    BIN_ONLY_CFG["BT_CFG"].auto_off_time.id,
	1,
	{auto_off_time}
);
--[[===================================================================================
==================================== tws配对码 ========================================
====================================================================================]]--
local tws_device_indicate = cfg:i32("对耳配对码(2字节)", 0x0000);
tws_device_indicate:setOSize(2);
-- 约束
tws_device_indicate:addConstraint(
    function ()
        local warn_str;
        --[[if cfg.lang == "zh" then
            warn_str = "请输入 2 Bytes 对耳配对码";
        else
            warn_str = "Please input 2 Bytes Pair Code";
        end]]--
        warn_str = "请输入 2 Bytes 对耳配对码";
        return (tws_device_indicate.val >= 0 and tws_device_indicate.val <= 0xFFFF)  or warn_str;
    end
);

-------------显示---------------------
local tws_device_indicate_group_view = cfg:hBox {
    cfg:stLabel("配对码：0x"),
    cfg:hexInputView(tws_device_indicate),
    cfg:stLabel2("(2字节)", "help"),
    cfg:stSpacer(),
};


----------------输出BIN----------------
local tws_device_indicate_output_bin = cfg:group("tws device indicate bin",
    BIN_ONLY_CFG["BT_CFG"].tws_device_indicate.id,
	1,
	{tws_device_indicate}
);

local two_cfg_mix_view = cfg:stGroup("功能配置",
    cfg:vBox {
        --auto_off_time_group_view,
		tws_device_indicate_group_view,
    }
);

--[[===================================================================================
==================================== lrc参数配置 ======================================
====================================================================================]]--
local lrc_cfg_list = {
    cdfItemInit();
    lrc_ws_inc      = intSpinCfgItem("lrc_ws_inc",     2, 480, 480, 600, "（lrc窗口步进值，单位：us，设置范围:480 ~ 600, 默认值480）");
	lrc_ws_init     = intSpinCfgItem("lrc_ws_init",    2, 400, 400, 600, "（lrc窗口初始值，单位：us，设置范围:400 ~ 600, 默认值400）");
	bt_osc_ws_inc   = intSpinCfgItem("bt_osc_ws_inc",  2, 480, 480, 600, "（btosc窗口步进值，单位：us，设置范围:480 ~ 600, 默认值480）");
	bt_osc_ws_init  = intSpinCfgItem("bt_osc_ws_init", 2, 140, 140, 480, "（btosc窗口初始值，单位：us，设置范围:140 ~ 480, 默认值140）");
	osc_change_mode = switchCfgItem("osc_change_mode", 1, 1, "（lrc切换使能，默认值enable）");
	layout = cfg:vaBox(allCfgItemViews());
    cfg_items = allCfgItem();
}

-------------显示---------------------
local lrc_cfg_group_view = cfg:stGroup("lrc参数配置",  lrc_cfg_list.layout );

----------------输出BIN----------------
local lrc_cfg_output_group = {};
table_extend(lrc_cfg_output_group, lrc_cfg_list.cfg_items);

local lrc_cfg_output_bin = cfg:group("lrc cfg bin",
        BIN_ONLY_CFG["BT_CFG"].lrc_cfg.id,
        1,
        lrc_cfg_output_group
    );

--[[===================================================================================
===================================== 保存bin和默认bin ================================
=====================================================================================]]--

cfg:addOutputGroups({
    edr_name_output_bin, 
	edr_mac_addr_output_bin, 
	edr_rf_power_output_bin,
	ble_name_output_bin,
	ble_mac_addr_output_bin,
	ble_rf_power_output_bin,
	--auto_off_time_output_bin,
	tws_device_indicate_output_bin,
	lrc_cfg_output_bin, 
});

local bt_cfg_default_table = {};
--insert_list_to_list(bt_cfg_default_table, bluenames_bin_cfgs);
table.insert(bt_cfg_default_table, edr_mac_addr);
table.insert(bt_cfg_default_table, edr_rf_power);
table.insert(bt_cfg_default_table, ble_name);
table.insert(bt_cfg_default_table, ble_mac_addr);
table.insert(bt_cfg_default_table, ble_rf_power);
table.insert(bt_cfg_default_table, auto_off_time);
table.insert(bt_cfg_default_table, tws_device_indicate);
insert_list_to_list(bt_cfg_default_table, lrc_cfg_output_group);

--[[===================================================================================
====================================== 总显示 =========================================
=====================================================================================]]--
local bt_default_button_view = cfg:stButton2(" 蓝牙配置恢复默认值 ", ":/uires/icon_recovery.png", "", reset_to_default(bt_cfg_default_table));

local view = cfg:vBox{
    edr_bluetooth_view,
	ble_bluetooth_view,
	two_cfg_mix_view,
	lrc_cfg_group_view,
}

local bt_output_vbox_view = cfg:vBox{
    cfg:stHScroll(cfg:vBox{view}),
	bt_default_button_view,
};

bluetooth_view = {"蓝牙配置", bt_output_vbox_view, ":/uires/icon_bt_nol.png", ":/uires/icon_bt_selected.png"};

--cfg:setLayout(layout); -- 设置界面
