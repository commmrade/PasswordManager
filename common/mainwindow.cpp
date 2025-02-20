#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../widgets/noteswidget.h"
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

