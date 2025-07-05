#include "settingscontroller.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include "consts.h"
#include <QSettings>
#include <QCoreApplication>

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

bool SettingsController::exportStorage(QString exportDir)
{
    if (exportDir.isEmpty()) {
        return false;
    }

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir{appDataLoc};

    if (exportDir.contains("file://")) {
        exportDir = exportDir.sliced(7); // File paths start with "file://", cut it
    }
    QFile exportFile(exportDir);

    QFile curStorageFile(appDataDir.filePath(PasswordManager::PM_FILENAME));
    if (!curStorageFile.open(QIODevice::ReadOnly)) {
        return false;
    }
    if (exportFile.exists() && !exportFile.remove()) { // If file exists, remove it, so copy works
        return false;
    }

    if (!curStorageFile.copy(exportFile.fileName())) { // File is closed before being copied
        return false;
    }
    return true;
}
