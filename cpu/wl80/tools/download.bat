@echo off

@echo *********************************************************************
@echo 			                AC790N SDK
@echo *********************************************************************
@echo %date%

cd /d %~dp0


echo %*

set OBJDUMP=C:\JL\pi32\bin\llvm-objdump.exe
set OBJCOPY=C:\JL\pi32\bin\llvm-objcopy.exe
set ELFFILE=sdk.elf

REM %OBJDUMP% -D -address-mask=0x1ffffff -print-dbg %ELFFILE% > sdk.lst
%OBJCOPY% -O binary -j .text %ELFFILE% text.bin
%OBJCOPY% -O binary -j .data %ELFFILE% data.bin
%OBJCOPY% -O binary -j .ram0_data  %ELFFILE% ram0_data.bin
%OBJCOPY% -O binary -j .cache_ram_data  %ELFFILE% cache_ram_data.bin

%OBJDUMP% -section-headers -address-mask=0x1ffffff %ELFFILE%
%OBJDUMP% -t %ELFFILE% > symbol_tbl.txt

copy /b text.bin+data.bin+ram0_data.bin+cache_ram_data.bin app.bin

REM set KEY_FILE=-key JL_790N-XXXX.key
REM set KEY_FILE=-key1 JL_790N-XXXX.key1 -mkey JL_790N-XXXX.mkey

REM  ����²���-update_files˵��
REM  1/normal������ͨ���ļ�Ŀ¼�ṹ
REM  2/embedded����ÿ���ļ�����ǰ������һ��ͷ

REM  ֻ����code + res�����ļ�
REM  -update_files normal
REM  ���ɵ��ļ�����Ϊ��db_update_files_data.bin

REM  ֻ����Ԥ������Դ�����ļ�
REM  -update_files embedded_only $(files) ,����$(files)Ϊ��Ҫ��ӵ���Դ�ļ�
REM  ���ɵ��ļ�����Ϊ��db_update_files_data.bin

REM  ����code + res + Ԥ������Դ�����ļ�
REM  -update_files embedded $(files) ,����$(files)Ϊ��Ҫ��ӵ���Դ�ļ�
REM  ���ɵ��ļ�����Ϊ��db_update_files_data.bin

isd_download.exe isd_config.ini -tonorflash -dev wl80 -boot 0x1c02000 -div1 -wait 300 -uboot uboot.boot -app app.bin cfg_tool.bin -res audlogo cfg -reboot 500 %KEY_FILE% -update_files normal

@REM ��������˵��
@rem -format vm         // ����VM ����
@rem -format all        // ��������
@rem -reboot 500        // reset chip, valid in JTAG debug

echo %errorlevel%

@REM ɾ����ʱ�ļ�
if exist *.mp3 del *.mp3 
if exist *.PIX del *.PIX
if exist *.TAB del *.TAB
if exist *.res del *.res
if exist *.sty del *.sty

@REM ���ɹ̼������ļ�
@REM fw_add.exe -noenc -fw jl_isd.fw  -add ota.bin -type 100 -out jl_isd.fw
@REM ������ýű��İ汾��Ϣ�� FW �ļ���
fw_add.exe -noenc -fw jl_isd.fw -add script.ver -out jl_isd.fw

ufw_maker.exe -fw_to_ufw jl_isd.fw
copy jl_isd.ufw update.ufw


@REM ���������ļ������ļ�
rem ufw_maker.exe -chip AC630N %ADD_KEY% -output config.ufw -res bt_cfg.cfg

ping /n 2 127.1>null
IF EXIST null del null


::del app.bin
del cache_ram_data.bin
del data.bin
del ram0_data.bin
del text.bin


::�˳���ǰ������ӷ���ֵ(0),���ܶ��������Ƕ�׵���������
exit /b 0
