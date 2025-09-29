#include "manageaccountdialog.h"
#include "ui_manageaccountdialog.h"
#include <QSettings>
#include <QMessageBox>

ManageAccountDialog::ManageAccountDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageAccountDialog)
{
    ui->setupUi(this);

    connect(&m_authManager, &AuthManager::successAuth, this, [this] {
        QMessageBox::information(this, tr("Success"), tr("Successfully changed user password"));
    });
    connect(&m_authManager, &AuthManager::errorAuth, this, [this](int statusCode, const QString &errorMessage) {
        QMessageBox::critical(this, tr("Error"), tr("Could not change password: ") + errorMessage);
    });
}

ManageAccountDialog::~ManageAccountDialog()
{
    delete ui;
}

void ManageAccountDialog::on_logOutBtn_clicked()
{
    m_authManager.logOut();
    emit loggedOut();
    close();
}



void ManageAccountDialog::on_changePassBtn_clicked()
{
    auto curPass = ui->curPassEdit->text();
    auto newPass1 = ui->newPassEdit->text();
    auto newPass2 = ui->newPassEdit2->text();

    if (curPass.isEmpty()) return;
    if (newPass1 != newPass2) {
        QMessageBox::critical(this, tr("Error"), tr("New password fields do not match"));
        return;
    }

    if (newPass1.isNull() || newPass2.length() < 6) {
        QMessageBox::critical(this, tr("Error"), tr("New password is too short, must be 6+ characters long"));
        return;
    }

    // Send the request
    m_authManager.changePassword(curPass, newPass1);
}

