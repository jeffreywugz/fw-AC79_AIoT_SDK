#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //加载多国语言
    QTranslator translator;
    bool b = translator.load("config/lang.qm");
    if(b){
        a.installTranslator(&translator);
    }

    MainWindow w;
    //居中显示
    w.move((qApp->desktop()->availableGeometry().width() - w.width()) / 2 + qApp->desktop()->availableGeometry().x(),
               (qApp->desktop()->availableGeometry().height() - w.height()) / 2 + qApp->desktop()->availableGeometry().y());
    w.show();

    return a.exec();
}
