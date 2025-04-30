#ifndef DOTENV_H
#define DOTENV_H
#include <QDir>
#include <QMap>
#include <QStandardPaths>
#include <iostream>
#include <QTextStream>
class DotEnv
{
public:

    DotEnv(const DotEnv&) = delete;
    DotEnv& operator=(const DotEnv&) = delete;

    static DotEnv& instance() {
        static DotEnv instance;
        return instance;
    }

    QString getEnvVar(const QString& name) {
        return envVariables_.value(name);
    }
private:
    DotEnv();
    QMap<QString, QString> envVariables_;
};

#endif // DOTENV_H
