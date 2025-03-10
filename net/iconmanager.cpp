#include "iconmanager.h"
#include <QNetworkReply>
#include <QPixmap>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include "notecontroller.h"

IconManager::IconManager(QObject *parent)
    : QObject{parent}, manager(new QNetworkAccessManager(this))
{

}

void IconManager::downloadImage(QString urlStr, int id)
{
    if (!urlStr.contains("http://") && !urlStr.contains("https://")) {
        urlStr.insert(0, "http://");
    }
    qDebug() << "Link is:" << urlStr;
    QUrl url(urlStr);
    if (!url.isValid()) {
        qDebug() << "Invalid url";
        return;
    }

    QNetworkRequest request(url);
    auto* reply = manager->get(request);
   QObject::connect(reply, &QNetworkReply::downloadProgress, this, [this, reply] (qint64 bytesReceived, qint64 bytesTotal) {
       qDebug() << "Bytes received: " << bytesReceived;
        if (bytesReceived > MAX_DOWNLOAD_SIZE) {
            reply->abort();
        }
   });

    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply, id]{
        if (reply->error() == QNetworkReply::NoError) {
            QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            QDir appDataDir{appDataLoc};
            QFile::remove(appDataDir.filePath("images/" + QString::number(id) + ".ico"));
            QPixmap pixmap;
            if (!pixmap.loadFromData(reply->readAll())) {
                qDebug() << "The return from the url is not an image";
                reply->deleteLater();
                return;
            }
            pixmap.save(appDataDir.filePath("images/" + QString::number(id) + ".ico"));
        } else {
            qDebug() << "Download failed" << reply->errorString();
        }
        reply->deleteLater();
    });
}

bool IconManager::hasIcon(int id) const
{
    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir{appDataLoc};
    return QFile::exists(appDataDir.filePath("images/" + QString::number(id) + ".ico"));
}

void IconManager::deleteIcon(int id) {
    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir{appDataLoc};
    if (!QFile::remove(appDataDir.filePath("images/" + QString::number(id) + ".ico"))) {
        qDebug() << "Could not remove the icon";
    }
    NoteController::instance().removeFromCache(id);
}


