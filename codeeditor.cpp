#include "codeeditor.h"
#include <QDebug>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QListView>
#include <QTextDocumentFragment>
#include <QScrollBar>





CodeEditor::CodeEditor(QWidget *parent) :
    QTextEdit(parent){


    setAcceptRichText(false);
    setTabStopWidth(40);
    SHL = new SyntaxHL(this);
    compList << "ADD" << "ADDI" << "XOR" << "LW" << "SW" << "BLE" << "J" << "SLT" << "JAL" << "JR";


    compList << "$zero"
             << "$at"
             << "$v0"
             << "$v1"
             <<"$a0"
             << "$a1"
             << "$a2"
             << "$a3"
             << "$t0"
             << "$t1"
             << "$t2"
             << "$t3"
             << "$t4"
             << "$t5"
             << "$t6"
             << "$t7"
             << "$s0"
             << "$s1"
             << "$s2"
             << "$s3"
             << "$s4"
             << "$s5"
             << "$s6"
             << "$s7"
             << "$t8"
             << "$t9"
             << "$gp"
             << "$fp"
             << "$ra"
             << "$sp";



    model = new QStringListModel(compList, this);

    codeCompleter = new QCompleter(model, this);
    codeCompleter->setCompletionMode(QCompleter::PopupCompletion);
    codeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    codeCompleter->setWidget(this);

    QObject::connect(codeCompleter, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(completerPop()));    
    QObject::connect(this, SIGNAL(selectionChanged()), this, SLOT(highlightLine()));
    QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightLine()));

}



void CodeEditor::focusInEvent(QFocusEvent *e){
    if (codeCompleter)
        codeCompleter->setWidget(this);
    QTextEdit::focusInEvent(e);
}





void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if ((codeCompleter) && codeCompleter->popup()->isVisible() && (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Backtab || e->key() == Qt::Key_Escape|| e->key() == Qt::Key_Return || e->key() == Qt::Key_Tab))
    {
        e->ignore();
        return;
    }
    else if (e->key() == Qt::Key_Tab)
    {
        QTextCursor cur = textCursor();
        cur.beginEditBlock();
        cur.insertText("\t");
        cur.endEditBlock();
        setTextCursor(cur);
        e->ignore();
        return;
    }
    else if (e->key() == Qt::Key_D && (e->modifiers() & Qt::ControlModifier)){
        deleteLine();
    } else if (e->key() == Qt::Key_Up && (e->modifiers() & Qt::AltModifier)){
        copyLineUp();
    }else if (e->key() == Qt::Key_Up && (e->modifiers() & Qt::ControlModifier)) {
        moveLineUp();
    } else if (e->key() == Qt::Key_Down && (e->modifiers() & Qt::AltModifier)){
        copyLineDown();
    }else if (e->key() == Qt::Key_Down && (e->modifiers() & Qt::ControlModifier)){
        moveLineDown();
    }else if (e->key() == Qt::Key_Space &&(e->modifiers() & Qt::ControlModifier)){
        popupSuggestions();

    }else if (e->key() == Qt::Key_3 && (e->modifiers() & Qt::ControlModifier)){
        toggleComments();
    }else if(e->key() == Qt::Key_Period){
         popupSuggestions();
         QTextEdit::keyPressEvent(e);
    }else
        QTextEdit::keyPressEvent(e);


}

QTextCursor CodeEditor::deleteCurrentLine()
{
    QTextCursor currentPos = textCursor();
    textCursor().beginEditBlock();
    currentPos.select(QTextCursor::BlockUnderCursor);

    if (currentPos.selectedText() == ""){
        if (currentPos.selectionStart() == 0){
            currentPos.deleteChar();
        }else{
            currentPos.deletePreviousChar();
        }
        currentPos.removeSelectedText();
    }
    else{
        currentPos.removeSelectedText();
        if (currentPos.selectionStart() == 0)
            currentPos.deleteChar();
    }
    return currentPos;
}

QString CodeEditor::getCurrentLine(){
    QTextCursor currentPos = textCursor();
    currentPos.select(QTextCursor::LineUnderCursor);
    return currentPos.selectedText();
}

QTextCursor CodeEditor::getSelectedLines(){
    QTextCursor startCursor = textCursor();
    QTextCursor endCursor = textCursor();
    int selStart = startCursor.selectionStart();
    int selEnd = endCursor.selectionEnd();
    startCursor.setPosition(selStart);
    endCursor.setPosition(selEnd);
    startCursor.movePosition(QTextCursor::StartOfLine);
    endCursor.movePosition(QTextCursor::EndOfLine);
    QTextCursor selectionCursor = textCursor();
    selectionCursor.clearSelection();
    selectionCursor.setPosition(startCursor.position());
    selectionCursor.setPosition(endCursor.position(), QTextCursor::KeepAnchor);
    return selectionCursor;
}


void CodeEditor::scrollContentsBy(int x, int y)
{
    QTextEdit::scrollContentsBy(x, y);
}

void CodeEditor::deleteSelection(){
    QTextCursor currentPos = textCursor();
    currentPos.removeSelectedText();

}

void CodeEditor::deleteLine(){
    QTextCursor selection = getSelectedLines();
    selection.beginEditBlock();
    selection.removeSelectedText();
    selection.deleteChar();
    selection.endEditBlock();
}

void CodeEditor::moveLineUp(){
    QTextCursor selectionCursor = getSelectedLines();
    QString selectedLines = selectionCursor.selection().toPlainText();
    selectionCursor.beginEditBlock();
    selectionCursor.removeSelectedText();
    selectionCursor.deleteChar();
    selectionCursor.movePosition(QTextCursor::Up);
    selectionCursor.insertText(QString(selectedLines + "\n"));
    selectionCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, selectedLines.split("\n").size());
    selectionCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, selectedLines.size());
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
    if(codeCompleter->popup()->isVisible())
        codeCompleter->popup()->hide();
}

void CodeEditor::moveLineDown(){
    QTextCursor selectionCursor = getSelectedLines();
    QString selectedLines = selectionCursor.selection().toPlainText();
    selectionCursor.beginEditBlock();
    selectionCursor.removeSelectedText();
    selectionCursor.deleteChar();
    selectionCursor.movePosition(QTextCursor::EndOfLine);
    selectionCursor.insertText(QString("\n" + selectedLines));
    selectionCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, selectedLines.size());
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
    if(codeCompleter->popup()->isVisible())
        codeCompleter->popup()->hide();

}

void CodeEditor::copyLineUp(){
    QTextCursor selectionCursor = getSelectedLines();
    QString selectedLines = selectionCursor.selection().toPlainText();
    selectionCursor.beginEditBlock();
    selectionCursor.setPosition(selectionCursor.selectionStart());
    selectionCursor.movePosition(QTextCursor::StartOfLine);
    selectionCursor.insertText(QString(selectedLines + "\n"));
    selectionCursor.movePosition(QTextCursor::PreviousCharacter);
    selectionCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, selectedLines.size());
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
    if(codeCompleter->popup()->isVisible())
        codeCompleter->popup()->hide();
}

void CodeEditor::copyLineDown(){
    QTextCursor selectionCursor = getSelectedLines();
    QString selectedLines = selectionCursor.selection().toPlainText();
    selectionCursor.beginEditBlock();
    selectionCursor.movePosition(QTextCursor::EndOfLine);
    selectionCursor.insertText(QString("\n" + selectedLines));
    selectionCursor.movePosition(QTextCursor::EndOfBlock);
    selectionCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, selectedLines.size());
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
    if(codeCompleter->popup()->isVisible())
        codeCompleter->popup()->hide();
}

void CodeEditor::popupSuggestions(){
    completerPop();
}

void CodeEditor::toggleComments(){
    QTextCursor selectionCursor = getSelectedLines();
    QStringList selectedLines = selectionCursor.selection().toPlainText().split("\n");
    selectionCursor.beginEditBlock();
    selectionCursor.removeSelectedText();
    selectionCursor.deleteChar();
    for (int i = 0; i < selectedLines.size(); i++){
        if (selectedLines.at(i).trimmed().size() > 0){
            if(!selectedLines.at(i).startsWith("#"))
                selectedLines[i] = "#" + selectedLines[i];
            else
                selectedLines[i].remove(0, 1);
        }
    }
    int contentCount = 0;
    foreach(QString line, selectedLines){
        selectionCursor.insertText(line + "\n");
        contentCount += line.size() + 1;
    }


    selectionCursor.movePosition(QTextCursor::PreviousCharacter);
    selectionCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, contentCount);
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
}

void CodeEditor::insertCompletion(QString completion){
    QTextCursor currentPos = textCursor();
    int compLength = completion.length() - codeCompleter->completionPrefix().length();
    currentPos.movePosition(QTextCursor::Left);
    currentPos.movePosition(QTextCursor::EndOfWord);
    currentPos.insertText(completion.right(compLength));
    setTextCursor(currentPos);
    codeCompleter->popup()->hide();
}

void CodeEditor::highlightLine(){

    QList<QTextEdit::ExtraSelection> linesHL;
    QTextEdit::ExtraSelection lineHL;
    lineHL.format.setBackground(QColor(Qt::lightGray).darker(180));
    lineHL.format.setProperty(QTextFormat::FullWidthSelection, true);
    lineHL.cursor = textCursor();

    lineHL.cursor.clearSelection();
    linesHL.append(lineHL);
    setExtraSelections(linesHL);
}





void CodeEditor::selectLine(int num){
    QTextCursor cur = textCursor();
    cur.clearSelection();
    cur.setPosition(0);
    cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, num);
    cur.select(QTextCursor::LineUnderCursor);
    setTextCursor(cur);
}



void CodeEditor::completerPop()
{
    QTextCursor sel = textCursor();
    sel.select(QTextCursor::WordUnderCursor);
    QTextCursor sel2 = textCursor();
    sel2.select(QTextCursor::LineUnderCursor);
    if (sel2.selection().toPlainText().trimmed().startsWith("#")){
        codeCompleter->popup()->hide();
        return;
    }
    if (sel2.selection().toPlainText().contains("\"") && sel2.selection().toPlainText().count("\"") % 2 != 0){
        QTextCursor stringChecker(sel);
        stringChecker.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        QString firstHalf = sel2.selection().toPlainText().mid(0, stringChecker.selectionStart());
        if (firstHalf.count("\"")%2 != 0){
            codeCompleter->popup()->hide();
            return;
        }


    }


    bool regS = (sel2.selectedText().mid(sel2.selectedText().lastIndexOf(QRegExp("( |,)+")) + 1, 1) == "$");

    QTextCursor tempCursor = textCursor();
    tempCursor.select(QTextCursor::WordUnderCursor);
    if (tempCursor.selectionStart() != 0){
        int length = tempCursor.selection().toPlainText().length();
        tempCursor.setPosition(tempCursor.selectionStart() - 1);
        tempCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, length == 0? 1:length);
    }

    QRegExp directivesRegEx("\\.\\w*");
    directivesRegEx.setPatternSyntax(QRegExp::RegExp2);



    bool dirS = tempCursor.selectedText().lastIndexOf(directivesRegEx) != -1;


    if (regS)
        codeCompleter->setCompletionPrefix("$" + sel.selectedText());
    else if (dirS){
        codeCompleter->setCompletionPrefix("." + sel.selectedText());
    }else
        codeCompleter->setCompletionPrefix(sel.selectedText());


    QString currenCom = codeCompleter->completionModel()->data(codeCompleter->model()->index(0, 0)).toString();


    if(sel.selectedText().length() > 0 || regS || dirS)
    {
        if ((codeCompleter->completionCount() != 1) || (currenCom != sel.selectedText() && !regS && !dirS) || (regS && currenCom != "$" + sel.selectedText()) || (dirS && currenCom != "." + sel.selectedText()))
        {
            QRect popRect = this->cursorRect();
            popRect.setWidth(50);
            codeCompleter->complete(popRect);
            codeCompleter->setCurrentRow(3);
            codeCompleter->popup()->setCurrentIndex(codeCompleter->popup()->indexAt(QPoint(0, 0)));

        }
        else
            codeCompleter->popup()->hide();
    }
    else
        codeCompleter->popup()->hide();


}


void CodeEditor::setOpened(){
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    setTextCursor(cursor);
    codeCompleter->popup()->hide();
}



