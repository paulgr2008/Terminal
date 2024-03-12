#ifndef TERMINALDIALOG_H
#define TERMINALDIALOG_H

#include <QDialog>
#include "highlighter.h"
#include "lineedit.h"

class QFont;
class Colorizer;
class TreeModelCompleter;
class QAbstractItemModel;
class QToolButton;

namespace Ui {
    class TerminalDialog;
}

class TerminalDialog : public QDialog {
        Q_OBJECT

    public:
        explicit TerminalDialog(QWidget* parent = nullptr);
        ~TerminalDialog();
        void setTerminalFont(QFont font);
        void changeFontSize(int hotKey);
        void initColorizer(QMap<QString, QColor> colorMap);
        void initCompleter(const QString& modelFileName);
        void setHistory(const QStringList& history);
        const QStringList& history();
        void keyPressEvent( QKeyEvent *e );
        QToolButton* expandButton();

    public slots:
        void dataToTerminal(const QString& data);
    signals:
        void dataFromTerminal(const QString& data);
        void fontChanged(QFont);
    protected:
        void createConnections();
        //void toggleFindWidget(bool visible);
    protected slots:
        void finishCommandEditing(QString data);

        //void keyPressEvent(QKeyEvent* event) override;
        void highlightText();
        void goToNextHighlight();
        void goToPrevHighlight();
        void consoleRefreshHighlighter(int cursorIndex);
        void consoleBtnManager();
        void manageAutocompleter(bool state);
        void closeHotKeySignalManager();
        void fontButtonClicked();
        void cleanConsole();
        void toggleFindEditor(bool isChecked);
        void toggleFontEditor(bool);


    private:
        Ui::TerminalDialog* ui;
        Highlighter m_textHighlighter;
        Colorizer* colorizer;
        bool autocompleteOn;
        TreeModelCompleter* completer = nullptr;
        LineEdit* commandEdit;



        QAbstractItemModel* completerModelFromFile(const QString& modelFileName);

};


#endif // TERMINALDIALOG_H
