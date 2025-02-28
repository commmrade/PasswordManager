#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "noteswidget.h"
#include <QDialog>
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(new NotesWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_settings_triggered()
{
    SettingsDialog dialog;
    dialog.exec();
}

