#include "storagemanager.h"
#include <QHttpMultiPart>
#include <QStandardPaths>
#include <QDir>
#include <QSettings>
#include <QJsonObject>
#include "consts.h"
#include <QJsonDocument>
#include <QNetworkReply>

StorageManager::StorageManager(QObject *parent)
    : QObject{parent}
{}

void StorageManager::saveStorage() {
    QHttpMultiPart* multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QString mimeType = "application/vnd.sqlite3";

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"user\"; filename=\"pmanager.pm\""));

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir {appDataLoc};

    QFile* file = new QFile(appDataDir.filePath(PasswordManager::PM_FILENAME));
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << file->errorString();
        return;
    }

    file->setParent(multipart);
    filePart.setBodyDevice(file);

    multipart->append(filePart);

    auto backendUrl = qgetenv("BACKEND_URL");
    QUrl url{backendUrl + "/upload"};
    QNetworkRequest request(url);

    QSettings settings;
    QString authToken = settings.value("account/jwtToken").toString();
    if (authToken.isEmpty()) {
        return;
    }

    QHttpHeaders headers;
    headers.append(QHttpHeaders::WellKnownHeader::Authorization, "Bearer " + authToken);
    request.setHeaders(headers);

    auto* reply = manager.post(request, multipart);
    multipart->setParent(reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        if (reply->error() == QNetworkReply::NoError) {
           emit success();
        } else {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << statusCode;
            if (statusCode == 401) {
                emit error(statusCode, tr("Authorization error. Try again"));
            } else if (statusCode >= 500) {
                emit error(statusCode, tr("Server internal error"));
            } else {
                emit error(0, tr("Server is dead / No internet"));
            }
        }
        reply->deleteLater();
    });
}

void StorageManager::loadStorage(QString masterPassword) {

    auto backendUrl = qgetenv("BACKEND_URL");
    QUrl url{backendUrl + "/download"};

    QNetworkRequest request{url};
    QHttpHeaders headers;
    QSettings settings;
    headers.append(QHttpHeaders::WellKnownHeader::Authorization, "Bearer " + settings.value("account/jwtToken").toString());
    request.setHeaders(headers);

    auto* reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, masterPassword = std::move(masterPassword)] {
        if (reply->error() == QNetworkReply::NoError) {
            QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            QDir appDataDir {appDataLoc};
            QFile storageFile{appDataDir.filePath("pmanager.pm")};
            if (storageFile.open(QIODevice::WriteOnly)) {
                QByteArray buf;
                while ((buf = reply->read(4096)).size() > 0) {
                    storageFile.write(buf);
                }
                storageFile.flush();
                storageFile.close();

                QSettings settings;
                settings.setValue("security/password", masterPassword);

                emit success();
            }
        } else {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 401) {
                emit error(statusCode, tr("Authorization error. Try again"));
            } else if (statusCode >= 500) {
                emit error(statusCode, tr("Server internal error"));
            } else if (statusCode == 404) {
                emit error(statusCode, tr("You haven't backed up anything"));
            } else {
                emit error(0, tr("Server is dead / No internet"));
            }
        }
        reply->deleteLater();
    });
}












