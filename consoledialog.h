#ifndef CONSOLEDIALOG_H
#define CONSOLEDIALOG_H

#include <QDialog>
#include "highlighter.h"

namespace Ui {
class ConsoleDialog;
}

class ConsoleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConsoleDialog(QWidget *parent = nullptr);
    ~ConsoleDialog();
    void setConsoleFont(QFont font);
    void changeFontSize(int hotKey);
protected:
    void createConnections();
    //void toggleFindWidget(bool visible);
protected slots:

    //void keyPressEvent(QKeyEvent* event) override;
    void highlightText();
    void goToNextHighlight();
    void goToPrevHighlight();
    void consoleRefreshHighlighter(int cursorIndex);
    void consoleBtnManager();
    void manageAutocompleter(int state);
    void closeHotKeySignalManager();
    void fontButtonClicked();
    void cleanConsole();


private:
    Ui::ConsoleDialog *ui;
    Highlighter m_textHighlighter;
};


#endif // CONSOLEDIALOG_H
