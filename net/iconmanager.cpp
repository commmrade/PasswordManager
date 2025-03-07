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

void IconManager::downloadImage(QString urlStr, int id)
{
    if (!urlStr.contains("http://") || !urlStr.contains("https://")) {
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
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply, id]{
        QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QFile::remove(appDataLoc + "/images/" + QString::number(id) + ".ico");
        QPixmap pixmap;
        if (!pixmap.loadFromData(reply->readAll())) {
            qDebug() << "The return from the url is not an image";
            reply->deleteLater();
            return;
        }
        pixmap.save(appDataLoc + "/images/" + QString::number(id) + ".ico");
        qDebug() << "saved icon";
        reply->deleteLater();
    });
}

bool IconManager::hasIcon(int id) const
{
    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QFile::exists(appDataLoc + "/images/" + QString::number(id) + ".ico");
}


