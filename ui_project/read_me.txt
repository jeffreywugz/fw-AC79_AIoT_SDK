建议 所有ui显示等均以 wifi_story_machine 中显示框架来。通过发消息显示ui的
的方式来显示UI 便于开发，管理以及移植。

0.V1.10.4_ui_template 为提供给客户复制用作工程模板 这样加快了工程的进行
1.V1.10.4_ui_test 为demo_ui工程ui（横屏320*240） 实现了基本控件的显示测试，以及帧率显示测试。
2.V1.10.4_ui_test 为demo_ui工程ui（竖屏240*320） 实现了基本控件的显示测试，以及帧率显示测试。
3.V1.10.4_ui_camera 为wifi_camera工程ui  可以用于视频显示 儿童相机等开发。
4.V1.10.4_ui_story  为wifi_story_machine工程ui  实现了故事机中SD卡播放MP3的UI界面显示。
5.V1.10.4_ui_scan_pen  为wifi_story测试扫描笔时候绘制的简单测试
6.V1.10.4_ui_double1 为双UI工程测试 1,2 两个工程 每个工程生成的ename文件需要合并 并且要注意手动修改避免名字重复

测试方法 1、确认屏幕接线能进行屏幕驱动中测试函数刷颜色
	2、点击UI工程V1.10.4_ui_test中 step2-打开UI资源生成工具 批处理会将ui工程中的文件放到cpu/ui_res中 
	3、下载UI_DEMO工程即可 
	