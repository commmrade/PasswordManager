#ifndef SECRETPASSWIDGET_H
#define SECRETPASSWIDGET_H

#include <QDialog>

namespace Ui {
class SecretPassWidget;
}

class SecretPassWidget : public QDialog
{
    Q_OBJECT

public:
    explicit SecretPassWidget(QWidget *parent = nullptr);
    ~SecretPassWidget();

private slots:
    void on_copyButton_clicked();

    void on_ackButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::SecretPassWidget *ui;
    bool isCopied{false};
    bool isLoaded{false};

    void closeEvent(QCloseEvent* event) override;
};

#endif // SECRETPASSWIDGET_H
