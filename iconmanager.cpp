#include "iconmanager.h"
#include <QNetworkReply>
#include <QPixmap>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

IconManager::IconManager(QObject *parent)
    : QObject{parent}, manager(new QNetworkAccessManager(this))
{

}

void IconManager::downloadImage(const QUrl &url, int id)
{
    QNetworkRequest request(url);
    auto* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, id]{
        QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QFile::remove(appDataLoc + "/images/" + QString::number(id) + ".ico");
        QPixmap pixmap;
        if (!pixmap.loadFromData(reply->readAll())) {
            qDebug() << "not found icon";
            reply->deleteLater();
            return;
        }


        pixmap.save(appDataLoc + "/images/" + QString::number(id) + ".ico");
        qDebug() << "saved?";
        emit downloadFinished(id);
        reply->deleteLater();
    });
}

bool IconManager::hasIcon(int id) const
{
    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QFile::exists(appDataLoc + "/images/" + QString::number(id) + ".ico");
}


