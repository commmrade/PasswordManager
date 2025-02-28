#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QSettings>
#include <QDir>
#include "consts.h"
#include <QMessageBox>
#include <QCoreApplication>
#include "loaderpasswords.h"
#include <QFileDialog>
#include <QStandardPaths>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_guiTypeBox_activated(int index)
{
    QString uiType = ui->guiTypeBox->itemText(index);

    QSettings settings;
    settings.setValue("gui/type", uiType);
}


void SettingsDialog::on_languageBox_activated(int index)
{
    QString language = ui->languageBox->itemText(index);
    qDebug() << language;
    QSettings settings;
    settings.setValue("gui/language", language);
    qDebug() << "change gui interface in runtime";
}


void SettingsDialog::on_resetButton_clicked()
{
    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile file(appDataLoc + PasswordManager::PM_FILENAME);

    if (!file.remove()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning); // Set the icon to Critical
        msgBox.setWindowTitle(tr("Error")); // Set the title of the message box
        msgBox.setText(tr("Application could not be reset.")); // Set the main message
        msgBox.setStandardButtons(QMessageBox::Ok); // Add an OK button
        msgBox.setDefaultButton(QMessageBox::Ok); // Set the default button
        msgBox.exec();
        return;
    }

    QDir dir(appDataLoc);
    if (!dir.removeRecursively()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning); // Set the icon to Critical
        msgBox.setWindowTitle(tr("Error")); // Set the title of the message box
        msgBox.setText(tr("Application could not be reset.")); // Set the main message
        msgBox.setStandardButtons(QMessageBox::Ok); // Add an OK button
        msgBox.setDefaultButton(QMessageBox::Ok); // Set the default button
        msgBox.exec();
        return;
    }

    QSettings settings;
    settings.clear();

    QCoreApplication::exit();
}


void SettingsDialog::on_loadButton_clicked()
{
    LoaderPasswords loader(this);
    if (loader.exec() == QDialog::Accepted) {
        qDebug() << "loaded";
        emit storageLoaded();
    }
}


void SettingsDialog::on_exportButton_clicked()
{
    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);


    QString saveFilePath = QFileDialog::getSaveFileName(this, tr("Save storage file"), QDir::homePath(), "PM files (*.pm)");
    QFile curStorageFile(appDataLoc + PasswordManager::PM_FILENAME);
    if (!curStorageFile.open(QIODevice::ReadOnly)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("Could not open file:");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    if (QFile::exists(saveFilePath)) {
        if (!QFile::remove(saveFilePath)) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error");
            msgBox.setText("Could not save storage file:");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    if (!curStorageFile.copy(saveFilePath)) { // File is closed before being copied
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("Could not save storage file:");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
}

