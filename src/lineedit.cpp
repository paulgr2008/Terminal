#include "lineedit.h"
#include <QEvent>
#include <QKeyEvent>

#define MAX_HISTORY_SIZE 5

LineEdit::LineEdit(QWidget *parent):
    QLineEdit(parent){

    connect(this,SIGNAL(returnPressed()),SLOT(execute()));
}

bool LineEdit::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if( ke->matches(QKeySequence::Undo) || ke->key() == Qt::Key_Up )
        {
            back();
            return true;
        }
        else if( ke->matches(QKeySequence::Redo) || ke->key() == Qt::Key_Down )
        {
            forward();
            return true;
        }
        else if(ke->modifiers().testFlag(Qt::ControlModifier))
        {
            switch(ke->key())
            {
                case Qt::Key_Plus:
                case Qt::Key_Minus:
                case Qt::Key_N:
                case Qt::Key_P:
                    ke->ignore();
                    return false;

            }
        }

    }
    return QLineEdit::event(event);
}

void LineEdit::setHistory(const QStringList &history)
{
    _history = history;
    historyPosition = _history.length()-1;
}

void LineEdit::back()
{
    if ( _history.empty() )
        return;

    if ( !text().isEmpty() &&
        ( historyPosition >= _history.size() || text() != _history[historyPosition] ) )
        unfinished = text();

    if ( historyPosition > 0 )
        historyPosition--;

    setText(_history[historyPosition]);
}


void LineEdit::forward()
{
    if ( _history.empty() )
        return;

    historyPosition++;

    if ( historyPosition >= _history.size() )
    {
        setText(unfinished);
        unfinished = "";
        historyPosition = _history.size();
    }
    else
        setText(_history[historyPosition]);
}

void LineEdit::execute()
{
    if ( _history.empty() || _history.last() != text())
    {
        if(_history.size() >= MAX_HISTORY_SIZE)
            _history.removeFirst();
        _history << text();
    }

    historyPosition = _history.size();

    clear();
    emit lineExecuted(_history.last());
}

