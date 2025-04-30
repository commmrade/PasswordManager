#include "dotenv.h"

DotEnv::DotEnv() {
    QDir appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    auto dotEnvPath = appDir.filePath("PasswordManager/.env");
    QFile file{dotEnvPath};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error{".env does not exist in " + file.fileName().toStdString()};
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line{in.readLine()};

        auto equalPos = line.indexOf('=');
        if (equalPos == -1) continue;
        auto name = line.left(equalPos);
        auto value = line.mid(equalPos + 1);
        envVariables_.insert(name, value);
    }
}
