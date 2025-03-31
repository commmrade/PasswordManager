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
    QFile file(appDataDir.filePath(PasswordManager::PM_FILENAME));

    if (!file.remove() || !appDataDir.removeRecursively()) {
        return false;
    }
    QSettings settings;
    settings.clear();
    QCoreApplication::exit(); // TODO: Try to make the app truly restart
    return true;
}

bool SettingsController::exportStorage(const QString &exportDir)
{
    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir{appDataLoc};

    if (exportDir.isEmpty()) {
        return false;
    }

    QFile exportFile(exportDir.sliced(7));
    qDebug() << exportDir;

    QFile curStorageFile(appDataDir.filePath(PasswordManager::PM_FILENAME));
    if (!curStorageFile.open(QIODevice::ReadOnly)) {
        qDebug() << "here 1";
        return false;
    }
    qDebug() << exportFile.exists();
    if (exportFile.exists() && !exportFile.remove()) {
        qDebug() << "here 2";
        return false;
    }

    if (!curStorageFile.copy(exportFile.fileName())) { // File is closed before being copied
        qDebug() << curStorageFile.errorString();
        qDebug() << "here 3";
        return false;
    }
    return true;
}
