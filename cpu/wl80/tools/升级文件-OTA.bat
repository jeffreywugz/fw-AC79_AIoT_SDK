@echo off

@echo *********************************************************************
@echo 			    AC790N SDK ���������ļ�����
@echo *********************************************************************
@echo %date%

::��ִ��download.bat�����ļ�
call .\download.bat

::�ٸ����ļ�
copy db_update_files_data.bin update-ota.ufw

echo.
echo.
echo �����ļ����ƣ�update-ota.ufw������OTA�����������ɣ�2���Ӻ��Զ��رմ��ڣ�
echo.
choice /t 2 /d y /n >nul 

::pause
exit