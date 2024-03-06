#include "consoledialog.h"
#include "ui_consoledialog.h"
#include <QFontDialog>

ConsoleDialog::ConsoleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConsoleDialog)
{
    ui->setupUi(this);
    createConnections();
    m_textHighlighter.setDocument(ui->consoleEditor->document());
    //QSettings settings;

    QFont deffont("Cascadia Mono",11);
    //QString fontKey = settings.value("console/font", deffont).toString();
    //QFont font;
    //font.fromString(deffont);
    setConsoleFont(deffont);
    QPalette p = palette();
    p.setColor( QPalette::Base, Qt::black );
    p.setColor( QPalette::Text, Qt::green );
    p.setColor(QPalette::Highlight, Qt::gray);
    p.setColor(QPalette::HighlightedText, Qt::white);
    ui->consoleEditor->setPalette(p);
}

ConsoleDialog::~ConsoleDialog()
{
    delete ui;
}

void ConsoleDialog::createConnections()
{
    connect(ui->searchEditor, &QLineEdit::textChanged, this, &ConsoleDialog::highlightText, Qt::DirectConnection);
    //connect(ui->consoleEditor, &QPlainTextEdit::textChanged, this, &ConsoleDialog::highlightText, Qt::DirectConnection);
    connect(ui->nextButton, &QPushButton::clicked, this, &ConsoleDialog::goToNextHighlight, Qt::DirectConnection);
    connect(ui->prevButton, &QPushButton::clicked, this, &ConsoleDialog::goToPrevHighlight, Qt::DirectConnection);
    connect(ui->autocomplChBox, &QCheckBox::stateChanged,
            this, &ConsoleDialog::manageAutocompleter);
    connect(ui->fullScreenButton, &QPushButton::clicked,
            this, &ConsoleDialog::consoleBtnManager);
    connect(ui->cleanButton, &QPushButton::clicked,
            this, &ConsoleDialog::cleanConsole);

    connect(ui->changeFontButton, &QPushButton::clicked,
            this, &ConsoleDialog::fontButtonClicked);
    connect(ui->increaseFontSzBtn, &QPushButton::clicked,
            this, &ConsoleDialog::fontButtonClicked);
    connect(ui->decreaseFontSzBtn, &QPushButton::clicked,
            this, &ConsoleDialog::fontButtonClicked);
}

void ConsoleDialog::cleanConsole()
{
    ui->consoleEditor->clear();
    ui->consoleEditor->setFocus();
    //emit writeData("\n");
}

void ConsoleDialog::highlightText()
{
    /*if(m_findWidget.isVisible()){
        m_textHighlighter.setWordPattern(m_findTextLineEdit.text());
    }else{
        m_textHighlighter.setWordPattern("");
    }*/

    // For some reason rehighlighting from the
    // context of the QTextEdit itself
    // will cause a recursive infinite loop...
    m_textHighlighter.setWordPattern(ui->searchEditor->text());
    m_textHighlighter.customRehighlight();


}

void ConsoleDialog::consoleRefreshHighlighter(int cursorIndex){
    if(cursorIndex >= 0){
        QTextCursor currentCursor = ui->consoleEditor->textCursor();
        currentCursor.setPosition(cursorIndex);
        ui->consoleEditor->setTextCursor(currentCursor);
        ui->consoleEditor->ensureCursorVisible();
    }
}

void ConsoleDialog::consoleBtnManager()
{

}

void ConsoleDialog::manageAutocompleter(int state)
{

}

void ConsoleDialog::closeHotKeySignalManager()
{

}

void ConsoleDialog::fontButtonClicked()
{
    QToolButton* button = static_cast<QToolButton*> (this->sender());
    if( button == ui->changeFontButton )
        setConsoleFont(QFontDialog::getFont(0, ui->consoleEditor->font(),NULL,QString(),
                                                     QFontDialog::MonospacedFonts));
    else if( button == ui->decreaseFontSzBtn )
        changeFontSize(Qt::Key_Minus);
    else if( button == ui->increaseFontSzBtn )
        changeFontSize(Qt::Key_Plus);
}

void ConsoleDialog::setConsoleFont(QFont font)
{
    ui->consoleEditor->document()->setDefaultFont(font);
}

void ConsoleDialog::changeFontSize(int hotKey)
{
    bool isPixelSize = false;
    QFont font =  ui->consoleEditor->document()->defaultFont();//this->font();
    int fontSize = font.pointSize();
    if( fontSize<0 )
    {
        fontSize = font.pixelSize();
        isPixelSize = true;
    }
    if( hotKey == Qt::Key_Plus )
        fontSize++;
    else if( hotKey == Qt::Key_Minus )
        fontSize--;

    if( isPixelSize )
        font.setPixelSize(fontSize);
    else
        font.setPointSize(fontSize);
    ui->consoleEditor->document()->setDefaultFont(font);
}

void ConsoleDialog::goToNextHighlight()
{
    int cursorIndex = m_textHighlighter.setNextMatchStateActive();
    consoleRefreshHighlighter(cursorIndex);
}

void ConsoleDialog::goToPrevHighlight()
{
    int cursorIndex = m_textHighlighter.setPrevMatchStateActive();
    consoleRefreshHighlighter(cursorIndex);
}
