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
#include <QApplication>
#include "authdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    // Loading settings...
    QSettings settings;
    QString language = settings.value("gui/language").isValid() ? settings.value("gui/language").toString() : QString("English");
    ui->languageBox->setCurrentText(language);

    QString guiType = settings.value("gui/type").isValid() ? settings.value("gui/type").toString() : QString("Widgets");
    ui->guiTypeBox->setCurrentText(guiType);


    if (settings.value("account/jwtToken").isNull() && settings.value("account/refreshToken").isNull()) {
        disableAccountSettings();
    } else {
        enableAccountSettings();
    }
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

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Language changed"));
    msgBox.setText(tr("Restart the app to apply settings?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes) {
        QApplication::quit();
    }
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
        return;
    }

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir {appDataLoc};
    QFile file(appDataDir.filePath(PasswordManager::PM_FILENAME));

    if (!file.remove() && !appDataDir.removeRecursively()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setText(tr("Application could not be reset. A"));
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
    QDir appDataDir{appDataLoc};
    QString saveFilePath = QFileDialog::getSaveFileName(this, tr("Save storage file"), QDir::homePath(), "PM files (*.pm)");
    if (saveFilePath.isEmpty()) {
        return;
    }

    QFile curStorageFile(appDataDir.filePath(PasswordManager::PM_FILENAME));
    if (!curStorageFile.open(QIODevice::ReadOnly)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setText(tr("Could not open storage file"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    if (QFile::exists(saveFilePath) && !QFile::remove(saveFilePath)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setText(tr("Could not save storage file"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
    if (!curStorageFile.copy(saveFilePath)) { // File is closed before being copied
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setText(tr("Could not save storage file"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
}


void SettingsDialog::on_guiThemeBox_currentIndexChanged(int index)
{
    QString theme = ui->guiThemeBox->itemText(index);
    QSettings settings;
    settings.setValue("gui/theme", theme);
}

void SettingsDialog::enableAccountSettings()
{
    ui->accountDescription->setVisible(true);
    ui->accountTitle->setVisible(true);
    ui->logOutButton->setVisible(true);

    ui->uploadButton->setVisible(true);
    ui->uploadDescription->setVisible(true);
    ui->uploadTitle->setVisible(true);

    ui->loadStorageTitle->setVisible(true);
    ui->loadStorageDescription->setVisible(true);
    ui->loadStorageButton->setVisible(true);
    ui->authButton->setVisible(false);
}

void SettingsDialog::disableAccountSettings()
{
    ui->accountDescription->setVisible(false);
    ui->accountTitle->setVisible(false);
    ui->logOutButton->setVisible(false);

    ui->uploadButton->setVisible(false);
    ui->uploadDescription->setVisible(false);
    ui->uploadTitle->setVisible(false);

    ui->loadStorageTitle->setVisible(false);
    ui->loadStorageDescription->setVisible(false);
    ui->loadStorageButton->setVisible(false);
    ui->authButton->setVisible(true);
}


void SettingsDialog::on_authButton_clicked()
{
    AuthDialog authDialog(authManager, this);
    if (authDialog.exec() == QDialog::Accepted) {
        enableAccountSettings();
    }
}


void SettingsDialog::on_logOutButton_clicked()
{
    QSettings settings;
    settings.remove("account/email");
    settings.remove("account/refreshToken");
    settings.remove("account/jwtToken");

    disableAccountSettings();
}

