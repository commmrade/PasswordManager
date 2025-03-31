#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QObject>

// Class used by QML for clipboard since it doesn't have a built-in one
class Clipboard : public QObject
{
    Q_OBJECT
public:
    explicit Clipboard(QObject *parent = nullptr);

    Q_INVOKABLE void copyText(const QString &text);
signals:
};

#endif // CLIPBOARD_H
