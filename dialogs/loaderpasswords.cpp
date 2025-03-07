#include "loaderpasswords.h"
#include "ui_loaderpasswords.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include "consts.h"
#include <QSettings>
#include <QMessageBox>
#include <QStandardPaths>

#include "notecontroller.h"

LoaderPasswords::LoaderPasswords(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoaderPasswords)
{
    ui->setupUi(this);
}

LoaderPasswords::~LoaderPasswords()
{
    delete ui;
}

void LoaderPasswords::on_chooseFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open password manager file", QDir::homePath(), "PM files (*.pm);; All files (*.*)");
    ui->chooseFileButton->setText(filePath);
}


void LoaderPasswords::on_loadButton_clicked()
{
    QString loadFromPath = ui->chooseFileButton->text();
    QString password = ui->passwordEdit->text();
    if (loadFromPath.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "Path/Password is empty", QMessageBox::Ok);
        return;
    }

    QFile file(loadFromPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open";
        QMessageBox::warning(nullptr, "Warning", "File you entered is not accessible", QMessageBox::Ok);
        return;
    }

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile currentFile(appDataLoc + PasswordManager::PM_FILENAME);
    if (!currentFile.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(nullptr, "Warning", "File in cur dir is not accessible. Perhaps run the app as an administrator", QMessageBox::Ok);
        qDebug() << "Could not open current file";
        return;
    }

    char buf[256];
    int bytes{0};
    while ((bytes = file.read(buf, 256)) > 0) {
        currentFile.write(buf, bytes);
    }
    currentFile.flush();
    currentFile.close();
    file.flush();
    file.close();

    QSettings settings;
    settings.setValue("security/password", password.toUtf8());
    NoteController::instance().resetStorage();
    accept();
}


void LoaderPasswords::on_pushButton_2_clicked()
{
    reject();
}

