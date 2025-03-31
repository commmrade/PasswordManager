#ifndef LOADERCONTROLLER_H
#define LOADERCONTROLLER_H

#include <QObject>
#include <qqmlintegration.h>

class LoaderController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit LoaderController(QObject *parent = nullptr);

    Q_INVOKABLE bool loadStorage(const QString& loadFromPath, const QString& password);
signals:
};

#endif // LOADERCONTROLLER_H
