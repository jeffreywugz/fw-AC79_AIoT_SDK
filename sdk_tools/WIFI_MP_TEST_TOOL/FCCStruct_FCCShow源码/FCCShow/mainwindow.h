#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QVector>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent * e);

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_btnSave_clicked();

    void on_btnCOM_clicked();

    void on_btnOpen_clicked();

    void on_btnRefresh_clicked();

private slots:
    void my_readuart(); //串口接收数据槽函数

private:
    unsigned int cal_crc(char *buffer, int len);

private:
    Ui::MainWindow *ui;
    QString PATH = QString("config\\struct\\");
    QMap<QString, QVector<int> > map;
    QString pre_ctrl;
    QSerialPort * my_serialport;
    QStandardItem * rootitem;
};

#endif // MAINWINDOW_H
