#ifndef MANAGEACCOUNTDIALOG_H
#define MANAGEACCOUNTDIALOG_H

#include <QDialog>
#include "authmanager.h"

namespace Ui {
class ManageAccountDialog;
}

class ManageAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageAccountDialog(QWidget *parent = nullptr);
    ~ManageAccountDialog();

private slots:
    void on_logOutBtn_clicked();
;

    void on_changePassBtn_clicked();

private:
    Ui::ManageAccountDialog *ui;

    AuthManager m_authManager;
signals:
    void loggedOut();
};

#endif // MANAGEACCOUNTDIALOG_H
