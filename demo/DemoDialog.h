#ifndef DEMODIALOG_H
#define DEMODIALOG_H

#include <QMainWindow>
#include "terminaldialog.h"
class QFont;
class DemoDialog : public QMainWindow {
        Q_OBJECT

    public:
        DemoDialog(QWidget* parent = nullptr);
        ~DemoDialog();
        void printDataFromConsole(const QString& data);
public slots:
        void terminalFontChanged(QFont);
    protected:
        TerminalDialog* m_terminalDialog;

};
#endif // DEMODIALOG_H
