#ifndef CONSOLECOMPLETERLOADER_H
#define CONSOLECOMPLETERLOADER_H

#include <QObject>
#include <QModelIndex>
QT_BEGIN_NAMESPACE
class TreeModelCompleter;
class QAbstractItemModel;
QT_END_NAMESPACE

class CompleterLoader : public QObject {
        Q_OBJECT
    public:
        explicit CompleterLoader(const QString& modelFileName, const QString& sep = QString(" "), QObject* parent = 0);
        TreeModelCompleter* completer;
    signals:

    public slots:

    private:
        QAbstractItemModel* modelFromFile(const QString& fileName);
};

#endif // CONSOLECOMPLETERLOADER_H
