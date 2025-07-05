#include "loadercontroller.h"
#include "notecontroller.h"
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QSettings>
#include <filesystem>

#include "consts.h"

LoaderController::LoaderController(QObject *parent)
    : QObject{parent}
{}

bool LoaderController::loadStorage(const QString& loadFromPath, const QString& password) {
    if (loadFromPath.isEmpty() || password.isEmpty()) {
        return false;
    }

    QFile file(loadFromPath.sliced(7)); // File paths start with file://, cut it
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir {appDataLoc};
    QFile currentFile(appDataDir.filePath(PasswordManager::PM_FILENAME));

    if (!currentFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open current storage";
        return false;
    }

    char buf[1024];
    int bytes{0};
    while ((bytes = file.read(buf, sizeof(buf))) > 0) { // I can't just copy file, since NoteModle woudl reference old file
        currentFile.write(buf, bytes);
    }
    currentFile.flush();
    currentFile.close();
    file.flush();
    file.close();

    NoteController::instance().resetStorage();
    return true;
}
