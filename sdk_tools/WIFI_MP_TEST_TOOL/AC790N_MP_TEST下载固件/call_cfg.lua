
call_cfg_output_vbox_view = {}; 

local call_cfg_list = {
    AEC_En1 = cfg:i32("AEC", 1);
    NLP_En1 = cfg:i32("NLP", 1);
    NS_En1  = cfg:i32("NS", 1);
    AGC_En1 = cfg:i32("AGC", 1);
    EQ_En1  = cfg:i32("EQ", 0);
};
call_cfg_list.EQ_En1:setOSize(1);
local en_call_cfg_list = {
    AEC_En1View = cfg:switchView(call_cfg_list.AEC_En1);
    NLP_En1View = cfg:switchView(call_cfg_list.NLP_En1);
    NS_En1View  = cfg:switchView(call_cfg_list.NS_En1);
    AGC_En1View = cfg:switchView(call_cfg_list.AGC_En1);
    EQ_En1View  = cfg:switchView(call_cfg_list.EQ_En1);
};

local Dialog_cfg_list = {

    MIC_list = {
	    cdfItemInit();
        MIC_Gain = intSpinCfgItem("MIC_Gain", 1, 31, 0, 31, "(MIC增益，0 ~ 31, 步进：1，默认值：31)");
		layout = cfg:vaBox(allCfgItemViews());
        cfg_items = allCfgItem();
	};
	
	DAC_list = {
	    cdfItemInit();
        DAC_Gain = intSpinCfgItem("DAC_Gain", 1, 20, 0, 31, "(DAC增益，0 ~ 31, 步进：1，默认值：20)");
		layout = cfg:vaBox(allCfgItemViews());
        cfg_items = allCfgItem();
	};
	
	AEC_list = {
	    cdfItemInit();
        AEC_DT_AGGRESS = dbfSpinCfgItem("AEC_DT_AGGRESS",   4,   1,   1,   5, 0.1,   "(原音回音追踪等级， 设置范围: 1.0 ~ 5.0，步进：0.1，默认值：1.0)");
		AEC_REFENGTHR  = dbfSpinCfgItem("AEC_REFENGTHR", 4, -70, -90, -60, 0.1, "dB (进入回音消除参考值， 设置范围: -90.0 ~ -60.0 dB，步进：0.1，默认值：-70.0 dB)");
		layout = cfg:vaBox(allCfgItemViews());
        cfg_items = allCfgItem();
	};
	
	AGC_list = {
	    cdfItemInit();
        NDT_FADE_IN      = dbfSpinCfgItem("NDT_FADE_IN", 4, 1.3, 0.1,   5, 0.1, "dB (单端讲话淡入步进，设置范围: 0.1 ~ 5 dB，步进：0.1，默认值：1.3 dB)");
        NDT_FADE_OUT     = dbfSpinCfgItem("NDT_FADE_OUT", 4, 0.7, 0.1,   5, 0.1, "dB (单端讲话淡出步进，设置范围: 0.1 ~ 5 dB，步进：0.1，默认值：0.7 dB)");
		
		DT_FADE_IN       = dbfSpinCfgItem("DT_FADE_IN", 4, 1.3, 0.1,   5, 0.1, "dB (双端讲话淡入步进，设置范围: 0.1 ~ 5 dB，步进：0.1，默认值：1.3 dB)");
		DT_FADE_OUT      = dbfSpinCfgItem("DT_FADE_OUT", 4, 0.7, 0.1,   5, 0.1, "dB (双端讲话淡出步进，设置范围: 0.1 ~ 5 dB，步进：0.1，默认值：0.7 dB)");
		
		NDT_MAX_GAIN     = dbfSpinCfgItem("NDT_MAX_GAIN", 4,   12, 0,  24, 0.1, "dB (单端讲话放大上限，设置范围: 0.0 ~ 24.0 dB，步进：0.1， 默认值：12.0 dB)");
		NDT_MIN_GAIN     = dbfSpinCfgItem("NDT_MIN_GAIN", 4,   0, -20,  24, 0.1, "dB (单端讲话放大下限，设置范围: -20.0 ~ 24.0 dB，步进：0.1， 默认值：0.0 dB)");
		NDT_SPEECH_THR   = dbfSpinCfgItem("NDT_SPEECH_THR", 4, -50, -70, -40, 0.1, "dB (单端讲话放大阈值，设置范围: -70.0 ~ -40.0 dB，步进：0.1，默认值：-50.0 dB)");
		
		DT_MAX_GAIN      = dbfSpinCfgItem("DT_MAX_GAIN", 4,  12, 0,  24, 0.1, "dB (双端讲话放大上限，设置范围: 0.0 ~ 24.0 dB，步进：0.1， 默认值：12.0 dB)");
		DT_MIN_GAIN      = dbfSpinCfgItem("DT_MIN_GAIN", 4,   0, -20,  24, 0.1, "dB (双端讲话放大下限，设置范围: -20.0 ~ 24.0 dB，步进：0.1， 默认值：0.0 dB)");
		DT_SPEECH_THR	 = dbfSpinCfgItem("DT_SPEECH_THR", 4, -40, -70, -40, 0.1, "dB (双端讲话放大阈值，设置范围: -70.0 ~ -40.0 dB，步进：0.1，默认值：-40.0 dB)");	
		
		ECHO_PRESENT_THR = dbfSpinCfgItem("ECHO_PRESENT_THR", 4, -70, -70, -40, 0.1, "dB (单端双端讲话阈值，设置范围: -70.0 ~ -40.0 dB，步进：0.1，默认值：-70.0 dB)");	
		layout = cfg:vaBox(allCfgItemViews());
        cfg_items = allCfgItem();
	};
	
	NLP_list = {
	    cdfItemInit();
        NLP_AGGRESS_FACTOR = dbfSpinCfgItem("NLP_AGGRESS_FACTOR", 4, -3, -5, -1, 0.1, "(回音前级动态压制,越小越强，设置范围: -5.0 ~ -1.0，步进：0.1，默认值：-3.0)");
		NLP_MIN_SUPPRESS   = dbfSpinCfgItem("NLP_MIN_SUPPRESS", 4,  4,  0, 10, 0.1, "(回音后级静态压制,越大越强，设置范围: 0 ~ 10.0，   步进：0.1，默认值：4.0)");
		layout = cfg:vaBox(allCfgItemViews());
        cfg_items = allCfgItem();
	};
	
	NS_list = {
	    cdfItemInit();
        ANS_AGGRESS  = dbfSpinCfgItem("ANS_AGGRESS", 4, 1.25, 1, 2, 0.01, "(噪声前级动态压制,越大越强，设置范围: 1 ~ 2.0，步进：0.01，默认值：1.25)");
		ANS_SUPPRESS = dbfSpinCfgItem("ANS_SUPPRESS", 4, 0.09, 0, 1, 0.01, "(噪声后级静态压制,越小越强，设置范围: 0 ~ 1.0，步进：0.01，默认值：0.09)");
		DNS_En1 = switchCfgItem("DNS", 1, 1, "");
		DNS_GAIN_FLOOR = dbfSpinCfgItem("DNS_GAIN_FLOOR", 4, 0.1, 0, 1, 0.1, "(增益最小值控制,设置范围: 0 ~ 1.0，步进：0.1，默认值：0.1)");
		DNS_OVER_DRIVE = dbfSpinCfgItem("DNS_OVER_DRIVE", 4, 1, 0, 6, 0.1, "(降噪强度,设置范围: 0 ~ 6.0，步进：0.1，默认值：1.0)");
		layout = cfg:vaBox(allCfgItemViews());
        cfg_items = allCfgItem();
	};
};

local box_show_list = {
    DAC_box_show = Dialog_show("DAC", "DAC", Dialog_cfg_list.DAC_list.layout);
	ADC_box_show = Dialog_show("ADC", "ADC", Dialog_cfg_list.MIC_list.layout);
	AEC_box_show = Dialog_show("回音消除AEC", "AEC", Dialog_cfg_list.AEC_list.layout);
	NLP_box_show = Dialog_show("回音抑制NLP", "NLP", Dialog_cfg_list.NLP_list.layout);
	NS_box_show  = Dialog_show("降噪NS", "NS", Dialog_cfg_list.NS_list.layout);
	AGC_box_show = Dialog_show("自动增益控制AGC", "AGC", Dialog_cfg_list.AGC_list.layout);
};

local imgItem1 = cfg:stImageItem(cfg.dir .. "/call_cfg.png");

local f = function (item, x, y, w, h, dialog)
	item:addDblClick(x, y, w, h, function (_x, _y)
		dialog:show(true)
	end);
end;

local Image_DblClick_list = {
	{ imgItem1, 706,  53, 56, 50, box_show_list.DAC_box_show},
	{ imgItem1, 716, 245, 56, 50, box_show_list.ADC_box_show},
	{ imgItem1, 600, 248, 89, 48, box_show_list.AEC_box_show},
	{ imgItem1, 484, 247, 89, 48, box_show_list.NLP_box_show},
	{ imgItem1, 386, 248, 75, 48, box_show_list.NS_box_show },
	{ imgItem1, 263, 247, 99, 48, box_show_list.AGC_box_show},
};

for _, v in ipairs(Image_DblClick_list) do
	local item, x, y, w, h, dialog = table.unpack(v);
	f(item, x, y, w, h, dialog);
end


local scene1 = cfg:stScene(910, 360);
scene1:add(imgItem1, 0, 0);
scene1:add(en_call_cfg_list.AEC_En1View, 621, 311);
scene1:add(en_call_cfg_list.NLP_En1View, 505, 311);
scene1:add(en_call_cfg_list.NS_En1View,  400, 311);
scene1:add(en_call_cfg_list.AGC_En1View, 289, 311);
scene1:add(en_call_cfg_list.EQ_En1View,  178, 311);


--[[===================================================================================
============================= 保存bin和默认bin ========================================
====================================================================================]]--

-------------------- 输出bin  ----------------------

local Dialog_cfg_list_output_group = {};

local AEC_Mode = cfg:i32("AEC_Mode", 0);
AEC_Mode:setOSize(1);
cfg:set(AEC_Mode, call_cfg_list.AEC_En1.val + call_cfg_list.NLP_En1.val*2 + call_cfg_list.NS_En1.val*4 + call_cfg_list.AGC_En1.val*16 + Dialog_cfg_list.NS_list.DNS_En1.cfg.val*32);
AEC_Mode:addDeps({call_cfg_list.AEC_En1, call_cfg_list.NLP_En1, call_cfg_list.NS_En1, call_cfg_list.AGC_En1});
AEC_Mode:setEval(function () return call_cfg_list.AEC_En1.val + call_cfg_list.NLP_En1.val*2 + call_cfg_list.NS_En1.val*4 + call_cfg_list.AGC_En1.val*16 + Dialog_cfg_list.NS_list.DNS_En1.cfg.val*32; end);

table_extend(Dialog_cfg_list_output_group, Dialog_cfg_list.MIC_list.cfg_items);
table_extend(Dialog_cfg_list_output_group, Dialog_cfg_list.DAC_list.cfg_items);
table.insert(Dialog_cfg_list_output_group, AEC_Mode);
table.insert(Dialog_cfg_list_output_group, call_cfg_list.EQ_En1);
table_extend(Dialog_cfg_list_output_group, Dialog_cfg_list.AGC_list.cfg_items);
table_extend(Dialog_cfg_list_output_group, Dialog_cfg_list.AEC_list.cfg_items);
table_extend(Dialog_cfg_list_output_group, Dialog_cfg_list.NLP_list.cfg_items);
table.insert(Dialog_cfg_list_output_group, Dialog_cfg_list.NS_list.ANS_AGGRESS.cfg);
table.insert(Dialog_cfg_list_output_group, Dialog_cfg_list.NS_list.ANS_SUPPRESS.cfg);
table.insert(Dialog_cfg_list_output_group, Dialog_cfg_list.NS_list.DNS_GAIN_FLOOR.cfg);
table.insert(Dialog_cfg_list_output_group, Dialog_cfg_list.NS_list.DNS_OVER_DRIVE.cfg);

local Dialog_cfg_output_bin = cfg:group("DIALOG_CFG_BIN",
    BIN_ONLY_CFG["BT_CFG"].aec.id,
    1,
    Dialog_cfg_list_output_group
);    

cfg:addOutputGroups({
    Dialog_cfg_output_bin,
});

------------------- 默认配置 ----------------------

local call_cfg_default_table = {};
local Dialog_cfg_list_default_group = {};
table_extend(call_cfg_default_table, Dialog_cfg_list.MIC_list.cfg_items);
table_extend(call_cfg_default_table, Dialog_cfg_list.DAC_list.cfg_items);
table.insert(call_cfg_default_table, call_cfg_list.AEC_En1);
table.insert(call_cfg_default_table, call_cfg_list.NLP_En1);
table.insert(call_cfg_default_table, call_cfg_list.NS_En1);
table.insert(call_cfg_default_table, call_cfg_list.AGC_En1);
table.insert(call_cfg_default_table, call_cfg_list.EQ_En1);
table_extend(call_cfg_default_table, Dialog_cfg_list.AGC_list.cfg_items);
table_extend(call_cfg_default_table, Dialog_cfg_list.AEC_list.cfg_items);
table_extend(call_cfg_default_table, Dialog_cfg_list.NLP_list.cfg_items);
table_extend(call_cfg_default_table, Dialog_cfg_list.NS_list.cfg_items);
--insert_list_to_list(call_cfg_default_table, Dialog_cfg_list_default_group);

--[[===================================================================================
====================================== 总显示 =========================================
=====================================================================================]]--

local call_cfg_default_button_view = cfg:stButton2(" 通话参数配置恢复默认值 ", ":/uires/icon_recovery.png", "", reset_to_default(call_cfg_default_table));

local scene1_view = cfg:vaBox{ -- 垂直方向
    scene1,
    cfg:stSpacer(),
};

local call_cfg_output_vbox_view = cfg:vBox{
    cfg:stHScroll(cfg:vBox{scene1_view}),
	call_cfg_default_button_view
};
call_cfg_view = {"通话参数配置", call_cfg_output_vbox_view, ":/uires/icon_phone_nol.png", ":/uires/icon_phone_sel.png"};

--cfg:setLayout(call_cfg_output_vbox_view); -- 设置界面
