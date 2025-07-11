#ifndef LOADERPASSWORDS_H
#define LOADERPASSWORDS_H

#include <QDialog>

namespace Ui {
class LoaderPasswords;
}

class LoaderPasswords : public QDialog
{
    Q_OBJECT

public:
    explicit LoaderPasswords(QWidget *parent = nullptr);
    ~LoaderPasswords();

private slots:
    void on_chooseFileButton_clicked();

    void on_loadButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::LoaderPasswords *ui;
};

#endif // LOADERPASSWORDS_H
