#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

class IconManager : public QObject
{
    Q_OBJECT

    QNetworkAccessManager* manager{nullptr};
public:
    explicit IconManager(QObject *parent = nullptr);

    void downloadImage(const QUrl& url, int id);


    bool hasIcon(int id) const;
public slots:

signals:
    void downloadFinished(int id);
};

#endif // ICONMANAGER_H
