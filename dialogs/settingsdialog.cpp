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
    // Loading settings...
    // ui->languageBox->setCurrentText("Russian");
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
    QSettings settings;
    settings.setValue("gui/language", language);
}


void SettingsDialog::on_resetButton_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Confirm Reset"));
    msgBox.setText(tr("Are you sure you want to reset the app?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::No) {
        qDebug() << "reset cancelled";
        return;
    }

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile file(appDataLoc + PasswordManager::PM_FILENAME);
    QDir dir(appDataLoc);
    if (!file.remove() || !dir.removeRecursively()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setText(tr("Application could not be reset."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
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
    loader.exec();
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
    if (QFile::exists(saveFilePath) && !QFile::remove(saveFilePath)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("Could not save storage file");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
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

