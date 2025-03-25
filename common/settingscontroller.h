#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>

class SettingsController : public QObject
{
    Q_OBJECT
public:
    explicit SettingsController(QObject *parent = nullptr);

    Q_INVOKABLE bool resetApp();

signals:
};

#endif // SETTINGSCONTROLLER_H
