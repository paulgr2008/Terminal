#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "consoledialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    ConsoleDialog m_consoleDialog;

};
#endif // MAINWINDOW_H
