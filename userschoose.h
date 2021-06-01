#ifndef USERSCHOOSE_H
#define USERSCHOOSE_H

#include <QDialog>

namespace Ui {
class UsersChoose;
}

class UsersChoose : public QDialog
{
    Q_OBJECT

public:
    explicit UsersChoose(QWidget *parent = nullptr);
    ~UsersChoose();

private:
    Ui::UsersChoose *ui;
};

#endif // USERSCHOOSE_H
