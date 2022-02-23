#ifndef FRMSTRUCTCONFIG_H
#define FRMSTRUCTCONFIG_H

#include <QDialog>

namespace Ui {
class frmStructConfig;
}

class frmStructConfig : public QDialog
{
    Q_OBJECT

public:
    explicit frmStructConfig(QWidget *parent = 0);
    ~frmStructConfig();

private slots:
    void on_pushButton_clicked();

    void on_txtkey_textChanged(const QString &arg1);


    void on_txtconfig_textChanged();

    void on_txtconfig_cursorPositionChanged();

    void on_btnhelp_clicked();

    void on_cbbtype_currentIndexChanged(const QString &arg1);

public:
    QString getKey();
    QString getUIname();
    QString getType();
    QString getDefault();
    QString getConfig();
    QString getInfo();
    bool getFlag();
    void setKey(QString key);
    void setUIname(QString uiname);
    void setType(QString type);
    void setDefault(QString _default);
    void setConfig(QString config);
    void setInfo(QString info);
    void setFlag(bool flag);
private:
    QString checkStr(QString str);

private:
    Ui::frmStructConfig *ui;
    bool flag;
};

#endif // FRMSTRUCTCONFIG_H
