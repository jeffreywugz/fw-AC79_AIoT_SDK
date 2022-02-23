@echo off

@echo *********************************************************************
@echo 			                AC791N SDK
@echo *********************************************************************
@echo %date%

cd /d %~dp0


echo %*

set OBJCOPY=llvm-objcopy.exe
set ELFFILE=sdk.elf

%OBJCOPY% -O binary -j .text %ELFFILE% text.bin
%OBJCOPY% -O binary -j .data %ELFFILE% data.bin
%OBJCOPY% -O binary -j .ram0_data  %ELFFILE% ram0_data.bin
%OBJCOPY% -O binary -j .cache_ram_data  %ELFFILE% cache_ram_data.bin



copy /b text.bin+data.bin+ram0_data.bin+cache_ram_data.bin app.bin

REM set KEY_FILE=-key JL_791N-XXXX.key
REM set KEY_FILE=-key1 JL_791N-XXXX.key1 -mkey JL_791N-XXXX.mkey

isd_download.exe isd_config.ini -tonorflash -dev wl82 -boot 0x1c02000 -div1 -wait 300 -uboot uboot.boot -app app.bin cfg_tool.bin -reboot 500 %KEY_FILE%
@REM 常用命令说明
@rem -format vm         // 擦除VM 区域
@rem -format all        // 擦除所有
@rem -reboot 500        // reset chip, valid in JTAG debug

echo %errorlevel%

@REM 删除临时文件
if exist *.mp3 del *.mp3 
if exist *.PIX del *.PIX
if exist *.TAB del *.TAB
if exist *.res del *.res
if exist *.sty del *.sty

@REM 生成固件升级文件
@REM fw_add.exe -noenc -fw jl_isd.fw  -add ota.bin -type 100 -out jl_isd.fw
@REM 添加配置脚本的版本信息到 FW 文件中
@REM fw_add.exe -noenc -fw jl_isd.fw -add script.ver -out jl_isd.fw

@REM ufw_maker.exe -fw_to_ufw jl_isd.fw
@REM copy jl_isd.ufw update.ufw


@REM 生成配置文件升级文件
rem ufw_maker.exe -chip AC630N %ADD_KEY% -output config.ufw -res bt_cfg.cfg

ping /n 2 127.1>null
IF EXIST null del null


del app.bin
del cache_ram_data.bin
del data.bin
del ram0_data.bin
del text.bin
del jl_isd.bin
del jl_isd.fw
del db_update_data.bin

exit
