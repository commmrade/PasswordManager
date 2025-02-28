#include "notecreatedialog.h"
#include "ui_notecreatedialog.h"
#include "notecontroller.h"
#include <QMessageBox>
#include "passwordgenerator.h"

NoteCreateDialog::NoteCreateDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NoteCreateWidget)
{
    ui->setupUi(this);
}

NoteCreateDialog::~NoteCreateDialog()
{
    delete ui;
}

void NoteCreateDialog::on_createButton_clicked()
{
    if (ui->titleEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
        qDebug() << "One of the fields empty...Abort";
        int ret = QMessageBox::warning(this, "Title or password empty", "Please, make sure both fields are filled with text");
        return;
    }
    auto& controller = NoteController::instance();
    controller.createNote(ui->titleEdit->text(), ui->urlEdit->text(), ui->usernameEdit->text(), ui->emailEdit->text(), ui->passwordEdit->text());
    accept();
}


void NoteCreateDialog::on_pushButton_clicked()
{
    ui->passwordEdit->setText(PasswordGenerator::generatePassword());
}

