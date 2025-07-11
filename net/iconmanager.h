#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

class IconManager : public QObject
{
    Q_OBJECT
    const static inline int MAX_DOWNLOAD_SIZE = 100000;

    QNetworkAccessManager* manager{nullptr};
public:
    explicit IconManager(QObject *parent = nullptr);

    Q_INVOKABLE void downloadImage(QString urlStr, int id);
    Q_INVOKABLE void deleteIcon(int id);


    Q_INVOKABLE bool hasIcon(int id) const;
public slots:

signals:

};

#endif // ICONMANAGER_H
