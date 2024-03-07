#include <QApplication>
#include <QFile>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QRegularExpression>
#include "completerloader.h"
#include "treemodelcompleter.h"

CompleterLoader::CompleterLoader(const QString& modelFileName,
                                 const QString& sep, QObject* parent)
    : QObject(parent)
{
    completer = new TreeModelCompleter(this);
    completer->setSeparator(sep);
    completer->setModel(modelFromFile(modelFileName));
    completer->setModelSorting(QCompleter::UnsortedModel);
    completer->setCaseSensitivity(Qt::CaseSensitive);
    completer->setWrapAround(true);
    completer->setCompletionMode(QCompleter::PopupCompletion);
}

QAbstractItemModel* CompleterLoader::modelFromFile(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly)) {
        return new QStringListModel(completer);
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
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
    QApplication::restoreOverrideCursor();
#endif

    return model;
}



