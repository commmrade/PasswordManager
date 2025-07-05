#include "dotenv.h"

DotEnv::DotEnv() {
    QDir appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    auto dotEnvPath = appDir.filePath(".env");
    QFile file{dotEnvPath};
    if (file.open(QIODevice::Text | QIODevice::ReadWrite)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line{in.readLine()};
            auto equalPos = line.indexOf('=');
            if (equalPos == -1) continue;
            auto name = line.left(equalPos);
            auto value = line.mid(equalPos + 1);
            qputenv(name.toUtf8().data(), value.toUtf8().data());
        }
        file.flush();
        file.close();
    }
}
