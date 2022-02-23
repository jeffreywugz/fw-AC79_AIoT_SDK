#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fccshowstruct.h"
#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QFileInfoList>
#include <QResizeEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItemModel>
#include <QSerialPort>
#include <QSerialPortInfo>

#define U32 unsigned int
#define U8 unsigned char
#define GENERATOR 0x1021
//#define GENERATOR 0x8005

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //功能区初始化
    ui->btnRead->setVisible(false); //暂时不支持读取
    ui->btnSave->setVisible(false); //展示不支持保存
    ui->btnCOM->setEnabled(false);
    //初始化串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->cbbCOM->addItem(info.portName());
            serial.close();
        }
        else
        {
            ui->cbbCOM->addItem(info.portName()+tr("(被占用)"));
        }
    }

    ui->cbbrate->addItem("1200");
    ui->cbbrate->addItem("2400");
    ui->cbbrate->addItem("4800");
    ui->cbbrate->addItem("9600");
    ui->cbbrate->addItem("14400");
    ui->cbbrate->addItem("19200");
    ui->cbbrate->addItem("38400");
    ui->cbbrate->addItem("56000");
    ui->cbbrate->addItem("57600");
    ui->cbbrate->addItem("115200");
    ui->cbbrate->addItem("128000");
    ui->cbbrate->addItem("256000");
    ui->cbbrate->setCurrentText("115200");

    my_serialport = new QSerialPort();


    this->setWindowTitle(tr("FCC串口工具(发送)") + tr("V1.2"));
    QStandardItemModel * model = new QStandardItemModel(ui->treeView);
    model->setHorizontalHeaderLabels(QStringList()<<tr("结构体名")<<tr("中文名称"));

    //http://www.tuicool.com/articles/ZFBZfm
    rootitem = new QStandardItem(tr("所有结构体"));
    model->appendRow(rootitem);
    //读取指定目录下所有struct
    QDir dir(PATH);
    if(!dir.exists())
    {
        QDir tmp;
        tmp.mkpath(PATH);
    }
    QStringList strlist;
    strlist<<QString("*.struct");
    dir.setNameFilters(strlist);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();
    for(int i=0; i<list.size(); i++)
    {
        if(list[i].suffix() != "struct")
            continue;
        QStandardItem * item = new QStandardItem(list[i].fileName().left(list[i].fileName().length()-7));
        QFile file(list[i].absoluteFilePath());
        if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
        {
            QMessageBox::information(this, tr("系统提示"), list[i].absoluteFilePath() + tr(" 文件打开失败"));
            continue;
        }
        QTextStream in(&file);
        in.setCodec("gbk");
        QString line = in.readLine();
        strlist = line.split("|");
        QString name = strlist[3].split(":")[1];
        rootitem->appendRow(item);
        rootitem->setChild(i, 1, new QStandardItem(name));
        file.close();
    }
    ui->treeView->setModel(model);
    ui->treeView->expandAll();

    ui->formLayout->setLabelAlignment(Qt::AlignRight);
    ui->formLayout->setContentsMargins(10,10,10,10);
    ui->groupBox->setLayout(ui->formLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    ui->gpb1->setGeometry(10,
                          10,
                          this->width()-20,
                          this->height()-20-ui->gpb2->height());
    ui->gpb2->setGeometry(10,
                          10+ui->gpb1->height()+5,
                          this->width()-20,
                          ui->gpb2->height());
    ui->treeView->setGeometry(3,
                              15,
                              (ui->gpb1->width()-20)*0.4,
                              ui->gpb1->height()-20);
    ui->groupBox->setGeometry((ui->gpb1->width()-20)*0.4+10,
                              15,
                              (ui->gpb1->width()-20)*0.6,
                              ui->gpb1->height()-20);
    ui->txtoutput->setGeometry(ui->txtoutput->x(),
                              ui->txtoutput->y(),
                              ui->gpb2->width()-70,
                              ui->txtoutput->height());
    ui->txtinput->setGeometry(ui->txtinput->x(),
                              ui->txtinput->y(),
                              ui->gpb2->width()-70,
                              ui->txtinput->height());
    ui->treeView->setColumnWidth(0, ui->treeView->width()/2-1);
    ui->treeView->setColumnWidth(1, ui->treeView->width()/2-1);
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    if(index.parent().data().toString() == "")
        return ;
    QString ctrl = "";
    if(index.column() == 0)
    {
        ctrl = index.data().toString();
    }
    else
    {
        ctrl = index.sibling(index.row(), 0).data().toString();
    }

    //保存
    QVector<int> v;
    //清除formlayout原来所有widget
    int itemcount = ui->formLayout->count();
    //从后往前遍历
    for(int i=(itemcount-1); i>=0; i--)
    {
        QLayoutItem * item = ui->formLayout->takeAt(i);
        if(item != 0)
        {
            //qDebug() << item->widget()->metaObject()->className();
            if(item->widget()->inherits("QSpinBox"))
            {
                QSpinBox * box = (QSpinBox *)item->widget();
                v.push_front(box->text().toInt());
            }
            else if(item->widget()->inherits("QComboBox"))
            {
                QComboBox * box = (QComboBox *)item->widget();
                v.push_front(box->currentData().toInt());
            }
            else if(item->widget()->inherits("QCheckBox"))
            {
                QCheckBox * box = (QCheckBox *)item->widget();
                int status = 0;
                if(box->checkState()==Qt::Checked)
                    status = 1;
                else if(box->checkState()==Qt::Unchecked)
                    status = 0;
                v.push_front(status);
            }
            ui->formLayout->removeWidget(item->widget());
            delete item->widget();
        }
    }
    if(map.contains(pre_ctrl) == true)
    { //存在
        map[pre_ctrl] = v;
    }
    else
    { //不存在
        map.insert(pre_ctrl, v);
    }

    //重新加载新的
    QString filename = PATH + ctrl + ".struct";
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        QMessageBox::information(this, tr("系统提示"), tr("文件打开失败"));
        return ;
    }
    QTextStream in(&file);
    in.setCodec("gbk");

    QString line = in.readLine();
    line = line.replace("\r", "");
    line = line.replace("\n", "");
    QStringList list = line.split("|");
    if(list.length() < 5)
    {
        QMessageBox::information(this, tr("系统提示"), tr("struct文件格式错误"));
        return ;
    }
    QString _version = list[0].split(":")[1];
    QString _id      = list[1].split(":")[1];
    QString _name    = list[2].split(":")[1];
    QString _uiname  = list[3].split(":")[1];
    QString _info    = list[4].split(":")[1];

    ui->formLayout->addRow(tr("结构体ID:"), new QLabel(_id));
    ui->formLayout->addRow(tr("结构体名称:"), new QLabel(ctrl + "(" +_name+ ")"));
//    ui->formLayout->addRow(tr("版本:"), new QLabel(_version));
    ui->formLayout->addRow(tr("中文名称:"), new QLabel(_uiname));
    ui->formLayout->addRow(tr("备注信息:"), new QLabel(_info));


    int id = 0;
    //渲染formlayout
    while(!in.atEnd())
    {
        line = in.readLine();
        list = line.split("|");
        //qDebug() << line << list.size();
        if(list.size() < 6)
        {
            QMessageBox::information(this, tr("系统提示"), tr("struct 文件格式错误"));
            return ;
        }
        QString _attr = list[0];
        QString _uiname = list[1];
        QString _type = list[2];
        QString _default = list[3];
        QString _config = list[4];
        QString _info = list[5];
        QLabel * lbl = new QLabel(_uiname+"("+_attr+"):");

        int _v = -999999;
        if(map.contains(ctrl) == true)
        {
            _v = map[ctrl][id];
        }

        try{
            if(_type == "int.u8" || _type == "int.u32")
            {
                if(_config.split(",").length() != 2)
                    throw QString( _config + tr(" 格式错误"));
                QString min = _config.split(",")[0];
                QString max = _config.split(",")[1];
                int _min = min.toInt();
                int _max = max.toInt();
                int def = _default.toInt();
                if(def < _min)
                    def = _min;

                QSpinBox * val = new QSpinBox();
                val->setValue(def); //设置默认值
                val->setMinimum(_min);
                val->setMaximum(_max);
                ui->formLayout->addRow(lbl, val);

                //设置
                if(_v != -999999)
                    val->setValue(_v);
            }
            else if(_type == "list.u8" || _type == "list.u32")
            {
                if(_config.split("$").length() <= 0)
                    throw QString( _config + tr(" 格式错误"));
                QStringList ql = _config.split("$");
                QComboBox * val = new QComboBox();
                for(int i=0; i<ql.length(); i++)
                {
                    QStringList ls = ql[i].split(",");
                    if(ls.length() != 2)
                        throw QString(ql[i] + tr(" 格式错误"));
                    int key = ls[0].toInt();
                    QString value = ls[1];
                    val->addItem(value, key);
                }
                ui->formLayout->addRow(lbl, val);

                //设置
                if(_v != -999999)
                {
                    for(int i=0; i<val->count(); i++)
                    {
                        if(val->itemData(i) == _v)
                        {
                            val->setCurrentIndex(i);
                        }
                    }
                }
            }
            else if(_type == "checkbox.u8")
            {
                QCheckBox * val = new QCheckBox();
                ui->formLayout->addRow(lbl, val);
                int def = _default.toInt();
                if(def == 1)
                    val->setChecked(true);//默认值

                //设置
                if(_v == 1)
                    val->setChecked(true);
                else if(_v == 0)
                    val->setChecked(false);
            }
        }
        catch(QString exception){
            QMessageBox::information(this, tr("系统提示"), tr("格式错误，请通过FCCStruct.exe工具生成。\n")+exception);
            return ;
        }

        id ++;
    }
    file.close();
    pre_ctrl = ctrl;

}

void MainWindow::on_btnSave_clicked()
{
    on_treeView_clicked(ui->treeView->currentIndex());
    //按照文件名排序
    //读取指定目录下所有struct
    QDir dir(PATH);
    if(!dir.exists())
    {
        QDir tmp;
        tmp.mkpath(PATH);
    }
    QStringList strlist;
    strlist<<QString("*.struct");
    dir.setNameFilters(strlist);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();
    for(int i=0; i<list.size(); i++)
    {
        if(list[i].suffix() != "struct")
            continue;
        QString st = QString(list[i].fileName().left(list[i].fileName().length()-7));
        if(map.contains(st) == false)
        {
            QMessageBox::information(this, tr("系统提示"), st + tr(" 结构体未初始化设置."));
            return ;
        }
        QFile file(list[i].absoluteFilePath());
        if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
        {
            QMessageBox::information(this, tr("系统提示"), list[i].absoluteFilePath() + tr(" 文件打开失败"));
            return ;
        }
        file.close();
    }

    //保存文件
    QString filename = QFileDialog::getSaveFileName(this, tr("保存文件"), "FCCStruct.bin", tr("所有文件(*.*)"));
    if(filename == "")
    {
        return ;
    }
    //导出BIN文件
    QFile ofile(filename);
    if(!ofile.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("系统提示"), tr("保存文件失败"));
        return ;
    }
    for(int i=0; i<list.size(); i++)
    {
        if(list[i].suffix() != "struct")
            continue;
        QString st = QString(list[i].fileName().left(list[i].fileName().length()-7));
        QFile file(list[i].absoluteFilePath());
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        in.setCodec("gbk");

        qDebug()<<map[st];
        //读取首行
        QString line = in.readLine();
        int index = 0;
        while(!in.atEnd())
        {
            line = in.readLine();
            QStringList strlist = line.split("|");
            if(strlist.length() < 6)
            {
                file.close();
                ofile.close();
                QMessageBox::information(this, tr("系统提示"), list[i].absoluteFilePath() + tr(" 配置文件格式错误"));
                return ;
            }
            QString _attr = strlist[0];
            QString _type = strlist[2];
            QString type = _type.split(".")[1];
//            qDebug() << _attr << _type.split(".")[1];
            int size = 8;
            if(type == "u8")
            {
                U8 tmp = (U8) map[st][index];
                qDebug() << tmp;
                ofile.write((char*)&tmp, sizeof(U8));
            }
            else if(type == "u32")
            {
                U32 tmp = (U32) map[st][index];
                qDebug() << tmp;
                ofile.write((char *)&tmp, sizeof(U32));
            }
            index ++;
        }
        file.close();
    }

    ofile.flush();
    ofile.close();
}

void MainWindow::on_btnCOM_clicked()
{
    //写串口
    on_treeView_clicked(ui->treeView->currentIndex());
    QDir dir(PATH);
    if(!dir.exists()){QDir tmp; tmp.mkpath(PATH);}

    QModelIndex index = ui->treeView->currentIndex();
    QString ctrl = "";
    if(index.column() == 0)
        ctrl = index.data().toString();
    else
        ctrl = index.sibling(index.row(), 0).data().toString();

    if(map.contains(ctrl) == false)
    {
        QMessageBox::information(this, tr("系统提示"), ctrl + tr(" 结构体未初始化设置"));
        return ;
    }
    QFile file(PATH + ctrl + ".struct");
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){
        QMessageBox::information(this, tr("系统提示"), ctrl + tr(" 文件打开失败"));
        return ;
    }
    char buffer[10240] = {0};
    int buffer_len = 0;
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "JL");
    buffer_len = buffer_len + 2;
    //发送
    QTextStream in(&file);
    in.setCodec("gbk");
    QString line = in.readLine();
    QStringList strlist = line.split("|");
    qint16 id = strlist[1].split(":")[1].toInt();
    for(int i=0; i<sizeof(qint16); i++)
    {
        buffer[buffer_len++] = ((char *)(&id))[i];
    }
    qint16 len = 0;
    for(int i=0; i<sizeof(qint16); i++)
    {
        buffer[buffer_len++] = ((char *)(&len))[i];
    }
    int cnt = 0;
    while(!in.atEnd())
    {
        line = in.readLine();
        strlist = line.split("|");
        if(strlist.length() < 6)
        {
            file.close();
            QMessageBox::information(this, tr("系统提示"), ctrl + tr(" 配置文件格式错误"));
            return ;
        }
        QString _type = strlist[2];
        QString type = _type.split(".")[1];
        if(type == "u8")
        {
            U8 tmp = (U8) map[ctrl][cnt];
            buffer[buffer_len++] = tmp;
            len = len + sizeof(U8);
        }
        else if(type == "u32")
        {
            U32 tmp = (U32) map[ctrl][cnt];
            for(int i=0; i<sizeof(U32); i++)
            {
                buffer[buffer_len++] = ((char *)(&tmp))[i];
            }
            len = len + sizeof(U32);
        }
        cnt ++;
    }
    qDebug() << len;
    //填充长度
    for(int i=0; i<sizeof(qint16); i++)
    {
        buffer[4+i] = ((char *)(&len))[i];
    }
    qint16 crc = cal_crc(buffer, buffer_len);
    for(int i=0; i<sizeof(crc); i++)
    {
        buffer[buffer_len++] = ((char *)(&crc))[i];
    }
    QString output = "";
    for(int i=0; i<buffer_len; i++)
    {
        QString tmp;
        output = output + tmp.sprintf("%02X ", (unsigned char)buffer[i]);
    }
    ui->txtoutput->setText(output);
    my_serialport->write(buffer, buffer_len);
}

void MainWindow::my_readuart()
{
    QByteArray request;
    request = my_serialport->readAll();
    if(request != NULL)
    {
        QString input = "";
        for(int i=0; i<request.size(); i++)
        {
            QString tmp;
            input = input + tmp.sprintf("%02X ", (unsigned char)request.data()[i]);
        }
        ui->txtinput->setText(input);
    }
    request.clear();
}

void MainWindow::on_btnOpen_clicked()
{
    if(ui->btnOpen->text() == tr("打开串口(&O)"))
    {
        my_serialport->setPortName(ui->cbbCOM->currentText());
        if(!my_serialport->open(QIODevice::ReadWrite))
        {
            QMessageBox::information(this, tr("系统提示"), tr("打开串口失败"));
            return ;
        }
        my_serialport->setBaudRate(ui->cbbrate->currentText().toInt());
        my_serialport->setDataBits(QSerialPort::Data8);
        my_serialport->setParity(QSerialPort::NoParity);
        my_serialport->setStopBits(QSerialPort::OneStop);
        my_serialport->setFlowControl(QSerialPort::NoFlowControl);
        connect(my_serialport, SIGNAL(readyRead()), this, SLOT(my_readuart()));
        ui->btnOpen->setText(tr("关闭串口(&C)"));
        ui->cbbCOM->setEnabled(false);
        ui->cbbrate->setEnabled(false);
        ui->btnRefresh->setEnabled(false);
        ui->btnCOM->setEnabled(true);
    }
    else if(ui->btnOpen->text() == tr("关闭串口(&C)"))
    {
        my_serialport->clear();
        ui->btnOpen->setText(tr("打开串口(&O)"));
        ui->cbbCOM->setEnabled(true);
        ui->cbbrate->setEnabled(true);
        ui->btnRefresh->setEnabled(true);
        ui->btnCOM->setEnabled(false);
        my_serialport->close();
    }
}

void MainWindow::on_btnRefresh_clicked()
{
    ui->cbbCOM->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->cbbCOM->addItem(info.portName());
            serial.close();
        }
        else
        {
            ui->cbbCOM->addItem(info.portName()+tr("(被占用)"));
        }
    }
}

unsigned int MainWindow::cal_crc(char *buffer, int len)
{
    unsigned char i;
    unsigned int crc = 0;
    while(len-- != 0)
    {
        for(i=0x80; i!=0; i/=2)
        {
            if((crc&0x8000) != 0)
            {
                crc *= 2;
                crc ^= GENERATOR;
            }
            else
            {
                crc *= 2;
            }
            if((*buffer & i) != 0)
                crc ^= GENERATOR;
        }
        buffer ++;
    }
    crc = crc&0xffff; //crc16
    return crc;
}
