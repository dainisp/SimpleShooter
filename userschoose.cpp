#include "userschoose.h"
#include "ui_userschoose.h"

UsersChoose::UsersChoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersChoose)
{
    ui->setupUi(this);
}

UsersChoose::~UsersChoose()
{
    delete ui;
}
