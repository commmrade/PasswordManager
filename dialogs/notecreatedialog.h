#ifndef NOTECREATEWIDGET_H
#define NOTECREATEWIDGET_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class NoteCreateWidget;
}

class NoteCreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoteCreateDialog(QWidget *parent = nullptr);
    ~NoteCreateDialog();

private slots:
    void on_createButton_clicked();

private:

    Ui::NoteCreateWidget *ui;
};

#endif // NOTECREATEWIDGET_H
