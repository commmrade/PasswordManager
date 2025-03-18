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

    void saveStorage();
    void loadStorage();


signals:
    void success();
    void error(int statusCode, QString errorMessage);
};

#endif // STORAGEMANAGER_H
