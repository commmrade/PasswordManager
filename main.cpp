#include "common/mainwindow.h"
#include <QApplication>
#include "dialogs/notecreatedialog.h"
#include <QDebug>
#include <QStyleFactory>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("../../style.qss"); // Save the QSS above in a file named "styles.qss"
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(file.readAll());
        file.close();
    }

    a.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;


    w.show();
    return a.exec();
}
