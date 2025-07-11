#include "authdialog.h"
#include "ui_authdialog.h"
#include <QMessageBox>

AuthDialog::AuthDialog(AuthManager &authManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthDialog)
    , authManager(authManager)
{
    ui->setupUi(this);
    QObject::connect(&authManager, &AuthManager::successAuth, this, &AuthDialog::successAuthenticated);
    QObject::connect(&authManager, &AuthManager::errorAuth, this, &AuthDialog::errorAuthenticated);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_continueRegButton_clicked()
{
    if ((ui->usernameRegEdit->text().isNull() || ui->usernameRegEdit->text().length() < 4)
        || (ui->emailRegEdit->text().isNull()) || ui->passwordRegEdit->text().isNull()
        || ui->passwordRegEdit->text().length() < 6)
    {
        QMessageBox::warning(this, "Warning", "Please, fill out all fields.");
        return;
    }
    authManager.registerUser(ui->usernameRegEdit->text(), ui->emailRegEdit->text(), ui->passwordRegEdit->text());
}

void AuthDialog::successAuthenticated() {
    QMessageBox::about(this, tr("Authentication"), tr("Successfully authenticated"));
    accept();
}

void AuthDialog::errorAuthenticated(int statusCode, const QString &errorMessage) {
    QMessageBox::warning(this, tr("Warning"), errorMessage);
}


void AuthDialog::on_continueLogButton_clicked()
{
    if ((ui->emailLogEdit->text().isNull())
       || (ui->passwordLogEdit->text().isNull()
       || ui->passwordLogEdit->text().length() < 6))
    {
        QMessageBox::warning(this, "Warning", "Please, fill out all fields.");
        return;
    }
    authManager.loginUser(ui->emailLogEdit->text(), ui->passwordLogEdit->text());
}

