#include "noteswidget.h"
#include "ui_noteswidget.h"
#include "../models/notecontroller.h"
#include "../dialogs/notecreatedialog.h"
#include "../widgets/infowidget.h"
#include "../common/Note.h"


NotesWidget::NotesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NotesWidget)
{
    ui->setupUi(this);
    ui->notesView->setModel(NoteController::instance().getModel());
    ui->notesView->setModelColumn(1);


    infoWidget = new InfoWidget();
    infoWidget->resize(1920, 1080);
    ui->stackedWidget->addWidget(infoWidget);
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
