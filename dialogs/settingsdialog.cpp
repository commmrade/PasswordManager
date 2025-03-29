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
#include <QTranslator>
#include "notecontroller.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    // Loading settings...
    QSettings settings;
    QString language = settings.value("gui/language", "English").toString();
    ui->languageBox->setCurrentText(language);

    QString guiType = settings.value("gui/type", "Widgets").toString();
    ui->guiTypeBox->setCurrentText(guiType);

    QString theme = settings.value("gui/theme", "Dark").toString();
    ui->guiThemeBox->setCurrentText(theme);
    qDebug() << settings.value("account/jwtToken") << settings.value("account/refreshToken");
    if (settings.value("account/jwtToken").toString().isEmpty() && settings.value("account/refreshToken").toString().isEmpty()) {
        disableAccountSettings();
    } else {
        qDebug() << "enabke";
        enableAccountSettings();
    }

    QObject::connect(&storageManager, &StorageManager::success, this, []() {
        NoteController::instance().resetStorage();
    });
    QObject::connect(&storageManager, &StorageManager::error, this, &SettingsDialog::on_request_error);

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_guiTypeBox_activated(int index)
{
    QString uiType = ui->guiTypeBox->itemText(index);
    qDebug() << uiType;
    QSettings settings;
    settings.setValue("gui/type", uiType);

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("GUI type changed"));
    msgBox.setText(tr("Restart the app to apply settings?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes) {
        QApplication::quit();
    }
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

    if (!file.remove() || !appDataDir.removeRecursively()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("Warning"));
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


void SettingsDialog::on_guiThemeBox_activated(int index)
{
    QString theme = ui->guiThemeBox->itemText(index);
    QSettings settings;
    settings.setValue("gui/theme", theme);

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Theme changed"));
    msgBox.setText(tr("Restart the app to apply settings?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes) {
        QApplication::quit();
    }
}


void SettingsDialog::on_loadStorageButton_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Warning"));
    msgBox.setText(tr("Are you sure you want to load the storage backup? It will overwrite the current storage"));

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (msgBox.exec() == QMessageBox::Ok) {
        storageManager.loadStorage();
    }
}


void SettingsDialog::on_uploadButton_clicked()
{
    storageManager.saveStorage();
}

void SettingsDialog::on_request_error(int statusCode, const QString& errorMsg) {
    QMessageBox::warning(this, tr("Warning"), errorMsg);

    if (statusCode == 401) {
        if (authManager.updateToken().isEmpty()) {
            disableAccountSettings();
            QMessageBox::warning(this, tr("Warning"), "Please, log in again!");
        }
    }
}

