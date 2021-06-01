#include "userschoose.h"
#include "ui_userschoose.h"

UsersChoose::UsersChoose(QStringList userList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersChoose)
{
    ui->setupUi(this);
    this->userList = userList;
    ui->userc->addItems(this->userList);
}



UsersChoose::~UsersChoose()
{
    delete ui;
}


QString UsersChoose::getChoosedUser()
{
  if(ui->userb->isChecked() && !ui->userc->currentText().isEmpty()  )
    return ui->userc->currentText();
  else if(ui->newuserb->isChecked() && !ui->newusere->text().trimmed().isEmpty())
      return ui->newusere->text().trimmed();
  else return QString();
}

void UsersChoose::on_newuserb_toggled(bool checked)
{
ui->newusere->setEnabled(checked);
ui->newuserl->setEnabled(checked);
ui->userc->setEnabled(ui->userb->isChecked());
ui->userl->setEnabled(ui->userb->isChecked());
}


void UsersChoose::on_userb_toggled(bool checked)
{
   ui->userc->setEnabled(checked);
   ui->userl->setEnabled(checked);
   ui->newusere->setEnabled(ui->newuserb->isChecked());
   ui->newuserl->setEnabled(ui->newuserb->isChecked());
}


void UsersChoose::on_gob_clicked()
{
    close();
}

