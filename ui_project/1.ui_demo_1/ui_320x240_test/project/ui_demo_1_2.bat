del res\*.sty
copy project.bin  res\JL.sty
copy result.bin  res\menu.res
copy result.str  res\str.res
copy F_ASCII.PIX   res\ascii.res

del  ..\..\..\..\cpu\WL80\tools\ui_res\*
copy ename.h          ..\..\..\..\apps\ui_demo\include\
copy res\ascii.res      ..\..\..\..\cpu\WL80\tools\ui_res\
copy res\menu.res    ..\..\..\..\cpu\WL80\tools\ui_res\
copy res\JL.sty          ..\..\..\..\cpu\WL80\tools\ui_res\
copy res\str.res        ..\..\..\..\cpu\WL80\tools\ui_res\
del  ..\..\..\..\cpu\WL82\tools\ui_res\*
copy ename.h          ..\..\..\..\apps\ui_demo\include\
copy res\ascii.res      ..\..\..\..\cpu\WL82\tools\ui_res\
copy res\menu.res    ..\..\..\..\cpu\WL82\tools\ui_res\
copy res\JL.sty          ..\..\..\..\cpu\WL82\tools\ui_res\
copy res\str.res        ..\..\..\..\cpu\WL82\tools\ui_res\
exit