#ifndef NOTESWIDGET_H
#define NOTESWIDGET_H

#include <QWidget>


class InfoWidget;

namespace Ui {
class NotesWidget;
}

class NotesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NotesWidget(QWidget *parent = nullptr);
    ~NotesWidget();

private slots:
    void on_notesView_clicked(const QModelIndex &index);

    void on_createButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::NotesWidget *ui;
    InfoWidget* infoWidget{nullptr};
};

#endif // NOTESWIDGET_H
