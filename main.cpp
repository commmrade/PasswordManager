#include "common/mainwindow.h"
#include <QApplication>
#include "dialogs/notecreatedialog.h"
#include <QDebug>
#include <QStyleFactory>
#include <QFile>
#include "widgets/secretpasswidget.h"
#include <QSettings>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("../../style.qss"); // Save the QSS above in a file named "styles.qss"
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(file.readAll());
        file.close();
    }
    a.setStyle(QStyleFactory::create("Fusion"));

    QSettings settings;
    //settings.clear();
    auto isFirstTime = settings.value("firstTime").toBool();
    if (!isFirstTime) {
        SecretPassWidget w;
        w.resize(1280, 720);
        w.exec();
    }

    MainWindow w;
    w.resize(1920, 1080);
    w.show();
    return a.exec();
}
