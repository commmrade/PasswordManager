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

    constexpr static inline char digits[] = {"0123456789"};
    constexpr static inline char symbols[] = {"!@#$%^&*()-_+=[]{}`~;:'\"<>,.?/\\"};
    constexpr static inline char letters[] = {"abcdefghijklmnopqrstuvwxyz"};
public:
    explicit PasswordGenerator(QObject* parent = nullptr) : QObject(parent) {};
    Q_INVOKABLE QString generatePassword(unsigned int len = 16);
    static QString generatePswd(unsigned int len = 16);
};

#endif // PASSWORDGENERATOR_H
