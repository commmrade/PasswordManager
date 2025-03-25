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

    if (!file.remove() && !appDataDir.removeRecursively()) {
        return false;
    }
    QSettings settings;
    settings.clear();
    QCoreApplication::exit();
    return true;
}
