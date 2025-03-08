#include "common/mainwindow.h"
#include <QApplication>
#include "dialogs/notecreatedialog.h"
#include <QDebug>
#include <QStyleFactory>
#include <QFile>
#include "widgets/secretpasswidget.h"
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkdir(appDataLoc);
    QDir().mkdir(appDataLoc + "/images");

    QFile file(QApplication::applicationDirPath() + "/general.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(file.readAll());
        file.close();
    }
    a.setStyle(QStyleFactory::create("Fusion"));

    QSettings settings;
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
