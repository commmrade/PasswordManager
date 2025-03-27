#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

class AuthManager : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager{};
public:
    explicit AuthManager(QObject *parent = nullptr);

    Q_INVOKABLE void registerUser(const QString& username, const QString& email, const QString& password);
    Q_INVOKABLE void loginUser(const QString& email, const QString& password);

    Q_INVOKABLE QString updateToken();

    Q_INVOKABLE void logOut();

signals:
    void successAuth();
    void errorAuth(int statusCode, QString errorMessage);
};

#endif // AUTHMANAGER_H
