del res\*.sty
copy project.bin  res\JL.sty
copy result.bin  res\menu.res
copy result.str  res\str.res
copy F_ASCII.PIX   res\ascii.res

del  ..\..\..\..\cpu\WL80\tools\ui_res\*
copy  ..\..\..\GIF\gif.gif    ..\..\..\..\cpu\WL80\tools\ui_res\

del  ..\..\..\..\cpu\WL82\tools\ui_res\*
copy  ..\..\..\GIF\gif.gif    ..\..\..\..\cpu\WL82\tools\ui_res\

exit