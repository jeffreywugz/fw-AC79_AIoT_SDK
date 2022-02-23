cd project

copy ui_demo_1_3.bat     copy_file.bat
if not exist ResBuilder.xml copy ..\..\..\UITools\ResBuilder.xml
if exist ..\..\..\UITools\config\ini\option.ini copy ..\..\..\UITools\config\ini\option.ini config\ini\
start ..\..\..\UITools\QtToolBin.exe