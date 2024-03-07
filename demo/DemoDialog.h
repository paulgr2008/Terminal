#ifndef DEMODIALOG_H
#define DEMODIALOG_H

#include <QMainWindow>
#include "terminaldialog.h"

class DemoDialog : public QMainWindow {
        Q_OBJECT

    public:
        DemoDialog(QWidget* parent = nullptr);
        ~DemoDialog();
        void printDataFromConsole(const QString& data);

    protected:
        TerminalDialog* m_terminalDialog;

};
#endif // DEMODIALOG_H
