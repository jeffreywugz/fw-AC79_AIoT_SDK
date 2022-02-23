#ifndef FCCSHOWSTRUCT_H
#define FCCSHOWSTRUCT_H

#include <QDialog>

namespace Ui {
class FCCShowStruct;
}

class FCCShowStruct : public QDialog
{
    Q_OBJECT

public:
    explicit FCCShowStruct(QWidget *parent = 0);
    ~FCCShowStruct();

private:
    Ui::FCCShowStruct *ui;
};

#endif // FCCSHOWSTRUCT_H
