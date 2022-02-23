#include "frmstructadd.h"
#include "ui_frmstructadd.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTime>

frmStructAdd::frmStructAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmStructAdd)
{
    ui->setupUi(this);
    QWidget::setTabOrder(ui->lineEdit, ui->pushButton);
}

frmStructAdd::~frmStructAdd()
{
    delete ui;
}

void frmStructAdd::on_lineEdit_textChanged(const QString &str)
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
    ui->lineEdit->setText(key);
}

void frmStructAdd::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();
    QString filename = PATH + str + ".struct";

    QFile file(filename);
    if(file.exists())
    {
        QMessageBox::information(this, tr("系统提示"), tr("该结构体已经存在，请重新输入"));
        return ;
    }
    else
    {
        //创建文件
        bool flag = file.open(QIODevice::WriteOnly | QIODevice::Text);
        if(flag == false)
        {
            QMessageBox::warning(this, tr("系统提示"), tr("创建结构体失败."));
            return ;
        }
        //随机ID
        QTime time;
        time =  QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        qint16 id = qrand() % 32767;
        QTextStream in(&file);
        str = "version:1|id:"+QString::number(id)+"|name:" + str + "|UIname:" + str + "|info:备注信息";
        in<<str;

        in.flush();
        file.close();

        QMessageBox::information(this, tr("系统提示"), tr("创建结构体成功"));
        this->close();
    }
}
