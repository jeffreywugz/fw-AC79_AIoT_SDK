#include "frmstructconfig.h"
#include "ui_frmstructconfig.h"
#include <QDebug>
#include <QMessageBox>
#include <QWidget>

frmStructConfig::frmStructConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmStructConfig)
{
    //初始化combox
    ui->setupUi(this);
    ui->cbbtype->addItem("int.u8");
    ui->cbbtype->addItem("int.u32");
    ui->cbbtype->addItem("list.u8");
    ui->cbbtype->addItem("list.u32");
    ui->cbbtype->addItem("checkbox.u8");

    QWidget::setTabOrder(ui->txtkey, ui->txtuiname);
    QWidget::setTabOrder(ui->txtuiname, ui->cbbtype);
    QWidget::setTabOrder(ui->cbbtype, ui->txtdefault);
    QWidget::setTabOrder(ui->txtdefault, ui->txtconfig);
    QWidget::setTabOrder(ui->txtconfig, ui->txtinfo);
    QWidget::setTabOrder(ui->txtinfo, ui->pushButton);
}

frmStructConfig::~frmStructConfig()
{
    delete ui;
}

void frmStructConfig::on_pushButton_clicked()
{
    //判读是否都已经填写
    if(getKey() == "")
    {
        QMessageBox::warning(this, tr("系统提示"), tr("属性一行不能为空"));
        return ;
    }
    if(getUIname() == "")
    {
        QMessageBox::warning(this, tr("系统提示"), tr("属性显示名称一行不能为空"));
        return ;
    }
    if(getType() == "")
    {
        QMessageBox::warning(this, tr("系统提示"), tr("属性类型一行不能为空"));
        return ;
    }
    if(getDefault() == "")
    {
        QMessageBox::warning(this, tr("系统提示"), tr("属性默认值一行不能为空"));
        return ;
    }
    if(getConfig() == "")
    {
        QMessageBox::warning(this, tr("系统提示"), tr("属性配置一行不能为空"));
        return ;
    }
    //TODO: 校验属性配置
    QString type = getType();
    QString config = getConfig();
    if(type == "int.u8" || type == "int.u32") // 1,36
    {
        try{
            if(config.split(",").length()!=2)
                throw QString("...");
            QString min = config.split(",")[0];
            QString max = config.split(",")[1];
            int _min = min.toInt();
            int _max = max.toInt();
            if(_max<_min)
            {
                throw QString("...");
            }
            setConfig(QString("%1,%2").arg(_min).arg(_max));
        }
        catch(...)
        {
            QMessageBox::information(this, tr("系统提示"), tr("int.u8 int.u32 填写格式为-> A,B <-,B不能小于A."));
            return ;
        }
    }
    else if(type == "list.u8" || type == "list.u32")
    {
        try{
            QString out = "";
            if(config.split("$").length() <= 0)
                throw QString(tr("整体格式错误"));
            QStringList list = config.split("$");
            for(int i=0; i<list.length(); i++)
            {
                QStringList ls = list[i].split(",");
                if(ls.length() != 2)
                {
                    throw QString(list[i] + " 格式错误 ");
                }
                int key = ls[0].toInt();
                QString value = ls[1];
                if(i != 0)
                    out = out + "$";
                out = out + QString("%1,%2").arg(key).arg(value);
            }
            setConfig(out);
        }catch(QString exception){
            QMessageBox::information(this, tr("系统提示"), tr("list.u8 list.u32 填写格式为->  0,long_GI$1,short_GI  <-, 下拉列表，以$分割， 0,long_GI 表示值为0 显示long_GI\n 错误信息:\n") + exception);
            return ;
        }
    }
    else if(type == "checkbox.u8")
    {
        try{
        if(config != "0,1")
            throw QString(tr("格式错误"));
        }catch(QString exception){
            QMessageBox::information(this, tr("系统提示"), tr("checkbox.u8 填写-> 0,1 <- 即可，无需修改"));
            return ;
        }
    }
    else
    {
        QMessageBox::information(this, tr("系统提示"), tr("该类型未被支持"));
        return ;
    }

    //ui->txtconfig->setPlainText(checkStr(getConfig()));
    setFlag(true);
    this->close();
}

void frmStructConfig::on_txtkey_textChanged(const QString &str)
{
    ui->txtkey->setText(checkStr(str));
}

void frmStructConfig::on_txtconfig_textChanged()
{
}


QString frmStructConfig::checkStr(QString str)
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
        if(ss[i]>='0' && ss[i]<='9')
        {
            key[j++] = ss[i];
        }
        if(ss[i] == '_')
        {
            key[j++] = ss[i];
        }
    }
    return key;
}


/*get set*/
QString frmStructConfig::getKey()
{
    return ui->txtkey->text();
}
QString frmStructConfig::getUIname()
{
    return ui->txtuiname->text();
}
QString frmStructConfig::getType()
{
    return ui->cbbtype->currentText();
}
QString frmStructConfig::getDefault()
{
    return ui->txtdefault->text();
}
QString frmStructConfig::getConfig()
{
    return ui->txtconfig->toPlainText();
}
QString frmStructConfig::getInfo()
{
    return ui->txtinfo->text();
}
bool frmStructConfig::getFlag()
{
    return flag;
}

void frmStructConfig::setKey(QString key)
{
    ui->txtkey->setText(key);
}
void frmStructConfig::setUIname(QString uiname)
{
    ui->txtuiname->setText(uiname);
}
void frmStructConfig::setType(QString type)
{
    ui->cbbtype->setCurrentText(type);
}
void frmStructConfig::setDefault(QString _default)
{
    ui->txtdefault->setText(_default);
}
void frmStructConfig::setConfig(QString config)
{
    ui->txtconfig->setText(config);
}
void frmStructConfig::setInfo(QString info)
{
    ui->txtinfo->setText(info);
}
void frmStructConfig::setFlag(bool flag)
{
    this->flag = flag;
}


void frmStructConfig::on_txtconfig_cursorPositionChanged()
{
}

void frmStructConfig::on_btnhelp_clicked()
{
    QMessageBox::information(this, tr("系统提示"), "");
}

void frmStructConfig::on_cbbtype_currentIndexChanged(const QString &str)
{
    if(str == "int.u8" || str == "int.u32")
    {
    }
    else if(str == "list.u8" || str == "list.u32")
    {
    }
    else if(str == "checkbox.u8")
    {
        setDefault("0");
        setConfig("0,1");
    }
}
