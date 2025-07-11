#include "settingscontroller.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include "consts.h"
#include <QSettings>
#include <QCoreApplication>
#include <filesystem>

SettingsController::SettingsController(QObject *parent)
    : QObject{parent}
{}

bool SettingsController::resetApp()
{
    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir {appDataLoc};
    if (!appDataDir.removeRecursively()) {
        return false;
    }

    QSettings settings;
    settings.clear();

    QCoreApplication::exit();
    return true;
}

bool SettingsController::exportStorage(QString exportDestination)
{
    if (exportDestination.isEmpty()) {
        return false;
    }

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir{appDataLoc};

    if (exportDestination.contains("file://")) {
        exportDestination = exportDestination.sliced(7); // File paths start with "file://", cut it
    }

    QFile exportFile(exportDestination);
    QFile curStorageFile(appDataDir.filePath(PasswordManager::PM_FILENAME));
  
    if (!std::filesystem::copy_file(curStorageFile.fileName().toStdString(), exportFile.fileName().toStdString(), std::filesystem::copy_options::overwrite_existing)) {
        return false;
    }
    return true;
}
