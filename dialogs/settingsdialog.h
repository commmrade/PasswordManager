#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "authmanager.h"
#include "storagemanager.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;



    enum GUITypes {
        Widgets,
        Quick,
    };

private slots:
    // void on_guiTypeBox_activated(int index);

    // void on_languageBox_activated(int index);

    void on_resetButton_clicked();

    void on_loadButton_clicked();

    void on_exportButton_clicked();


    // void on_guiThemeBox_activated(int index);

    void on_authButton_clicked();

    void on_logOutButton_clicked();

    void on_loadStorageButton_clicked();

    void on_uploadButton_clicked();

    void on_request_error(int statusCode, const QString &errorMsg);

    void on_languageBox_textChanged(const QString& language);
    void on_themeBox_textChanged(const QString& theme);
    void on_typeBox_textChanged(const QString& type);
private:
    Ui::SettingsDialog *ui;
    AuthManager authManager{};
    StorageManager storageManager{};


    void enableAccountSettings();
    void disableAccountSettings();
};

#endif // SETTINGSDIALOG_H
