#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();



    enum GUITypes {
        Widgets,
        Quick,
    };

private slots:
    void on_guiTypeBox_activated(int index);

    void on_languageBox_activated(int index);

    void on_resetButton_clicked();

    void on_loadButton_clicked();

    void on_exportButton_clicked();


    void on_guiThemeBox_currentIndexChanged(int index);

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
