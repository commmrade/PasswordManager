#ifndef APPCONTROL_H
#define APPCONTROL_H

#include <QObject>

class AppControl : public QObject
{
    Q_OBJECT
    explicit AppControl(QObject *parent = nullptr);
public:
    static AppControl& instance() {
        static AppControl control;
        return control;
    }
    Q_INVOKABLE void restartApp();
signals:
};

#endif // APPCONTROL_H
