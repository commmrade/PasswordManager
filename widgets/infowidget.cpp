#include "infowidget.h"
#include "ui_infowidget.h"
#include "notecontroller.h"
#include <QMessageBox>

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::sendNote(const Note &note)
{
    currentNoteIndex = note.id;
    ui->titleEdit->setText(note.title);
    ui->urlEdit->setText(note.url);
    ui->userEdit->setText(note.username);
    ui->emailEdit->setText(note.email);
    ui->passwordEdit->setText(note.password);
}

void InfoWidget::on_titleEdit_editingFinished()
{
    if (currentNoteIndex == -1 || ui->titleEdit->text().isEmpty()) {
        int ret = QMessageBox::warning(this, tr("Title or password empty"), tr("Please, make sure both fields are filled with text"));
        return;
    }
    NoteController::instance().setTitle(currentNoteIndex, ui->titleEdit->text());
}


void InfoWidget::on_urlEdit_editingFinished()
{
    if (currentNoteIndex == -1 || ui->urlEdit->text().isEmpty()) {
        return;
    }
    NoteController::instance().setUrl(currentNoteIndex, ui->urlEdit->text());
    emit urlChanged(currentNoteIndex);
}


void InfoWidget::on_userEdit_editingFinished()
{
    if (currentNoteIndex == -1 || ui->userEdit->text().isEmpty()) {
        return;
    }
    NoteController::instance().setUsername(currentNoteIndex, ui->userEdit->text());
}
void InfoWidget::on_emailEdit_editingFinished()
{
    if (currentNoteIndex == -1 || ui->emailEdit->text().isEmpty()) {
        return;
    }
    NoteController::instance().setEmail(currentNoteIndex, ui->emailEdit->text());
}


void InfoWidget::on_passwordEdit_editingFinished()
{
    if (currentNoteIndex == -1 || ui->passwordEdit->text().isEmpty()) {
        int ret = QMessageBox::warning(this, tr("Title or password empty"), tr("Please, make sure both fields are filled with text"));
        return;
    }
    NoteController::instance().setPassword(currentNoteIndex, ui->passwordEdit->text());
}


