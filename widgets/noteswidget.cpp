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
}

NotesWidget::~NotesWidget()
{
    delete ui;
}

void NotesWidget::on_notesView_clicked(const QModelIndex &index)
{
    qDebug() << "begin";
    if (!index.isValid()) {
        return;
    }
    if (infoWidget == nullptr) {
        infoWidget = new InfoWidget();
        infoWidget->resize(1920, 1080);
        ui->stackedWidget->addWidget(infoWidget);
        ui->stackedWidget->setCurrentWidget(infoWidget);
    }
    qDebug() << "after checks";

    Note note;
    note.id = ui->notesView->model()->index(index.row(), 0).data().toInt();
    note.title = ui->notesView->model()->index(index.row(), 1).data().toString();
    note.url = ui->notesView->model()->index(index.row(), 2).data().toString();
    note.username = ui->notesView->model()->index(index.row(), 3).data().toString();
    note.email = ui->notesView->model()->index(index.row(), 4).data().toString();
    note.password = ui->notesView->model()->index(index.row(), 5).data().toString();
    infoWidget->sendNote(note);
}


void NotesWidget::on_createButton_clicked()
{
    NoteCreateDialog* dialog = new NoteCreateDialog(this);
    dialog->exec();
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
    ui->notesView->selectAll();

    if (noteId == infoWidget->getCurrentNoteId()) {
        infoWidget->sendNote(Note{});
    }
}
