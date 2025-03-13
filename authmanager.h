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

    void registerUser(const QString& username, const QString& email, const QString& password);
    void loginUser(const QString& email, const QString& password);

    QString updateToken();

signals:
    void successAuth();
    void errorAuth(int statusCode, QString errorMessage);
};

#endif // AUTHMANAGER_H
