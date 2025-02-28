#include "secretpasswidget.h"
#include "ui_secretpasswidget.h"
#include <QClipboard>
#include "passwordgenerator.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include "loaderpasswords.h"

SecretPassWidget::SecretPassWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SecretPassWidget)
{
    ui->setupUi(this);
    ui->passwordEdit->setText(PasswordGenerator::generatePassword(32));
}

SecretPassWidget::~SecretPassWidget()
{
    delete ui;
}

void SecretPassWidget::on_copyButton_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->passwordEdit->text());
    isCopied = true;
}

void SecretPassWidget::closeEvent(QCloseEvent *event)
{
    if (!isCopied) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle("Confirmation");
        msgBox.setText("Do you really want to close this? Save the password!");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes) {
            QSettings settings;
            settings.setValue("security/password", ui->passwordEdit->text());
            event->accept();
        } else {
            event->ignore();
        }
    }
}


void SecretPassWidget::on_ackButton_clicked()
{
    close();
}


void SecretPassWidget::on_pushButton_clicked()
{
    LoaderPasswords dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        isCopied = true;
        close();
    }
}

