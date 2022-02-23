#include "fccshowstruct.h"
#include "ui_fccshowstruct.h"

FCCShowStruct::FCCShowStruct(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FCCShowStruct)
{
    ui->setupUi(this);
}

FCCShowStruct::~FCCShowStruct()
{
    delete ui;
}
