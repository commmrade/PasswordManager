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
#include "dotenv.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("klewy");
    QCoreApplication::setOrganizationDomain("klewy.com");
    QCoreApplication::setApplicationName("Password Manager");



    QSettings settings;
    auto guiType = settings.value("gui/type", "Widgets");

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkdir(appDataLoc);
    QDir().mkdir(appDataLoc + "/images");

    DotEnv::instance(); // Load .env

    if (guiType == "Quick") {
        QGuiApplication app(argc, argv);
        app.setOrganizationName("klewy");
        app.setOrganizationDomain("klewy.com");
        app.setApplicationName("Password Manager");

        QTranslator translator;

        QString language = settings.value("gui/language", "English").toString();
        if (language == "Russian") {
            if (!translator.load(":/translation_ru.qm")) {
                qDebug() << "Could not load translation";
            }
        }
        app.installTranslator(&translator);
        qDebug() << language;

        qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Normal");
        qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", "Dark");

        QQmlApplicationEngine engine;
        engine.addImportPath("qrc:/");
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


        AuthManager authManager;
        authManager.validateToken();

        engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

        return app.exec();
    } else {
        QApplication a(argc, argv);
        QTranslator translator;

        QString language = settings.value("gui/language", "English").toString();
        if (language == "Russian") {
            if (!translator.load(":/translation_ru.qm")) {
                qDebug() << "Could not load translation";
            }
        }
        a.installTranslator(&translator);


        QString themeStr = settings.value("gui/theme", "Dark").toString();
        QString themeLoc = themeStr == "Dark" ? "/generalDark.qss" : "/generalLight.qss";


        QFile file(QApplication::applicationDirPath() + themeLoc);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            a.setStyleSheet(file.readAll());
            file.close();
        }
        a.setStyle(QStyleFactory::create("Fusion"));


        auto isFirstTime = settings.value("firstTime", true).toBool();
        if (isFirstTime) {
            SecretPassWidget w;
            w.exec();
            settings.setValue("firstTime", false);
        }

        MainWindow w;
        w.show();
        return a.exec();
    }
}
