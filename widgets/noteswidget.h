#ifndef NOTESWIDGET_H
#define NOTESWIDGET_H

#include <QWidget>

namespace Ui {
class NotesWidget;
}

class NotesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NotesWidget(QWidget *parent = nullptr);
    ~NotesWidget();

private:
    Ui::NotesWidget *ui;
};

#endif // NOTESWIDGET_H
