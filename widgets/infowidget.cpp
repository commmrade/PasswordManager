#include "infowidget.h"
#include "ui_infowidget.h"
#include "../models/notecontroller.h"

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
    if (currentNoteIndex == -1) {
        qDebug() << "note not set";
        return;
    }
    NoteController::instance().setTitle(currentNoteIndex, ui->titleEdit->text());
}


void InfoWidget::on_urlEdit_editingFinished()
{
    if (currentNoteIndex == -1) {
        qDebug() << "note not set";
        return;
    }
    NoteController::instance().setUrl(currentNoteIndex, ui->urlEdit->text());
}


void InfoWidget::on_userEdit_editingFinished()
{
    if (currentNoteIndex == -1) {
        qDebug() << "note not set";
        return;
    }
    NoteController::instance().setUsername(currentNoteIndex, ui->userEdit->text());
}
void InfoWidget::on_emailEdit_editingFinished()
{
    if (currentNoteIndex == -1) {
        qDebug() << "note not set";
        return;
    }
    NoteController::instance().setEmail(currentNoteIndex, ui->emailEdit->text());
}


void InfoWidget::on_passwordEdit_editingFinished()
{
    if (currentNoteIndex == -1) {
        qDebug() << "note not set";
        return;
    }
    NoteController::instance().setPassword(currentNoteIndex, ui->passwordEdit->text());
}


void InfoWidget::on_deleteButton_clicked()
{
    qDebug() << currentNoteIndex;
    if (currentNoteIndex < 0) {
        qDebug() << "note not set";
        return;
    }
    NoteController::instance().deleteNote(currentNoteIndex);
    sendNote(Note{});
}

