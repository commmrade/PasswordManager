#include "authmanager.h"

#include <qeventloop.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>


AuthManager::AuthManager(QObject *parent)
    : QObject{parent}
{}

void AuthManager::registerUser(const QString &username, const QString &email, const QString &password)
{
    QNetworkRequest req{QUrl{"http://localhost:3000/register"}};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    jsonObj["email"] = email;
    jsonObj["password"] = password;
    QJsonDocument jsonDoc{jsonObj};
    QByteArray jsonByte = jsonDoc.toJson();

    auto* reply = manager.post(req, jsonByte);
    connect(reply, &QNetworkReply::finished, this, [this, reply, email] {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();
            QString jwtToken = jsonObj["jwt_token"].toString();
            QString refreshToken = jsonObj["refresh_token"].toString();

            {
                QSettings settings;
                settings.setValue("account/jwtToken", jwtToken);
                settings.setValue("account/refreshToken", refreshToken);
                settings.setValue("account/email", std::move(email));
            }
            qDebug() << "scueess";
            emit successAuth();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();
            QString errorMessage = jsonObj["message"].toString();
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "error";
            emit errorAuth(statusCode, errorMessage);
        }
    });

}
void AuthManager::loginUser(const QString& email, const QString& password) {
    QNetworkRequest req{QUrl{"http://localhost:3000/login"}};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject jsonObj;
    jsonObj["email"] = email;
    jsonObj["password"] = password;
    QJsonDocument jsonDoc{jsonObj};
    QByteArray jsonByte = jsonDoc.toJson();

    auto* reply = manager.post(req, jsonByte);
    connect(reply, &QNetworkReply::finished, this, [this, reply, email] {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();
            QString jwtToken = jsonObj["jwt_token"].toString();
            QString refreshToken = jsonObj["refresh_token"].toString();

            {
                QSettings settings;
                settings.setValue("account/jwtToken", jwtToken);
                settings.setValue("account/refreshToken", refreshToken);
                settings.setValue("account/email", std::move(email));
            }
            qDebug() << "scueess";
            emit successAuth();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();
            QString errorMessage = jsonObj["message"].toString();
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            emit errorAuth(statusCode, errorMessage);
        }
    });
}

QString AuthManager::updateToken() {
    QSettings settings;
    auto refreshToken = settings.value("account/refreshToken").toString();
    if (refreshToken.isEmpty()) {
        return "";
    }
    QNetworkRequest req{QUrl{"http://localhost:3000/token"}};

    req.setRawHeader("Authorization", refreshToken.toUtf8());

    QEventLoop loop;
    auto* reply = manager.get(req);
    connect(reply, &QNetworkReply::finished, &loop, [&loop] {
        loop.exit();
    });
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        auto bytes = reply->readAll();
        settings.setValue("account/jwtToken", bytes);
        return bytes;
    } else {
        // TODO: If 403 error then reset all tokens
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();
        QString errorMessage = jsonObj["message"].toString();
        QMessageBox::warning(nullptr, tr("Warning"), errorMessage);
    }
    return "";
}
