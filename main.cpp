#include "common/mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QStyleFactory>
#include <QFile>
#include "widgets/secretpasswidget.h"
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QSettings settings;

    // TODO: сделать загрузку в зависимости от локали, если язык не установлен
    QString language = settings.value("gui/language").isValid() ? settings.value("gui/language").toString() : QString("English");
    if (language == "Russian") {
        if (!translator.load(":/translation_ru.qm")) {
            qDebug() << "Could not load translation";
        }
    }
    a.installTranslator(&translator);

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkdir(appDataLoc);
    QDir().mkdir(appDataLoc + "/images");
    QFile file(QApplication::applicationDirPath() + "/general.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(file.readAll());
        file.close();
    }
    a.setStyle(QStyleFactory::create("Fusion"));


    // Handle settings like language, ui type and etc.

    auto isFirstTime = settings.value("firstTime").toBool();
    if (!isFirstTime) {
        SecretPassWidget w;
        w.exec();
        settings.setValue("firstTime", true);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
