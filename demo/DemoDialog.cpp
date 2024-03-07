#include "DemoDialog.h"

DemoDialog::DemoDialog(QWidget* parent)
    : QMainWindow(parent)
{
    resize(640, 480);
    m_terminalDialog = new TerminalDialog();
    QMap<QString, QColor> colorMap;
    colorMap.insert("(Pattern1|Pattern2)", Qt::red);
    colorMap.insert("(Pattern3|Pattern4)", Qt::yellow);
    m_terminalDialog->initColorizer(colorMap);
    m_terminalDialog->initCompleter(":/treeModel.txt");
    setCentralWidget(m_terminalDialog);
    connect(m_terminalDialog, &TerminalDialog::dataFromConsole, this, &DemoDialog::printDataFromConsole);
    QString data = "const Mnet_out_pres_t me_out_pres_lst_upper[] ={\n"
                   "  /*   0*/{ OpeM,                   0x1502,63, 57, DB5_PRO_UNITS_NO      }, /* TBL NO.8 */\n";

    m_terminalDialog->dataToConsole(data);
}

void DemoDialog::printDataFromConsole(const QString& data)
{
    qDebug() << data;
}

DemoDialog::~DemoDialog()
{
}

