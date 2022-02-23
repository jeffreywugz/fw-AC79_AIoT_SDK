
function insert_list_to_list(dest_list, src_list)
    for k, item in pairs(src_list) do
        table.insert(dest_list, item);
    end
end

function insert_item_to_list(dest_list, item)
        table.insert(dest_list, item);
end

function depend_item_en_show(dep_cfg, new_cfg)
    new_cfg:addDeps{dep_cfg};
    new_cfg:setDepChangeHook(function() new_cfg:setShow(dep_cfg.val ~= 0); end);
end

function enum_item_hbox_view(enum_item)
    local item_view = cfg:hBox {
        cfg:stLabel(enum_item.name),
        cfg:enumView(enum_item),
        cfg:stSpacer(),
    };

    return item_view;
end

function checkbox_item_hbox_view(enum_item)
    local item_view = cfg:hBox {
        cfg:stLabel(enum_item.name),
        cfg:checkBoxView(enum_item),
        cfg:stSpacer(),
    };

    return item_view;
end

function item_value_set_zero(item)
    item:setEval(function() return 0; end);
end

--十进制转十六进制
function item_val_dec2hex(val)
    return string.format("%#X", val);
end

--十六进制转十进制
function item_val_hex2dec(val)
    return string.format("%d", val);
end

-- 配置项注释类函数
function module_comment_context(module_name)
    local cmt_cfg = cfg:i32(module_name, 0);
    cmt_cfg:setTextOut(
        function (ty)
            if (ty == 3) then
                return "//============= " .. module_name .. NLINE_TABLE[1];
            end
        end
    );
    return cmt_cfg;
end

function item_comment_context(header_str, val_table)
    local ret_str = header_str .. ", ";
    for key, val in pairs(val_table) do
        ret_str = ret_str .. key .. ":" .. val .. ", ";
    end
    ret_str = string.sub(ret_str, 1, -3);
    ret_str = ret_str .. "."
    return ret_str;
end

function item_output_htext(item, mcro_head_str, tab_num, unmap_table, comment_str, nline_num)
    item:setTextOut(
        function (ty)
            if (ty == 3) then
                local item_val_str;
                if (unmap_table == nil) then
                    item_val_str = item.val;
                else 
                    item_val_str = unmap_table[item.val];
                end
                return "#define " .. mcro_head_str .. TAB_TABLE[tab_num] .. item_val_str .. TAB_TABLE[2] .. 
                        "//" .. comment_str .. NLINE_TABLE[nline_num]
            end
        end)

    return item;
end

function item_output_htext_hex(item, mcro_head_str, tab_num, unmap_table, comment_str, nline_num)
    item:setTextOut(
        function (ty)
            if (ty == 3) then
                local item_val_str;
                if (unmap_table == nil) then
                    item_val_str = item.val;
                else 
                    item_val_str = unmap_table[item.val];
                end
                return "#define " .. mcro_head_str .. TAB_TABLE[tab_num] .. item_val_dec2hex(item_val_str) .. TAB_TABLE[1] .. 
                        "//" .. comment_str .. NLINE_TABLE[nline_num]
            end
        end)

    return item;
end


function module_output_comment(comment_cfg, module_name)
    comment_cfg:setTextOut(
        function (ty)
            if (ty == 3) then
			    return COMMMENT_LINE .. NLINE_TABLE[1] .. module_name .. NLINE_TABLE[1] .. COMMMENT_LINE .. NLINE_TABLE[1]
            end
        end)

    return comment_cfg;
end

function reset_to_default(list_of_item)
	return function ()
		if (all_defaults ~= nil) then
			for k, v in pairs(list_of_item) do
				if all_defaults[v.name] ~= nil then
					cfg:set(v, all_defaults[v.name]);
				end
			end
		end
	end
end

function reset_to_default_with_hook(list_of_item, hook)
	return function ()
		if (all_defaults ~= nil) then
			for k, v in pairs(list_of_item) do
				if all_defaults[v.name] ~= nil then
					cfg:set(v, all_defaults[v.name]);
				end
			end
		end
        if (hook ~= nil) then
            hook();
        end
	end
end

lua_cfg_output_bin_tabs = {};

--[[
	所有控件列表
--]]
all_cfg_item_count = 0;  -- 控件个数
all_cfg_item = {};

function cdfItemInit()
	all_cfg_item_count = 0;
	all_cfg_item = {};
end

-- 将控件记录到控件列表中
function cfgItemAdd( cfg_item )
	all_cfg_item[all_cfg_item_count] = cfg_item;
	all_cfg_item_count = all_cfg_item_count + 1;
end

-- 所有控件的view
function allCfgItemViews()
	local allViews = {};
	for idx=0, all_cfg_item_count-1 do
		table.insert(allViews, all_cfg_item[idx].view);
	end
	return allViews;
end

-- 所有控件
function allCfgItem()
	local allViews = {};
	for idx=0, all_cfg_item_count-1 do
		table.insert(allViews, all_cfg_item[idx]);
	end
	return allViews;
end

-- 创建一个ComboBox配置项控件
function comboBoxCfgItem(name, size, default, list, comment)
	local item = {};
	item.map = cfg:enumMap(name, list);
	item.cfg = cfg:enum(name, item.map, default);   
	item.cfg:setOSize(size);
	item.label = cfg:stLabel(item.cfg.name);
	item.view = cfg:hBox {
		item.label,
		cfg:enumView(item.cfg),
		cfg:stLabel2(comment, "help"),
		cfg:stSpacer(),
	};
	cfgItemAdd(item);
	return item;
end

-- 创建一个int SpinBoxF配置项控件
function intSpinCfgItem(name, size, default, min_value, max_value, comment)
	local item = {};
	item.cfg = cfg:i32(name, default);
	item.cfg:setOSize(size);
	item.label = cfg:stLabel(item.cfg.name);
	item.view = cfg:hBox {
		item.label,
		cfg:ispinView(item.cfg, min_value, max_value, 1),
		cfg:stLabel2(comment, "help"),
		cfg:stSpacer(),
	};
	cfgItemAdd(item);
	return item;
end

-- 创建一个str SpinBoxF配置项控件
function strSpinCfgItem(name, size, default, comment)
	local item = {};
	item.cfg = cfg:str(name, default);
	item.cfg:setOSize(size);
	item.label = cfg:stLabel(item.cfg.name);
	item.view = cfg:hBox {
		item.label,
		cfg:inputMaxLenView(item.cfg, size),
		cfg:stLabel(comment),
		cfg:stSpacer(),
	};
	cfgItemAdd(item);
	return item;
end

-- 创建一个float SpinBoxF配置项控件
function dbfSpinCfgItem(name, size, default, min_value, max_value, step, comment)
	local item = {};
	item.cfg = cfg:dbf(name, default);
	item.cfg:setOSize(size);
	item.label = cfg:stLabel(item.cfg.name);
	item.view = cfg:hBox {
		item.label,
		cfg:dspinView(item.cfg, min_value, max_value, step, 2),
		cfg:stLabel2(comment, "help"),
		cfg:stSpacer(),
	};
	cfgItemAdd(item);
	return item;
end

-- 创建一个ON/OFF配置项控件
function switchCfgItem(name, size, default, comment)
	local item = {};
	item.cfg = cfg:i32(name, default);
	item.cfg:setOSize(size);
	item.label = cfg:stLabel(item.cfg.name);
	item.view = cfg:hBox {
		item.label,
		cfg:switchView(item.cfg),
		cfg:stLabel2(comment, "help"),
		cfg:stSpacer(),
	};
	
	cfgItemAdd(item);
	return item;
end

-- 创建一个窗口显示控件
function Dialog_show(box_name, name, layout)
    local item = {};
    item = cfg:stDialog(box_name,
    	cfg:vBox{
    		cfg:stGroup(name, layout),
    	}
    );
	item:show(false);
	return item;
end

-- table延长
function table_extend(dest_list, src_list)
    for k, cfg in pairs(src_list) do
        table.insert(dest_list, src_list[k].cfg);
    end
end

require("typeids");
require("wifi_cfg");
require("BT_cfg");
--require("tone");
require("call_cfg");
require("power_cfg");
--require("com_vol.main");
--require("device_inf")
--[[require("generic_def");

require("board_common.board-main");
require("bluetooth.blue-main");
require("status.status_main");
require("tone.tone");
require("isdtool.isdtool");
require("com_vol.main");
require("extfile.main");
require("anc.anc-main");]]--


lua_cfg_output_bin = cfg:group("CFG_RESERVED_BIN",
    LUA_CFG_RESERVED_ID,
    1,
    lua_cfg_output_bin_tabs
);



-- A. 输出htext
-- B. 输出ctext：无
-- C. 输出bin：无
-- D. 显示
-- E. 默认值
-- F. bindGroup：无
