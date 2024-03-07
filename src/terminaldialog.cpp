#include "terminaldialog.h"
#include "ui_terminaldialog.h"
#include <QFontDialog>
#include <QFile>
#include <QStringListModel>
#include <QStandardItemModel>
#include "completerloader.h"
#include "colorizer.h"
#include "treemodelcompleter.h"

TerminalDialog::TerminalDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::TerminalDialog)
{
    ui->setupUi(this);
    createConnections();
    m_textHighlighter.setDocument(ui->consoleEditor->document());
    QFont deffont("Cascadia Mono", 11);
    setConsoleFont(deffont);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    p.setColor(QPalette::Highlight, Qt::gray);
    p.setColor(QPalette::HighlightedText, Qt::white);
    ui->consoleEditor->setPalette(p);

    completer = new TreeModelCompleter(this);
    toggleFindEditor(false);
    toggleFontEditor(false);
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
    connect(ui->nextButton, &QPushButton::clicked, this, &TerminalDialog::goToNextHighlight, Qt::DirectConnection);
    connect(ui->prevButton, &QPushButton::clicked, this, &TerminalDialog::goToPrevHighlight, Qt::DirectConnection);
    connect(ui->autocomplChBox, &QCheckBox::clicked,
            this, &TerminalDialog::manageAutocompleter);
    connect(ui->fullScreenButton, &QPushButton::clicked,
            this, &TerminalDialog::consoleBtnManager);
    connect(ui->cleanButton, &QPushButton::clicked,
            this, &TerminalDialog::cleanConsole);

    connect(ui->changeFontButton, &QPushButton::clicked,
            this, &TerminalDialog::fontButtonClicked);
    connect(ui->increaseFontSzBtn, &QPushButton::clicked,
            this, &TerminalDialog::fontButtonClicked);
    connect(ui->decreaseFontSzBtn, &QPushButton::clicked,
            this, &TerminalDialog::fontButtonClicked);
    connect(ui->commandEditor, &QLineEdit::returnPressed, this, &TerminalDialog::finishCommandEditing);
    connect(ui->findToolButton, &QToolButton::clicked, this, &TerminalDialog::toggleFindEditor);
    connect(ui->fontToolButton, &QToolButton::clicked, this, &TerminalDialog::toggleFontEditor);

}


void TerminalDialog::finishCommandEditing()
{
    emit dataFromConsole(ui->commandEditor->text());
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
    ui->commandEditor->setCompleter((state) ? completer : nullptr);
}

void TerminalDialog::closeHotKeySignalManager()
{

}

void TerminalDialog::fontButtonClicked()
{
    QToolButton* button = static_cast<QToolButton*>(this->sender());

    if (button == ui->changeFontButton)
        setConsoleFont(QFontDialog::getFont(0, ui->consoleEditor->font(), NULL, QString(),
                                            QFontDialog::MonospacedFonts));
    else if (button == ui->decreaseFontSzBtn) {
        changeFontSize(Qt::Key_Minus);
    }
    else if (button == ui->increaseFontSzBtn) {
        changeFontSize(Qt::Key_Plus);
    }
}

void TerminalDialog::setConsoleFont(QFont font)
{
    ui->consoleEditor->document()->setDefaultFont(font);
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

    ui->consoleEditor->document()->setDefaultFont(font);
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
    ui->commandEditor->setCompleter(completer);

}

void TerminalDialog::dataToConsole(const QString& data)
{
    ui->consoleEditor->insertPlainText(data);
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
