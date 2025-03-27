#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
class StorageManager : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager{};
public:
    explicit StorageManager(QObject *parent = nullptr);

    Q_INVOKABLE void saveStorage();
    Q_INVOKABLE void loadStorage();


signals:
    void success();
    void error(int statusCode, QString errorMessage);
};

#endif // STORAGEMANAGER_H
