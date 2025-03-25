#include "loadercontroller.h"
#include "notecontroller.h"
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QSettings>

#include "consts.h"

LoaderController::LoaderController(QObject *parent)
    : QObject{parent}
{}

bool LoaderController::loadStorage(const QString& loadFromPath, const QString& password) {
    if (loadFromPath.isEmpty() || password.isEmpty()) {
        return false;
    }

    QFile file(loadFromPath.sliced(7));
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open load from storage";
        return false;
    }

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir {appDataLoc};
    QFile currentFile(appDataDir.filePath(PasswordManager::PM_FILENAME));
    qDebug() << appDataDir.filePath(PasswordManager::PM_FILENAME);
    if (!currentFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open current storage";
        return false;
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
    //settings.setValue("security/password", password);
    settings.sync();
    NoteController::instance().resetStorage();
    return true;
}
