#ifndef LOADERCONTROLLER_H
#define LOADERCONTROLLER_H

#include <QObject>

class LoaderController : public QObject
{
    Q_OBJECT
public:
    explicit LoaderController(QObject *parent = nullptr);

signals:
};

#endif // LOADERCONTROLLER_H
