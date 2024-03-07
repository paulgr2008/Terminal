#ifndef COLORIZER_H
#define COLORIZER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QMap>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Colorizer: public QSyntaxHighlighter {
        Q_OBJECT

    public:
        explicit Colorizer(QTextDocument* parent = 0);
        void init(QMap<QString, QColor> patterns);

    protected:
        void highlightBlock(const QString& text) Q_DECL_OVERRIDE;
    private:
        struct HighlightingRule {
            QRegularExpression pattern;
            QTextCharFormat format;
        };
        QVector<HighlightingRule> highlightingRules;

        int index;
        int length;
        QString word;
};

#endif // COLORIZER_H
