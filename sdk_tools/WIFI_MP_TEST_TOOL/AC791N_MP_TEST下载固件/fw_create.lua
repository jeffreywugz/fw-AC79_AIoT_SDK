package.path = package.path .. ';' .. cfg.dir .. '/?.lua'
package.path = package.path .. ';' .. cfg.projDir .. '/?.lua'

--require("fw_common");
require("lang_en");
cfg:setBinaryFormat("vm"); --配置项与vm格式相同
require("default_cfg"); -- 参数的默认值
--cfg:addKeyInfo("tone_cfg:dir", cfg.projDir .. '/ton_file');                    -- 默认加载提示音 tone.tone 的目录
--cfg:addKeyInfo("bin_cfg:dir", cfg.projDir .. '/');                     -- 默认打开 bin 配置的目录

--open_by_program = "create"; -- 生成编辑

require("common");

-- 把下面这些insert放到模块里面去

fw_create_output_view_tabs = {}
insert_item_to_list(fw_create_output_view_tabs, wifi_view);
insert_item_to_list(fw_create_output_view_tabs, bluetooth_view);
insert_item_to_list(fw_create_output_view_tabs, call_cfg_view);
insert_item_to_list(fw_create_output_view_tabs, power_cfg_view);


-- isdtool 的，已经放置过去了

---------------------bin文件输出-----------------------

cfg:addOutputGroups({lua_cfg_output_bin});

-----------------------设置界面-----------------------
local fw_create_view = cfg:vBox {
	cfg:stTab2(fw_create_output_view_tabs)
}
cfg:setLayout(fw_create_view);

dump_save_file_path = cfg.projDir .. '/' .. 'cfg_tool_state_complete.lua'; -- 生成目录也是加载目录
-------------------- 设置文件输出目录 --------------------
cfg:setOutputPath(function (ty)
	if (ty == 2) then 
	    return cfg.dir .. '/' .. 'cfg_tool.bin';
	elseif (ty == 4) then
		return dump_save_file_path; -- 生成目录也是加载目录
	elseif (ty == 6) then
        return cfg.dir .. '/' .. 'default_cfg.lua';
	end
end);
-------------------- 设置文件输出后的回调函数 --------------------
cfg:setSaveAllHook(function (ty)
    if (ty == 2) then
        if cfg.lang == "zh" then
		    cfg:msgBox("info", "cfg_tool.bin 文件保存在" .. cfg.projDir);
        else
		    cfg:msgBox("info", "cfg_tool.bin file have been saved in" .. cfg.projDir);
        end
		print('you are saving binary file');
	end
end);

--[[-------------------- 设置文件输出目录 --------------------
-- ty 是输出的文件类型，1 是ｃ文件，2是bin文件  3是文件
cfg:setOutputPath(function (ty)
	if (ty == 1) then
		-- 对于c文件，输出到当前配置文件所在目录的 myconfig.c 下
		return c_out_path .. 'cfg_tool.c';
	elseif (ty == 2) then 
		-- 对于bin文件，输出到当前配置文件所在目录的 myconfig.bin 下
		return bin_out_path .. 'cfg_tool.bin';
	elseif (ty == 3) then
		return h_out_path .. 'cfg_tool.h';
	elseif (ty == 4) then
		return dump_save_file_path; -- 生成目录也是加载目录
	elseif (ty == 5) then
		return ver_out_path .. 'script.ver';
    elseif (ty == 6) then
        return default_out_path .. 'default_cfg.lua';
	end
end);



-------------------- 设置文件输出后的回调函数 --------------------
-- ty 是输出的文件类型，1 是ｃ文件，2是bin文件  3是文件
cfg:setSaveAllHook(function (ty)
	if (ty == 1) then 
		print('you are saving c source file');
	elseif (ty == 2) then
        if cfg.lang == "zh" then
		    cfg:msgBox("info", "cfg_tool.bin 文件保存在" .. bin_out_path);
        else
		    cfg:msgBox("info", "cfg_tool.bin file have been saved in" .. bin_out_path);
        end
		print('you are saving binary file');
	elseif (ty == 3) then
		print('you are saving header file');
	elseif (ty == 0) then
		-- 如果全部保存完毕，调用这个分支
	end
end);]]--


