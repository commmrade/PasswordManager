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
#include <QQmlApplicationEngine>
#include "notecontroller.h"
#include <QQmlContext>
#include "authmanager.h"
#include "clipboard.h"
#include "passwordgenerator.h"
#include "iconmanager.h"
#include "loadercontroller.h"
#include "settingscontroller.h"
#include "storagemanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("klewy");
    app.setOrganizationDomain("klewy.com");
    app.setApplicationName("Password Manager");
    QCoreApplication::setOrganizationName("klewy");
    QCoreApplication::setOrganizationDomain("klewy.com");
    QCoreApplication::setApplicationName("Password Manager");

    QTranslator translator;
    QSettings settings;

    // TODO: сделать загрузку в зависимости от локали, если язык не установлен
    QString language = settings.value("gui/language").isValid() ? settings.value("gui/language").toString() : QString("English");
    if (language == "Russian") {
        if (!translator.load(":/translation_ru.qm")) {
            qDebug() << "Could not load translation";
        }
    }
    app.installTranslator(&translator);
    qDebug() << language;

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkdir(appDataLoc);
    QDir().mkdir(appDataLoc + "/images");


    qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Normal");
    qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", "Dark");


    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");
    //engine.addImportPath("qrc:/qml");
    qmlRegisterType<PasswordGenerator>("PasswordGenerator", 1, 0, "PasswordGenerator");
    qmlRegisterType<IconManager>("IconManager", 1, 0, "IconManager");
    qmlRegisterType<LoaderController>("LoaderController", 1, 0, "LoaderController");
    qmlRegisterType<SettingsController>("SettingsController", 1, 0, "SettingsController");
    qmlRegisterType<AuthManager>("AuthManager", 1, 0, "AuthManager");
    qmlRegisterType<StorageManager>("StorageManager", 1, 0, "StorageManager");

    Clipboard clipboard;
    auto& ins = NoteController::instance();
    engine.rootContext()->setContextProperty("noteController", &ins);
    engine.rootContext()->setContextProperty("clipboard", &clipboard);

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    return app.exec();

    // QApplication a(argc, argv);
    // QTranslator translator;
    // QSettings settings;

    // // TODO: сделать загрузку в зависимости от локали, если язык не установлен
    // QString language = settings.value("gui/language").isValid() ? settings.value("gui/language").toString() : QString("English");
    // if (language == "Russian") {
    //     if (!translator.load(":/translation_ru.qm")) {
    //         qDebug() << "Could not load translation";
    //     }
    // }
    // a.installTranslator(&translator);

    // QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // QDir().mkdir(appDataLoc);
    // QDir().mkdir(appDataLoc + "/images");

    // QString themeStr = settings.value("gui/theme").isValid() ? settings.value("gui/theme").toString() : QString("Dark");
    // QString themeLoc = themeStr == "Dark" ? "/generalDark.qss" : "/generalLight.qss";


    // QFile file(QApplication::applicationDirPath() + themeLoc);
    // if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     a.setStyleSheet(file.readAll());
    //     file.close();
    // }
    // a.setStyle(QStyleFactory::create("Fusion"));


    // // Handle settings like language, ui type and etc.

    // auto isFirstTime = settings.value("firstTime").toBool();
    // if (!isFirstTime) {
    //     SecretPassWidget w;
    //     w.exec();
    //     settings.setValue("firstTime", true);
    // }

    // MainWindow w;
    // w.show();
    // return a.exec();
}
