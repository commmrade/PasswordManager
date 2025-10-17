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
#include <QtQml/qqml.h>
#include "appcontrol.h"
#include "settingsvalues.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("klewy");
    QCoreApplication::setOrganizationDomain("klewy.com");
    QCoreApplication::setApplicationName("PasswordManager");

    QSettings settings;
    int guiType = settings.value(SettingsNames::GUI_TYPE, SettingsValues::GUI_TYPE_WIDGETS).toInt();

    QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkdir(appDataLoc);
    QDir().mkdir(appDataLoc + "/images");

    DotEnv{}; // Loads env variables

    if (guiType == SettingsValues::GUI_TYPE_QUICK) {
        QGuiApplication app(argc, argv);
        app.setOrganizationName("klewy");
        app.setOrganizationDomain("klewy.com");
        app.setApplicationName("PasswordManager");

        QTranslator translator;

        int language = settings.value(SettingsNames::GUI_LANGUAGE, SettingsValues::GUI_LANGUAGE_ENGLISH).toInt();
        if (language == SettingsValues::GUI_LANGUAGE_RUSSIAN) {
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
        auto& appControl = AppControl::instance();
        engine.rootContext()->setContextProperty("noteController", &ins);
        engine.rootContext()->setContextProperty("clipboard", &clipboard);
        engine.rootContext()->setContextProperty("appControl", &appControl);

        AuthManager authManager;
        authManager.validateToken();

        engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

        return app.exec();
    } else {
        QApplication a(argc, argv);
        QTranslator translator;

        int language = settings.value(SettingsNames::GUI_LANGUAGE, SettingsValues::GUI_LANGUAGE_ENGLISH).toInt();
        if (language == SettingsValues::GUI_LANGUAGE_RUSSIAN) {
            if (!translator.load(":/translation_ru.qm")) {
                qDebug() << "Could not load translation";
            }
        }
        a.installTranslator(&translator);


        int themeStr = settings.value(SettingsNames::GUI_THEME, SettingsValues::GUI_THEME_DARK).toInt();
        QString themeLoc = themeStr == SettingsValues::GUI_THEME_DARK ? ":/qss/generalDark.qss" : ":/qss/generalLight.qss";


        QFile file(themeLoc);
        if (file.open(QIODevice::ReadOnly)) {
            a.setStyleSheet(file.readAll());
            file.close();
        } else {
            qDebug() << themeLoc;
            qDebug() << "could not open theme" << file.errorString();
        }
        a.setStyle(QStyleFactory::create("Fusion"));


        auto isFirstTime = settings.value(SettingsNames::FIRST_TIME, true).toBool();
        if (isFirstTime) {
            SecretPassWidget w;
            w.exec();
            settings.setValue(SettingsNames::FIRST_TIME, false);
        }

        MainWindow w;
        w.show();
        return a.exec();
    }
}
