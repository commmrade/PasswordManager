#include "common/mainwindow.h"
#include <QApplication>
#include "dialogs/notecreatedialog.h"
#include <QDebug>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;

    NoteCreateDialog dialog;
    dialog.exec();

    w.show();
    return a.exec();
}
