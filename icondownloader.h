#ifndef ICONDOWNLOADER_H
#define ICONDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>

class IconDownloader : public QObject
{
    Q_OBJECT

    QNetworkAccessManager* manager{nullptr};
public:
    explicit IconDownloader(QObject *parent = nullptr);

    void downloadImage(const QUrl& url, int id);

public slots:

signals:
    void downloadFinished(int id);
};

#endif // ICONDOWNLOADER_H
