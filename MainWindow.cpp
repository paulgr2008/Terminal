#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(640, 480);
    setCentralWidget(&m_consoleDialog);
}

MainWindow::~MainWindow()
{
}

