#include "noteswidget.h"
#include "ui_noteswidget.h"
#include "notecontroller.h"
#include "notecreatedialog.h"
#include "infowidget.h"
#include "Note.h"
#include <QDir>
#include <QStandardPaths>
#include "iconmanager.h"
#include <QMenu>

NotesWidget::NotesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NotesWidget)
    , iconManager(new IconManager(this))
{
    ui->setupUi(this);
    ui->notesView->setModel(NoteController::instance().getModel());
    ui->notesView->setModelColumn(1);
    ui->notesView->setIconSize({32, 32});
    ui->notesView->setContextMenuPolicy(Qt::CustomContextMenu);

    loadIcons(); // Downloads icons in case if STORAGE was loaded from somewhere
    infoWidget = new InfoWidget();
    ui->stackedWidget->addWidget(infoWidget);
    connect(ui->notesView, &QListView::customContextMenuRequested, this, &NotesWidget::on_contextMenu_requested);
    connect(infoWidget, &InfoWidget::urlChanged, this, &NotesWidget::on_url_changed);
}

NotesWidget::~NotesWidget()
{
    delete ui;
}

void NotesWidget::on_contextMenu_requested(const QPoint &pos)
{
    QPoint globalPos = ui->notesView->mapToGlobal(pos);
    auto selectedModelIndex = ui->notesView->indexAt(pos);
    if (!selectedModelIndex.isValid()) {
        return;
    }

    QMenu contextMenu(ui->notesView);
    QAction* deleteItem = contextMenu.addAction("Delete");
    auto* selectedMenuItem = contextMenu.exec(globalPos);
    if (selectedMenuItem == deleteItem)
    {
        auto id = ui->notesView->model()->index(selectedModelIndex.row(), 0).data().toInt();
        NoteController::instance().deleteNote(id);
    }
}

void NotesWidget::on_notesView_clicked(const QModelIndex &index)
{
    if (!index.isValid() || ui->notesView->model()->index(index.row(), 0).data().toInt() == infoWidget->getCurrentNoteId()) {
        return;
    }
    if (infoWidget->getCurrentNoteId() == -1) {
        infoWidget->show();
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
    if (dialog.exec() == QDialog::Accepted) {
        auto &controller = NoteController::instance();
        auto lastId = controller.getLastInsertId();
        auto lastUrl = controller.getUrl(lastId);
        iconManager->downloadImage(lastUrl + "/favicon.ico", lastId);
    }
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

    infoWidget->hide();
    infoWidget->sendNote(Note{});
    ui->stackedWidget->setCurrentIndex(-1);
}

void NotesWidget::on_url_changed(int id)
{
    auto& controller = NoteController::instance();
    iconManager->deleteIcon(id);
    auto url = controller.getUrl(id);
    QString urlIcon{url + "/favicon.ico"};
    iconManager->downloadImage(urlIcon, id);
}



void NotesWidget::loadIcons()
{
    for (int i = 0; i < ui->notesView->model()->rowCount(); ++i) {
        auto id = ui->notesView->model()->index(i, 0).data().toInt();
        if (!iconManager->hasIcon(id)) {
            auto link = ui->notesView->model()->index(i, 2).data().toString();
            QString url{link + "/favicon.ico"};
            iconManager->downloadImage(url, id);
        }
    }
}
