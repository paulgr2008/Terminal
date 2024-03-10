#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QWidget* parent = 0);
    bool event(QEvent *event);

    int historySize() const { return _history.size(); }
    const QStringList &history() const { return _history; }
    void setHistory(const QStringList& history);

public slots:
    void execute();

signals:
    void lineExecuted(QString);

private:
    void back();
    void forward();
    QString     unfinished;
    QStringList _history;
    int         historyPosition;


};

#endif // LINEEDIT_H
