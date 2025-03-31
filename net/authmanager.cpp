#include "authmanager.h"

#include <qeventloop.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QUrlQuery>

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
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode != 0) {
                QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject jsonObj = jsonDoc.object();
                QString errorMessage = jsonObj["message"].toString();

                emit errorAuth(statusCode, errorMessage);
            } else {
                emit errorAuth(0, "Server is offline");
            }
        }
        reply->deleteLater();
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
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            if (statusCode != 0) {
                QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject jsonObj = jsonDoc.object();
                QString errorMessage = jsonObj["message"].toString();
                emit errorAuth(statusCode, errorMessage);
            } else {
                emit errorAuth(0, "Server is offline");
            }
        }
        reply->deleteLater();
    });
}

QString AuthManager::updateToken() {
    QSettings settings;
    auto refreshToken = settings.value("account/refreshToken").toString();
    if (refreshToken.isEmpty()) {
        return "";
    }
    QNetworkRequest req{QUrl{"http://localhost:3000/token"}};

    req.setRawHeader("Authorization", "Bearer " + refreshToken.toUtf8());

    QEventLoop loop;
    auto* reply = manager.get(req);
    connect(reply, &QNetworkReply::finished, &loop, [&loop] {
        loop.exit();
    });
    loop.exec();
    qDebug() << "refresh is" << refreshToken;
    if (reply->error() == QNetworkReply::NoError) {
        auto bytes = reply->readAll();
        settings.setValue("account/jwtToken", bytes);
        return bytes;
    } else {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode == 403) {
            settings.remove("account/jwtToken");
            settings.remove("account/refreshToken");
            settings.remove("account/email");
        }
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();
        QString errorMessage = jsonObj["message"].toString();
    }
    delete reply;
    return "";
}

void AuthManager::logOut()
{
    QSettings settings;
    settings.remove("account/email");
    settings.remove("account/refreshToken");
    settings.remove("account/jwtToken");
}

void AuthManager::validateToken()
{
    QSettings settings;

    QUrl url{"http://localhost:3000/validate"};
    QUrlQuery query;
    query.addQueryItem("token", settings.value("account/jwtToken").toString());
    url.setQuery(query);

    QNetworkRequest request{url};

    auto* reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        if (reply->error() != QNetworkReply::NoError) {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            if (statusCode == 401) {
                if (updateToken().isEmpty()) {
                    QSettings settings;
                    settings.remove("account/email");
                    settings.remove("account/refreshToken");
                    settings.remove("account/jwtToken");
                }
            }
        }
        reply->deleteLater();
    });

}
