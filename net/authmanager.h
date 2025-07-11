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

    Q_INVOKABLE void updateToken();

    Q_INVOKABLE void logOut();

    void validateToken();

signals:
    void successAuth();
    void errorAuth(int statusCode, QString errorMessage); // User not only for auth, but for token verification
};

#endif // AUTHMANAGER_H
