@echo off

..\..\..\UITools\style_table.exe project.bin sidebar.tab 0x2

copy .\project.bin.deal	..\..\..\..\ui_resource\sidebar.sty

copy project.bin.deal      res\prj2.sty
copy result.bin        res\prj2.res
copy result.str         res\prj2.str
copy sidebar.tab     res\prj2.tab


copy res\prj2.sty         ..\..\..\..\cpu\WL80\tools\ui_res\
copy res\prj2.res         ..\..\..\..\cpu\WL80\tools\ui_res\
copy res\prj2.str          ..\..\..\..\cpu\WL80\tools\ui_res\
copy res\prj2.tab         ..\..\..\..\cpu\WL80\tools\ui_res\


copy res\prj2.sty         ..\..\..\..\cpu\WL82\tools\ui_res\
copy res\prj2.res         ..\..\..\..\cpu\WL82\tools\ui_res\
copy res\prj2.str          ..\..\..\..\cpu\WL82\tools\ui_res\
copy res\prj2.tab         ..\..\..\..\cpu\WL82\tools\ui_res\

exit