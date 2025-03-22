#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H
#include <QObject>
#include <qqml.h>
#include <qtmetamacros.h>
#include <QString>

class PasswordGenerator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit PasswordGenerator(QObject* parent = nullptr) : QObject(parent) {};
    Q_INVOKABLE QString generatePassword(unsigned int len = 16);
};

#endif // PASSWORDGENERATOR_H
