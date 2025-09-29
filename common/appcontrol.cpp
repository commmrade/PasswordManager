#include "appcontrol.h"
#include <QProcess>
#include <QApplication>

AppControl::AppControl(QObject *parent)
    : QObject{parent}
{}

void AppControl::restartApp()
{
    QString const binaryPath = QCoreApplication::applicationFilePath();
    QApplication::quit(); // Kill old app process and start a detached new one
    QProcess::startDetached(binaryPath, qApp->arguments().mid(1));
}
