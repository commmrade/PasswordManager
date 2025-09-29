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
#include <QInputDialog>
#include "notecontroller.h"
#include "manageaccountdialog.h"
#include <QProcess>
#include "appcontrol.h"

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
    if (settings.value("account/jwtToken").toString().isEmpty() && settings.value("account/refreshToken").toString().isEmpty()) { // If user doesn't have tokens set up - unauthorized (dumb but idc)
        disableAccountSettings();
    } else {
        enableAccountSettings();
    }

    QObject::connect(ui->languageBox, &QComboBox::currentTextChanged, this, &SettingsDialog::on_languageBox_textChanged);
    QObject::connect(ui->guiThemeBox, &QComboBox::currentTextChanged, this, &SettingsDialog::on_themeBox_textChanged);
    QObject::connect(ui->guiTypeBox, &QComboBox::currentTextChanged, this, &SettingsDialog::on_typeBox_textChanged);

    QObject::connect(&storageManager, &StorageManager::success, this, []() {
        NoteController::instance().resetStorage();
    });
    QObject::connect(&storageManager, &StorageManager::error, this, &SettingsDialog::on_request_error);

    QObject::connect(&authManager, &AuthManager::errorAuth, this, &SettingsDialog::on_request_error); // Connect for token update error
    // We don't care about the case, when token is updated succesfully since in that case we'd do nothing
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
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

    if (!settingsController.resetApp()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("Warning"));
        msgBox.setText(tr("Application could not be reset."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
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

    if (!settingsController.exportStorage(std::move(saveFilePath))) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setText(tr("Could not save storage file"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

void SettingsDialog::enableAccountSettings()
{
    ui->accountDescription->setVisible(true);
    ui->accountTitle->setVisible(true);
    // ui->logOutButton->setVisible(true);
    ui->manageAccBtn->setVisible(true);

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
    ui->manageAccBtn->setVisible(false);

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


void SettingsDialog::on_loadStorageButton_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Warning"));
    msgBox.setText(tr("Are you sure you want to load the storage backup? It will overwrite the current storage"));

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QString text = QInputDialog::getText(this, tr("Form"), tr("Enter the master password of your backup:"), QLineEdit::EchoMode::Password);
    if (text.isEmpty()) {
        return;
    }

    if (msgBox.exec() == QMessageBox::Ok) {
        storageManager.loadStorage(text);
    }
}


void SettingsDialog::on_uploadButton_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Warning"));
    msgBox.setText(tr("Are you sure you want to upload the storage backup? It will overwrite the backup storage"));

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (msgBox.exec() == QMessageBox::Ok) {
        storageManager.saveStorage();
    }
}

void SettingsDialog::on_request_error(int statusCode, const QString& errorMsg) {
    QMessageBox::warning(this, tr("Warning"), errorMsg);

    if (statusCode == 401) {
        authManager.updateToken();
    } else if (statusCode == 403) { // If FORBIDDEN, token update failed
        disableAccountSettings();
        QMessageBox::warning(this, tr("Warning"), tr("Please, log in again!"));
    }
}

void SettingsDialog::on_languageBox_textChanged(const QString &language)
{
    QSettings settings;
    settings.setValue("gui/language", language);

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Language changed"));
    msgBox.setText(tr("Restart the app to apply settings?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes) {
        // restartApp();
        AppControl::instance().restartApp();
    }
    qDebug() << "Language box changed";
}

void SettingsDialog::on_themeBox_textChanged(const QString &theme)
{
    QSettings settings;
    settings.setValue("gui/theme", theme);

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Theme changed"));
    msgBox.setText(tr("Restart the app to apply settings?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes) {
        // QApplication::quit();
        AppControl::instance().restartApp();
    }
}

void SettingsDialog::on_typeBox_textChanged(const QString &type)
{
    QSettings settings;
    settings.setValue("gui/type", type);

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("GUI type changed"));
    msgBox.setText(tr("Restart the app to apply settings?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes) {
        AppControl::instance().restartApp();
        // QApplication::quit();
    }
}



void SettingsDialog::on_manageAccBtn_clicked()
{
    ManageAccountDialog accountDialog(this);
    connect(&accountDialog, &ManageAccountDialog::loggedOut, this, [this] {
        disableAccountSettings();
    });
    accountDialog.exec();
}

