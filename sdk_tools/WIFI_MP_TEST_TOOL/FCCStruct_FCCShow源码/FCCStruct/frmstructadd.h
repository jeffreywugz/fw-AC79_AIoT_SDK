#ifndef FRMSTRUCTADD_H
#define FRMSTRUCTADD_H

#include <QDialog>

namespace Ui {
class frmStructAdd;
}

class frmStructAdd : public QDialog
{
    Q_OBJECT

public:
    explicit frmStructAdd(QWidget *parent = 0);
    ~frmStructAdd();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::frmStructAdd *ui;

    QString PATH = "config\\struct\\";
};

#endif // FRMSTRUCTADD_H
