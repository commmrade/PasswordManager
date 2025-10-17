#include "authmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QUrlQuery>
#include "dotenv.h"
#include "settingsvalues.h"

AuthManager::AuthManager(QObject *parent)
    : QObject{parent}
{
    QObject::connect(this, &AuthManager::errorAuth, this, [](int statusCode, const QString& message) {
        if (statusCode == 403) {
            QSettings settings;
            settings.remove(SettingsNames::ACCOUNT_EMAIL);
            settings.remove(SettingsNames::ACCOUNT_REFRESHTOKEN);
            settings.remove(SettingsNames::ACCOUNT_JWTTOKEN);
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
                settings.setValue(SettingsNames::ACCOUNT_JWTTOKEN, std::move(jwtToken));
                settings.setValue(SettingsNames::ACCOUNT_REFRESHTOKEN, std::move(refreshToken));
                settings.setValue(SettingsNames::ACCOUNT_EMAIL, std::move(email));
            }
            emit successAuth();
        } else {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode != 0) {
                QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject jsonObj = jsonDoc.object();
                QString errorMessage = jsonObj["error_msg"].toString();

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
                settings.setValue(SettingsNames::ACCOUNT_JWTTOKEN, jwtToken);
                settings.setValue(SettingsNames::ACCOUNT_REFRESHTOKEN, refreshToken);
                settings.setValue(SettingsNames::ACCOUNT_EMAIL, std::move(email));
            }
            qDebug() << "scueess";
            emit successAuth();
        } else {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            if (statusCode != 0) {
                QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject jsonObj = jsonDoc.object();
                QString errorMessage = jsonObj["error_msg"].toString();
                emit errorAuth(statusCode, errorMessage);
            } else {
                emit errorAuth(0, "Server is offline");
            }
        }
        reply->deleteLater();
    });
}

void AuthManager::changePassword(const QString &password, const QString &newPassword)
{
    QSettings settings;
    auto jwtToken = settings.value(SettingsNames::ACCOUNT_JWTTOKEN).toString();
    if (jwtToken.isEmpty()) {
        return;
    }

    auto backendUrl = qgetenv("BACKEND_URL");

    QNetworkRequest req{QUrl{backendUrl + "/reset"}};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer " + jwtToken.toUtf8());

    QJsonObject jsonObj;
    jsonObj["password"] = password;
    jsonObj["new_password"] = newPassword;
    QJsonDocument jsonDoc{jsonObj};
    QByteArray jsonBytes = jsonDoc.toJson();

    auto* reply = manager.post(req, jsonBytes);
    connect(reply, &QNetworkReply::finished, this, [this, reply]{
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "SUCCESSFULLY UPDATED PASSWORD";
            emit successAuth();
        } else {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode != 0) {
                QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject jsonObj = jsonDoc.object();
                QString errorMessage = jsonObj["error_msg"].toString();
                emit errorAuth(statusCode, errorMessage);
            } else {
                emit errorAuth(statusCode, "Server is offline");
            }
        }
    });
}

void AuthManager::updateToken() {
    QSettings settings;
    auto refreshToken = settings.value(SettingsNames::ACCOUNT_REFRESHTOKEN).toString();
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
            settings.setValue(SettingsNames::ACCOUNT_JWTTOKEN, bytes);
            emit successAuth();
        } else {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();
            QString errorMessage = jsonObj["error_msg"].toString();

            if (statusCode == 403) {
                settings.remove(SettingsNames::ACCOUNT_JWTTOKEN);
                settings.remove(SettingsNames::ACCOUNT_REFRESHTOKEN);
                settings.remove(SettingsNames::ACCOUNT_EMAIL);
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
    auto refreshToken = settings.value(SettingsNames::ACCOUNT_REFRESHTOKEN).toString();
    if (refreshToken.isEmpty()) {
        return;
    }
    auto backendUrl = qgetenv("BACKEND_URL");
    QUrl url{backendUrl + "/logout"};
    QNetworkRequest request{std::move(url)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["refresh_token"] = std::move(refreshToken);
    QByteArray jsonBytes {QJsonDocument{jsonObj}.toJson()};

    auto* reply = manager.post(request, jsonBytes);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        reply->deleteLater();
    });
    settings.remove(SettingsNames::ACCOUNT_EMAIL);
    settings.remove(SettingsNames::ACCOUNT_REFRESHTOKEN);
    settings.remove(SettingsNames::ACCOUNT_JWTTOKEN);
}

void AuthManager::validateToken()
{
    QSettings settings;
    auto jwtToken = settings.value(SettingsNames::ACCOUNT_JWTTOKEN).toString();
    if (jwtToken.isEmpty()) {
        return;
    }

    auto backendUrl = qgetenv("BACKEND_URL");
    QUrl url{backendUrl + "/validate"};
    QUrlQuery query;
    query.addQueryItem("token", std::move(jwtToken));
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
