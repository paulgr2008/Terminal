#include "terminaldialog.h"
#include "ui_terminaldialog.h"
#include <QFontDialog>
#include <QFile>
#include <QStringListModel>
#include <QStandardItemModel>
#include "completerloader.h"
#include "colorizer.h"
#include "treemodelcompleter.h"


/* TODO:
 * 1. Terminal mode: dev info, reboot and disconnect messages to a title???
*/

TerminalDialog::TerminalDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::TerminalDialog)
{
    ui->setupUi(this);

    m_textHighlighter.setDocument(ui->consoleEditor->document());
    QFont deffont("Cascadia Mono", 11);
    setTerminalFont(deffont);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    p.setColor(QPalette::Highlight, Qt::gray);
    p.setColor(QPalette::HighlightedText, Qt::white);
    ui->consoleEditor->setPalette(p);
    ui->commandEditor->setPalette(p);
    commandEdit = ui->commandEditor;
    completer = new TreeModelCompleter(this);
    toggleFindEditor(false);
    toggleFontEditor(false);
    createConnections();
}

TerminalDialog::~TerminalDialog()
{
    delete ui;
}

void TerminalDialog::toggleFindEditor(bool isChecked)
{
    ui->findToolButton->setText(isChecked ? "Find:" : "Find");
    ui->searchArea->setVisible(isChecked);
}

void TerminalDialog::toggleFontEditor(bool isChecked)
{
    ui->fontToolButton->setText(isChecked ? "Font:" : "Font");
    ui->fontEditorArea->setVisible(isChecked);
}

void TerminalDialog::createConnections()
{
    connect(ui->searchEditor, &QLineEdit::textChanged, this, &TerminalDialog::highlightText, Qt::DirectConnection);
    connect(ui->nextButton, &QToolButton::clicked, this, &TerminalDialog::goToNextHighlight, Qt::DirectConnection);
    connect(ui->prevButton, &QToolButton::clicked, this, &TerminalDialog::goToPrevHighlight, Qt::DirectConnection);
    connect(ui->autocomplChBox, &QCheckBox::clicked,
            this, &TerminalDialog::manageAutocompleter);
    connect(ui->fullScreenButton, &QToolButton::clicked,
            this, &TerminalDialog::consoleBtnManager);
    connect(ui->cleanButton, &QToolButton::clicked,
            this, &TerminalDialog::cleanConsole);

    connect(ui->changeFontButton, &QToolButton::clicked,
            this, &TerminalDialog::fontButtonClicked);
    connect(ui->increaseFontSzBtn, &QToolButton::clicked,
            this, &TerminalDialog::fontButtonClicked);
    connect(ui->decreaseFontSzBtn, &QToolButton::clicked,
            this, &TerminalDialog::fontButtonClicked);
    connect(commandEdit, &LineEdit::lineExecuted, this, &TerminalDialog::finishCommandEditing);
    connect(ui->findToolButton, &QToolButton::clicked, this, &TerminalDialog::toggleFindEditor);
    connect(ui->fontToolButton, &QToolButton::clicked, this, &TerminalDialog::toggleFontEditor);

}


void TerminalDialog::finishCommandEditing(QString data)
{
    emit dataFromConsole(data);
}

void TerminalDialog::cleanConsole()
{
    ui->consoleEditor->clear();
    ui->consoleEditor->setFocus();
}

void TerminalDialog::highlightText()
{
    m_textHighlighter.setWordPattern(ui->searchEditor->text());
    m_textHighlighter.customRehighlight();
}

void TerminalDialog::consoleRefreshHighlighter(int cursorIndex)
{
    if (cursorIndex >= 0) {
        QTextCursor currentCursor = ui->consoleEditor->textCursor();
        currentCursor.setPosition(cursorIndex);
        ui->consoleEditor->setTextCursor(currentCursor);
        ui->consoleEditor->ensureCursorVisible();
    }
}

void TerminalDialog::consoleBtnManager()
{

}

void TerminalDialog::manageAutocompleter(bool state)
{
    commandEdit->setCompleter((state) ? completer : nullptr);
}

void TerminalDialog::closeHotKeySignalManager()
{

}

void TerminalDialog::fontButtonClicked()
{
    QToolButton* button = static_cast<QToolButton*>(this->sender());

    if (button == ui->changeFontButton)
        setTerminalFont(QFontDialog::getFont(0, ui->consoleEditor->font(), NULL, QString(),
                                            QFontDialog::MonospacedFonts));
    else if (button == ui->decreaseFontSzBtn) {
        changeFontSize(Qt::Key_Minus);
    }
    else if (button == ui->increaseFontSzBtn) {
        changeFontSize(Qt::Key_Plus);
    }
}

void TerminalDialog::setTerminalFont(QFont font)
{
    ui->consoleEditor->document()->setDefaultFont(font);
    ui->commandEditor->setFont(font);
}



void TerminalDialog::changeFontSize(int hotKey)
{
    bool isPixelSize = false;
    QFont font = ui->consoleEditor->document()->defaultFont();
    int fontSize = font.pointSize();

    if (fontSize < 0) {
        fontSize = font.pixelSize();
        isPixelSize = true;
    }

    if (hotKey == Qt::Key_Plus) {
        fontSize++;
    }
    else if (hotKey == Qt::Key_Minus) {
        fontSize--;
    }

    if (isPixelSize) {
        font.setPixelSize(fontSize);
    }
    else {
        font.setPointSize(fontSize);
    }

    setTerminalFont(font);
    emit fontChanged(font);
}

void TerminalDialog::initColorizer(QMap<QString, QColor> colorMap)
{
    colorizer = new Colorizer(ui->consoleEditor->document());
    colorizer->init(colorMap);
}

void TerminalDialog::initCompleter(const QString& modelFileName)
{
    QPointer<CompleterLoader> loader = new CompleterLoader(modelFileName);
    completer = loader->completer;
    commandEdit->setCompleter(completer);

}

void TerminalDialog::setHistory(const QStringList &history)
{
    commandEdit->setHistory(history);
}

const QStringList &TerminalDialog::history()
{
    return commandEdit->history() ;
}

void TerminalDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers().testFlag(Qt::ControlModifier))
    {
        switch( e->key() )
        {
            case Qt::Key_Plus:
            case Qt::Key_Minus:
                changeFontSize( e->key());
                break;

        }
    }
}

void TerminalDialog::dataToConsole(const QString& data)
{
    ui->consoleEditor->insertPlainText(data);
    QTextCursor cursor = ui->consoleEditor->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->consoleEditor->setTextCursor(cursor);

}


QAbstractItemModel* TerminalDialog::completerModelFromFile(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly)) {
        return new QStringListModel(completer);
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

    QStandardItemModel* model = new QStandardItemModel(completer);
    QList<QStandardItem*> parents(10);
    parents[0] = model->invisibleRootItem();

    QRegularExpression re("^\\s+");

    while (!file.atEnd()) {
        const QString line = QString::fromUtf8(file.readLine());
        const QString trimmedLine = line.trimmed();

        if (trimmedLine.isEmpty()) {
            continue;
        }

        const QRegularExpressionMatch match = re.match(line);
        int nonws = match.capturedStart();
        int level = 0;

        if (nonws == -1) {
            level = 0;
        }
        else {
            const int capLen = match.capturedLength();
            level = capLen / 4;
        }

        if (level + 1 >= parents.size()) {
            parents.resize(parents.size() * 2);
        }

        QStandardItem* item = new QStandardItem;
        item->setText(trimmedLine);
        parents[level]->appendRow(item);
        parents[level + 1] = item;
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    return model;
}

void TerminalDialog::goToNextHighlight()
{
    int cursorIndex = m_textHighlighter.setNextMatchStateActive();
    consoleRefreshHighlighter(cursorIndex);
}

void TerminalDialog::goToPrevHighlight()
{
    int cursorIndex = m_textHighlighter.setPrevMatchStateActive();
    consoleRefreshHighlighter(cursorIndex);
}
