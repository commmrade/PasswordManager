#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "authmanager.h"

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(AuthManager &authManager, QWidget *parent = nullptr);
    ~AuthDialog() override;

private slots:
    void on_continueRegButton_clicked();

    void successAuthenticated();
    void errorAuthenticated(int statusCode, const QString& errorMessage);
    void on_continueLogButton_clicked();

private:
    Ui::AuthDialog *ui;

    QNetworkAccessManager manager{};
    AuthManager &authManager;
};

#endif // AUTHDIALOG_H
