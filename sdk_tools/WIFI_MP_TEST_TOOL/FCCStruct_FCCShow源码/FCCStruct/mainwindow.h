#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnstructrefresh_clicked();

    void on_btnstructadd_clicked();

    void on_btnstructdelete_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_btninfosave_clicked();

    void on_pushButton_clicked();

    void on_btninfoadd_clicked();

    void on_btnup_clicked();

    void on_btndown_clicked();

    void on_btninfodelete_clicked();

    void on_btninfoupdate_clicked();

    void on_btnstructsave_clicked();

    void on_txtname_textChanged(const QString &arg1);

private:
    void getDirFileList(QString Path);
    void saveStructFile(QString Path);
    void readStructFile(QString Path);
    void remarkStructFileID(QString Path);

private:
    Ui::MainWindow *ui;

    QStandardItemModel * model;
    QString PATH = ("config\\struct\\");
};

#endif // MAINWINDOW_H
