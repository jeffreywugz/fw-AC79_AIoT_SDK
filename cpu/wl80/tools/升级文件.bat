@echo off

@echo *********************************************************************
@echo 			    AC790N SDK SD/U�������ļ�����
@echo *********************************************************************
@echo %date%

::��ִ��download.bat�����ļ�
call .\download.bat

::�ٸ����ļ�
copy jl_isd.ufw update.ufw

echo.
echo.
echo �����ļ����ƣ�update.ufw����update.ufw������SD��/U�̵ĸ�Ŀ¼���忨�ϵ缴��������2���Ӻ��Զ��رմ��ڣ�
echo.
choice /t 2 /d y /n >nul 

::pause
exit
