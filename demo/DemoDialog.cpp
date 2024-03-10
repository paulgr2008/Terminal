#include "DemoDialog.h"
#include <QSettings>
#include <QFont>
#include <QPushButton>
#include <QToolButton>
#include <QFile>
#include <QApplication>


QFont loadFont(QSettings& settings, const QString& key)
{
    QFont defaultFont("Cascadia Mono", 11);
    if(settings.contains(key))
    {
        QFont font;
        font.fromString(settings.value(key).toString());
        return font;
    }
    else {
        // Setting not found, return the default value
        return defaultFont;
    }
}


DemoDialog::DemoDialog(QWidget* parent)
    : QMainWindow(parent)
{
    resize(640, 480);

    QCoreApplication::setApplicationVersion("1.0.0");
    QSettings settings("D:\\myapp.ini",
                       QSettings::IniFormat);


    m_terminalDialog = new TerminalDialog();
    QMap<QString, QColor> colorMap;
    colorMap.insert("(Pattern1|Pattern2)", Qt::red);
    colorMap.insert("(Pattern3|Pattern4)", Qt::yellow);
    m_terminalDialog->initColorizer(colorMap);
    m_terminalDialog->initCompleter(":/treeModel.txt");
    setCentralWidget(m_terminalDialog);
    connect(m_terminalDialog, &TerminalDialog::dataFromConsole, this, &DemoDialog::printDataFromConsole);
    connect(m_terminalDialog, &TerminalDialog::fontChanged, this, &DemoDialog::terminalFontChanged);
    //settings.setDefaultFormat(QSettings::IniFormat);
    QVariant from_store = settings.value("history");
    if (from_store.isValid())
    {
        m_terminalDialog->setHistory(from_store.toStringList());
    }

    QString data = "const Mnet_out_pres_t me_out_pres_lst_upper[] ={\n"
                   "  /*   0*/{ OpeM,                   0x1502,63, 57, DB5_PRO_UNITS_NO      }, /* TBL NO.8 */\n";

    m_terminalDialog->dataToConsole(data);
    QFont loadedFont = loadFont(settings, "font");
    m_terminalDialog->setTerminalFont(loadedFont);
    QFile file("D:\\XOL\\bitbucket\\CATools\\QuickInstall\\styles_copy.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    qApp->setStyleSheet(styleSheet);
    QList<QPushButton *> buttons = m_terminalDialog->findChildren<QPushButton *>();
    for(auto button: buttons){
        button->setProperty("configButton", true);
    }
   /*QList<QToolButton *> tbuttons = m_terminalDialog->findChildren<QToolButton *>();
    for(auto button: tbuttons){
        if(button->objectName()=="decreaseFontSzBtn")
        {
            button->setIcon(Q)
        }
    }*/

}



void DemoDialog::printDataFromConsole(const QString& data)
{
    qDebug() << "Data from console"<<data;
    qDebug() << "History to save"<<m_terminalDialog->history();
    QSettings settings("D:\\myapp.ini",
                       QSettings::IniFormat);
    settings.setValue("history", m_terminalDialog->history());
    settings.sync();

}

void DemoDialog::terminalFontChanged(QFont font)
{
    QSettings settings("D:\\myapp.ini",
                       QSettings::IniFormat);
    settings.setValue("font", font.toString());
    settings.sync();
}

DemoDialog::~DemoDialog()
{
}

