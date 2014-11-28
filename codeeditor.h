#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QScroller>
#include "syntaxhl.h"

class CodeEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    void setOpened();
    QStringList getLabels();

private:
    SyntaxHL *SHL;
    QCompleter *codeCompleter;
    void focusInEvent(QFocusEvent *);
    void keyPressEvent(QKeyEvent *);
    QTextCursor deleteCurrentLine();
    QString getCurrentLine();
    QTextCursor getSelectedLines();
    QStringListModel *model;
    QStringList compList;

protected:
    void scrollContentsBy(int, int);

signals:


public slots:
    void deleteSelection();
    void deleteLine();
    void moveLineUp();
    void moveLineDown();
    void copyLineUp();
    void copyLineDown();
    void popupSuggestions();
    void toggleComments();



private slots:
    void insertCompletion(QString completion);
    void completerPop();
    void highlightLine();
    void selectLine(int);


};

#endif // CODEEDITOR_H
