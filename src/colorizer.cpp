#include "colorizer.h"
#include <QDebug>
#include <QRegularExpression>


Colorizer::Colorizer(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{

}

void Colorizer::init(QMap<QString, QColor> patterns)
{
    QMapIterator<QString, QColor> i(patterns);

    while (i.hasNext()) {
        i.next();
        QTextCharFormat coloredFormat;
        coloredFormat.setFontWeight(QFont::Bold);
        coloredFormat.setForeground(i.value());
        HighlightingRule rule;
        rule.pattern = QRegularExpression(i.key());
        rule.format = coloredFormat;
        highlightingRules.append(rule);
    }
}


void Colorizer::highlightBlock(const QString& text)
{
    foreach (const HighlightingRule& rule, highlightingRules) {
        const QRegularExpression expression(rule.pattern);

        QRegularExpressionMatchIterator i = expression.globalMatch(text);

        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();

            if (match.hasMatch()) {
                int length = match.capturedLength();
                int index = match.lastCapturedIndex() - 1;
                setFormat(index, length, rule.format);
            }
        }
    }

    setCurrentBlockState(0);
}


