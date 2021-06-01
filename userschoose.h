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
    explicit UsersChoose(QStringList userList,  QWidget *parent = nullptr);
    ~UsersChoose();
    QString getChoosedUser();

private slots:
    void on_newuserb_toggled(bool checked);

    void on_userb_toggled(bool checked);

    void on_gob_clicked();

private:
    Ui::UsersChoose *ui;
    QStringList userList;

};

#endif // USERSCHOOSE_H
