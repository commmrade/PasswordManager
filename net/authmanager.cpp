#include "authmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QUrlQuery>
#include "dotenv.h"
AuthManager::AuthManager(QObject *parent)
    : QObject{parent}
{
    QObject::connect(this, &AuthManager::errorAuth, this, [](int statusCode, const QString& message) {
        if (statusCode == 403) {
            QSettings settings;
            settings.remove("account/email");
            settings.remove("account/refreshToken");
            settings.remove("account/jwtToken");
        }
    });
}

void AuthManager::registerUser(const QString &username, const QString &email, const QString &password)
{
    QString backendUrl = qgetenv("BACKEND_URL");
    QNetworkRequest req{QUrl{backendUrl + "/register"}};
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
                settings.setValue("account/jwtToken", std::move(jwtToken));
                settings.setValue("account/refreshToken", std::move(refreshToken));
                settings.setValue("account/email", std::move(email));
            }
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
    auto backendUrl = qgetenv("BACKEND_URL");
    QNetworkRequest req{QUrl{backendUrl + "/login"}};
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

void AuthManager::updateToken() {
    QSettings settings;
    auto refreshToken = settings.value("account/refreshToken").toString();
    if (refreshToken.isEmpty()) {
        return;
    }
    auto backendUrl = qgetenv("BACKEND_URL");

    QNetworkRequest req{QUrl{backendUrl + "/token"}};

    req.setRawHeader("Authorization", "Bearer " + refreshToken.toUtf8());

    auto* reply = manager.get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        QSettings settings;
        if (reply->error() == QNetworkReply::NoError) {
            auto bytes = reply->readAll();
            settings.setValue("account/jwtToken", bytes);
            emit successAuth();
        } else {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();
            QString errorMessage = jsonObj["message"].toString();

            if (statusCode == 403) {
                settings.remove("account/jwtToken");
                settings.remove("account/refreshToken");
                settings.remove("account/email");
                emit errorAuth(statusCode, errorMessage);
            }
            emit errorAuth(statusCode, errorMessage);
        }
        reply->deleteLater();
    });

}

void AuthManager::logOut()
{
    QSettings settings;
    auto backendUrl = qgetenv("BACKEND_URL");
    QUrl url{backendUrl + "/logout"};
    QNetworkRequest request{std::move(url)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["refresh_token"] = settings.value("account/refreshToken", "").toString();
    QJsonDocument jsonDoc{jsonObj};
    QByteArray jsonBytes {jsonDoc.toJson()};

    auto* reply = manager.post(request, jsonBytes);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        QSettings settings;
        settings.remove("account/email");
        settings.remove("account/refreshToken");
        settings.remove("account/jwtToken");

        reply->deleteLater();
    });


}

void AuthManager::validateToken()
{
    QSettings settings;

    auto backendUrl = qgetenv("BACKEND_URL");
    QUrl url{backendUrl + "/validate"};
    QUrlQuery query;
    query.addQueryItem("token", settings.value("account/jwtToken").toString());
    url.setQuery(query);

    QNetworkRequest request{url};

    auto* reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        if (reply->error() != QNetworkReply::NoError) {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            if (statusCode == 401) {
                updateToken();
            }
        }
        reply->deleteLater();
    });

}
