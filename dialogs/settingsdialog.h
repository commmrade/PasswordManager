#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "authmanager.h"
#include "storagemanager.h"
#include "settingscontroller.h"

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
    void on_resetButton_clicked();
    void on_loadButton_clicked();
    void on_exportButton_clicked();
    void on_authButton_clicked();
    void on_logOutButton_clicked();
    void on_loadStorageButton_clicked();
    void on_uploadButton_clicked();
    void on_request_error(int statusCode, const QString &errorMsg);

    void on_languageBox_indexChanged(int index);
    void on_themeBox_indexChanged(int index);
    void on_typeBox_indexChanged(int index);

    void on_manageAccBtn_clicked();

    void on_applyButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::SettingsDialog *ui;
    AuthManager authManager{};
    StorageManager storageManager{};

    SettingsController settingsController;
    bool m_guiTypeChanged{false};
    bool m_languageChanged{false};
    bool m_themeChanged{false};
    bool m_restartRequired{false};

    void enableAccountSettings();
    void disableAccountSettings();
    void applyGeneralSettings();
};

#endif // SETTINGSDIALOG_H
