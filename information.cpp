#include "information.h"
#include "ui_information.h"

Information::Information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Information)
{
    ui->setupUi(this);
    setWindowTitle("Information");
}

Information::~Information()
{
    delete ui;
}
