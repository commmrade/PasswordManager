#ifndef DOTENV_H
#define DOTENV_H
#include <QDir>
#include <QMap>
#include <QStandardPaths>
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
        return qgetenv(name.toUtf8().data());
    }
private:
    DotEnv();
};

#endif // DOTENV_H
