#ifndef NOTESWIDGET_H
#define NOTESWIDGET_H

#include <QWidget>


class InfoWidget;
class IconManager;
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

    void on_url_changed(int id);

private:
    Ui::NotesWidget *ui;
    InfoWidget* infoWidget{nullptr};
    IconManager* iconManager{nullptr};


    void loadIcons();
};

#endif // NOTESWIDGET_H
