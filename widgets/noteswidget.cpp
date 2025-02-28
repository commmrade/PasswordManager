#include "noteswidget.h"
#include "ui_noteswidget.h"
#include "notecontroller.h"
#include "notecreatedialog.h"
#include "infowidget.h"
#include "Note.h"
#include <QDir>
#include <QStandardPaths>
#include "../icondownloader.h"

NotesWidget::NotesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NotesWidget)
    , downloader(new IconDownloader(this))
{
    ui->setupUi(this);
    ui->notesView->setModel(NoteController::instance().getModel());
    ui->notesView->setModelColumn(1);






    for (int i = 0; i < ui->notesView->model()->rowCount(); ++i) {
        auto id = ui->notesView->model()->index(i, 0).data().toInt();
        auto link = ui->notesView->model()->index(i, 2).data().toString();

        QUrl url = QString(link + "/favicon.ico");
        downloader->downloadImage(url, id);
    }

    infoWidget = new InfoWidget();
    infoWidget->resize(1920, 1080);
    ui->stackedWidget->addWidget(infoWidget);

    connect(infoWidget, &InfoWidget::urlChanged, this, &NotesWidget::on_url_changed);
}

NotesWidget::~NotesWidget()
{
    delete ui;
}

void NotesWidget::on_notesView_clicked(const QModelIndex &index)
{
    if (!index.isValid() || ui->notesView->model()->index(index.row(), 0).data().toInt() == infoWidget->getCurrentNoteId()) {
        return;
    }
    if (infoWidget->getCurrentNoteId() == -1) {
        ui->stackedWidget->setCurrentWidget(infoWidget);
    }
    Note note;

    auto& instance = NoteController::instance();
    note.id = ui->notesView->model()->index(index.row(), 0).data().toInt();
    note.title = instance.getTitle(note.id);
    note.url = instance.getUrl(note.id);
    note.username = instance.getUsername(note.id);
    note.email = instance.getEmail(note.id);
    note.password = instance.getPassword(note.id);
    infoWidget->sendNote(note);
}


void NotesWidget::on_createButton_clicked()
{
    NoteCreateDialog dialog(this);
    dialog.exec();
}

void NotesWidget::on_deleteButton_clicked()
{
    QModelIndex currentIndex = ui->notesView->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }
    int row = currentIndex.row();
    int noteId = ui->notesView->model()->index(row, 0).data().toInt();

    NoteController::instance().deleteNote(noteId);

    if (noteId == infoWidget->getCurrentNoteId()) {
        infoWidget->sendNote(Note{});
    }
}

void NotesWidget::on_url_changed(int id)
{

    auto url = NoteController::instance().getUrl(id);
    QUrl urlIcon = QString(url + "/favicon.ico");
    downloader->downloadImage(urlIcon, id);
}
