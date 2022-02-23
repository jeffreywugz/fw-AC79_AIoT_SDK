#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frmstructadd.h"
#include "frmstructconfig.h"
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QTextStream>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //读取当前目录下config\\struct文件
    this->setWindowTitle("FCC串口工具(配置) V1.1");
    this->setFixedSize(this->width(), this->height());
    ui->txtid->setEnabled(false);

    //初始化tableview
    model = new QStandardItemModel();

    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    ui->tableView->setModel(model);

    getDirFileList(PATH);
}

void MainWindow::getDirFileList(QString Path)
{
    QDir dir(Path);
    if(!dir.exists())
    {
        QDir dd;
        dd.mkpath(Path); //如果文件不存在，那么进行创建
    }
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();

    //重新修改各个struct的ID
    remarkStructFileID(Path);

    ui->comboBox->clear();
    for(int i=0; i<list.size(); i++)
    {
        if(list[i].suffix() != "struct") //如果不是struct后缀 那么就进行过滤
            continue;
        ui->comboBox->addItem(list[i].fileName().left(list[i].fileName().length()-7));
    }
}

void MainWindow::saveStructFile(QString Path)
{
    QString str = ui->comboBox->currentText();
    if(str == "")
        return ;
    QString filename = Path + str + ".struct";
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        QMessageBox::information(this, tr("系统提示"), tr("保存该结构体失败"));
        return ;
    }
    QTextStream in(&file);
    //保存头信息
    QString line;
    line = "version:" + QString::number(ui->txtversion->value()) + "|" +
            "id:" + QString::number(ui->txtid->value()) + "|" +
            "name:" + ui->txtname->text() + "|" +
            "uiname:" + ui->txtuiname->text() + "|" +
            "info:" + ui->txtinfo->text() + "\n";
    in<<line;

    //保存主体, 遍历tableview
    int rows = model->rowCount();
    int columns = model->columnCount();
    qDebug() << rows << columns;
    QString tt;
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<columns; j++)
        {
            QString str = model->data(model->index(i, j)).toString();
            tt = tt + str + "|";
            in<<str<<"|";
        }
        qDebug() << tt;
        tt="";
        in<<"\n";
    }
    in.flush();
    file.close();
}

void MainWindow::readStructFile(QString Path)
{
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnstructrefresh_clicked()
{
    getDirFileList(PATH);
}

void MainWindow::on_btnstructadd_clicked()
{
    frmStructAdd frm;
    frm.setWindowTitle("新建结构体");
    frm.setFixedSize(frm.width(), frm.height());
    frm.exec();
    getDirFileList(PATH);
}

void MainWindow::on_btnstructdelete_clicked()
{
    //删除文件
    QString str = ui->comboBox->currentText();
    if(str == "")
        return ;
    if(QMessageBox::information(this, tr("系统提示"), tr("是否确定删除 ") + str + tr(" 结构体"),
                                QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)
            == QMessageBox::No)
    {
       return ;
    }
    QString filename = PATH + str + ".struct";
    QFile file (filename);
    bool flag = file.remove();
    if(flag == true)
    {
        QMessageBox::information(this, tr("系统提示"), tr("删除结构体成功"));
    }
    getDirFileList(PATH);
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &name)
{
    if(name == "")
        return ;
    //读取文件
    QString filename = PATH + name + ".struct";
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        QMessageBox::information(this, tr("系统提示"), tr("结构体属性打开失败"));
        return ;
    }
    QTextStream in(&file);
    in.setCodec("gbk");

    QString line = in.readLine();
    line = line.replace("\r", "");
    line = line.replace("\n", "");
    QStringList list = line.split("|");
    if(list.size() < 5)
    {
        QMessageBox::information(this, tr("系统提示"), name+tr(" 结构体格式错误, 请删除后重新创建."));
        return ;
    }
    QStringList strlist = list[0].split(":");
    ui->txtversion->setValue(strlist[1].toInt());

    strlist = list[1].split(":");
    ui->txtid->setValue(strlist[1].toInt());

    strlist = list[2].split(":");
    ui->txtname->setText(strlist[1]);

    strlist = list[3].split(":");
    ui->txtuiname->setText(strlist[1]);

    strlist = list[4].split(":");
    ui->txtinfo->setText(strlist[1]);

    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("属性")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("显示名称")));
    model->setHorizontalHeaderItem(2, new QStandardItem(tr("类型")));
    model->setHorizontalHeaderItem(3, new QStandardItem(tr("默认值")));
    model->setHorizontalHeaderItem(4, new QStandardItem(tr("配置约束")));
    model->setHorizontalHeaderItem(5, new QStandardItem(tr("备注")));

    int width = (ui->tableView->width() - 50)/100;
    ui->tableView->setColumnWidth(0, 15 * width);
    ui->tableView->setColumnWidth(1, 15 * width);
    ui->tableView->setColumnWidth(2, 15 * width);
    ui->tableView->setColumnWidth(3, 15 * width);
    ui->tableView->setColumnWidth(4, 25 * width);
    ui->tableView->setColumnWidth(5, 15 * width);

    //填充字段
    int index = 0;
    while(!in.atEnd())
    {
        line = in.readLine();
        list = line.split("|");
        for(int i=0; i<list.size(); i++)
        {
            if(list[i] == "") continue;
            model->setItem(index, i, new QStandardItem(list[i]));
        }
        index ++;
    }
    file.close();
}

void MainWindow::on_btninfosave_clicked()
{
    saveStructFile(PATH);
}

void MainWindow::on_pushButton_clicked()
{
    QDir dir(PATH);
    QDesktopServices::openUrl(QUrl("file:///"+dir.absolutePath(), QUrl::TolerantMode));
}

void MainWindow::on_btninfoadd_clicked()
{
    frmStructConfig frm;
    frm.setFixedSize(frm.width(), frm.height());
    frm.setWindowTitle(tr("属性配置"));
    frm.move(this->x()+(this->width()-frm.width())/2,this->y()+(this->height()-frm.height())/2);
    frm.setFlag(false);
    frm.exec();
    if(frm.getFlag() == false)
    {
        return ;
    }
    //增加
    int rowid = model->rowCount();
    QString _key = frm.getKey();
    QString _uiname = frm.getUIname();
    QString _type = frm.getType();
    QString _default = frm.getDefault();
    QString _config = frm.getConfig();
    QString _info = frm.getInfo();

    model->setItem(rowid, 0, new QStandardItem(_key));
    model->setItem(rowid, 1, new QStandardItem(_uiname));
    model->setItem(rowid, 2, new QStandardItem(_type));
    model->setItem(rowid, 3, new QStandardItem(_default));
    model->setItem(rowid, 4, new QStandardItem(_config));
    model->setItem(rowid, 5, new QStandardItem(_info));

    saveStructFile(PATH);
}

void MainWindow::on_btnup_clicked()
{
    int rowid = ui->tableView->currentIndex().row();
    if(rowid < 1) return ;

    QList<QStandardItem *> list = model->takeRow(rowid);
    model->insertRow(rowid-1, list);

    ui->tableView->selectRow(rowid -1);
    saveStructFile(PATH);
}

void MainWindow::on_btndown_clicked()
{
    int rowid = ui->tableView->currentIndex().row();
    if(rowid < 0) return ;
    if(rowid >= model->rowCount()-1) return ;

    QList<QStandardItem *> list = model->takeRow(rowid);
    model->insertRow(rowid+1, list);

    ui->tableView->selectRow(rowid +1);

    saveStructFile(PATH);
}

void MainWindow::on_btninfodelete_clicked()
{
    int rowid = ui->tableView->currentIndex().row();
    if(rowid < 0) return ;

    model->removeRow(rowid);
    saveStructFile(PATH);
}

void MainWindow::on_btninfoupdate_clicked()
{
    int rowid = ui->tableView->currentIndex().row();
    if(rowid < 0) return ;

    QString _key    = model->data(model->index(rowid, 0)).toString();
    QString _uiname = model->data(model->index(rowid, 1)).toString();
    QString _type   = model->data(model->index(rowid, 2)).toString();
    QString _default= model->data(model->index(rowid, 3)).toString();
    QString _config = model->data(model->index(rowid, 4)).toString();
    QString _info   = model->data(model->index(rowid, 5)).toString();

    frmStructConfig frm;
    frm.setFixedSize(frm.width(), frm.height());
    frm.setWindowTitle(tr("属性配置"));
    frm.move(this->x()+(this->width()-frm.width())/2,this->y()+(this->height()-frm.height())/2);

    frm.setFlag(false);
    frm.setKey(_key);
    frm.setUIname(_uiname);
    frm.setType(_type);
    frm.setDefault(_default);
    frm.setConfig(_config);
    frm.setInfo(_info);

    frm.exec();
    if(frm.getFlag() == false)
    {
        return ;
    }

    _key    = frm.getKey();
    _uiname = frm.getUIname();
    _type   = frm.getType();
    _default= frm.getDefault();
    _config = frm.getConfig();
    _info   = frm.getInfo();

    model->setItem(rowid, 0, new QStandardItem(_key));
    model->setItem(rowid, 1, new QStandardItem(_uiname));
    model->setItem(rowid, 2, new QStandardItem(_type));
    model->setItem(rowid, 3, new QStandardItem(_default));
    model->setItem(rowid, 4, new QStandardItem(_config));
    model->setItem(rowid, 5, new QStandardItem(_info));

    saveStructFile(PATH);
}

void MainWindow::on_btnstructsave_clicked()
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("保存文件"), "FCCStruct.h", tr("所有文件(*.*)"));
    qDebug() << filepath;
    if(filepath == "")
    {
        return ;
    }
    //导出H文件
//    QString filename = "export.h";
    QFile ofile(filepath);
    if(!ofile.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        QMessageBox::information(this, tr("系统提示"), tr("结构体保存失败"));
        return ;
    }
    QTextStream out(&ofile);
    out.setCodec("gbk");

    QDir dir(PATH);
    dir.setFilter(QDir::Files);
    QStringList strlist;
    strlist << QString("*.struct");
    dir.setNameFilters(strlist);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();

    out<<"#ifndef __FCCSTRUCT_H__\n";
    out<<"#define __FCCSTRUCT_H__\n";
    out<<"#include \"typedef.h\"\n";
    out<<"/* FCCStruct ==NOT Modified==*/\n\n";
    for(int i=0; i<list.size(); i++)
    {
        //打印文件名
        out<<"/**\n *    File: "<<list[i].absoluteFilePath()<<"\n";
        QFile file(list[i].absoluteFilePath());
        file.open(QIODevice::ReadOnly| QIODevice::Text);
        QTextStream in(&file);
        in.setCodec("gbk");

        //读取首行
        QString line = in.readLine();
        line = line.replace("\r", "");
        line = line.replace("\n", "");
        QStringList lists = line.split("|");
        if(lists.size() < 5)
        {
            out << "**/\n";
            QMessageBox::information(this, tr("系统提示"), list[i].fileName() + tr(" 结构体格式错误，请删除后再导出"));
            continue;
        }
        QStringList strlist = lists[0].split(":");
        QString version = strlist[1];

        strlist = lists[1].split(":");
        QString id = strlist[1];

        strlist = lists[2].split(":");
        QString name = strlist[1];

        strlist = lists[3].split(":");
        QString uiname = strlist[1];

        strlist = lists[4].split(":");
        QString info = strlist[1];

        out<<" *      ID: " << id << "\n";
        out<<" *    Name: " << name << "\n";
        out<<" *  UIName: " << uiname << "\n";
        out<<" * Version: " << version << "\n";
        out<<" *    Info: " << info << "\n";
        out<<"**/\n";

        out << "#define STRUCT_ID_"+name+" "+id+"\n";
        out << "#pragma pack (1) \n";
        out << "typedef struct "<< list[i].fileName().left(list[i].fileName().length()-7) <<"\n";
        out << "{\n";

        while(!in.atEnd())
        {
            line = in.readLine();
            strlist = line.split("|");
            QString _attr = strlist[0];
            QString _uiname = strlist[1];
            QString _type = strlist[2];
            QString _default = strlist[3];
            QString _config = strlist[4];
            QString _info = strlist[5];
            out << "\t" << _type.split(".")[1] << "\t" << _attr << ";\t" << "// " << _info << "\n";
        }
        out<< "}" << name << ";\n";
        out<< "#pragma pack ()\n";
        out<< "/*******/\n\n";
        file.close();
    }
    out<<"#endif\n";
    ofile.flush();
    ofile.close();
}

void MainWindow::on_txtname_textChanged(const QString &str)
{
    QString ss = str;
    QString key;
    int j = 0;
    for(int i=0; i<ss.length(); i++)
    {
        if(ss[i]>='a' && ss[i]<='z')
        {
            key[j++] = ss[i];
        }
        if(ss[i]>='A' && ss[i]<='Z')
        {
            key[j++] = ss[i];
        }
        if(ss[i] == '_')
        {
            key[j++] = ss[i];
        }
    }
    ui->txtname->setText(key);
}

void MainWindow::remarkStructFileID(QString Path)
{
    QDir dir(Path);
    if(!dir.exists())
    {
        QDir tmp;
        tmp.mkpath(Path);
    }
    QStringList strlist;
    strlist << QString("*.struct");
    dir.setNameFilters(strlist);
    dir.setFilter(QDir::Files| QDir::NoSymLinks);
    dir.setSorting(QDir::Name);
    int count = dir.count();
    if(count <= 0) return ;

    int index = 1;
    QFileInfoList list = dir.entryInfoList();
    for(int i=0; i<list.size(); i++)
    {
        if(list[i].suffix() != "struct")
            continue;
        QString filename = list[i].absoluteFilePath();
        QFile::remove("tmp.tmp");
        QFile::rename(filename, "tmp.tmp");
        QFile tmp("tmp.tmp");
        if(!tmp.open(QIODevice::ReadOnly| QIODevice::Text))
            continue;
        QTextStream in(&tmp);
        in.setCodec("gbk");

        QFile file(filename);
        if(!file.open(QIODevice::WriteOnly| QIODevice::Text))
            continue;
        QTextStream out(&file);
        out.setCodec("gbk");
        QString line = in.readLine();
        strlist = line.split("|");
        if(strlist.size() < 5)
        {
            continue;
        }
        out << strlist[0] + "|"; //version
        out << "id:" << QString::number(index) + "|"; //id
        out << strlist[2] + "|"; //name
        out << strlist[3] + "|"; //uiname
        out << strlist[4] + "|"; //info
        out << "\n";
        index ++;

        while(!in.atEnd())
        {
            out << in.readAll();
        }
        file.flush();
        file.close();

        tmp.remove();
        tmp.close();
    }
}
