#ifndef TERMINALDIALOG_H
#define TERMINALDIALOG_H

#include <QDialog>
#include "highlighter.h"

class Colorizer;
class TreeModelCompleter;
class QAbstractItemModel;

namespace Ui {
    class TerminalDialog;
}

class TerminalDialog : public QDialog {
        Q_OBJECT

    public:
        explicit TerminalDialog(QWidget* parent = nullptr);
        ~TerminalDialog();
        void setConsoleFont(QFont font);
        void changeFontSize(int hotKey);
        void initColorizer(QMap<QString, QColor> colorMap);
        void initCompleter(const QString& modelFileName);
    public slots:
        void dataToConsole(const QString& data);
    signals:
        void dataFromConsole(const QString& data);
    protected:
        void createConnections();
        //void toggleFindWidget(bool visible);
    protected slots:
        void finishCommandEditing();

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

        QAbstractItemModel* completerModelFromFile(const QString& modelFileName);

};


#endif // TERMINALDIALOG_H
