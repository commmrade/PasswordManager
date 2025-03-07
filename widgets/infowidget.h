#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include "../common/Note.h"


namespace Ui {
class InfoWidget;
}

class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();


    void sendNote(const Note& note);

    int getCurrentNoteId() const { return currentNoteIndex; }

private slots:
    void on_titleEdit_editingFinished();

    void on_urlEdit_editingFinished();

    void on_userEdit_editingFinished();

    void on_emailEdit_editingFinished();

    void on_passwordEdit_editingFinished();

    void on_deleteButton_clicked();

private:
    Ui::InfoWidget *ui;
    int currentNoteIndex{-1};

signals:
    void urlChanged(int id);
};

#endif // INFOWIDGET_H
