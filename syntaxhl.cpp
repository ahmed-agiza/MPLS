#include "syntaxhl.h"
#include <QDebug>

//    add
//    addu
//    sub
//    subu
//    and
//    or
//    nor
//    xor
//    srlv
//    sllv
//    srav
//    slt
//    sltu


//    addi
//    addiu
//    andi
//    ori
//    nori
//    xori
//    srl
//    sll
//    sra
//    slti
//    sltiu
//    beq
//    bne


//  instruction $register, $register, label
//      beq
//      bne


//  instruction $register, immediate
//      lui


//  instruction $register
//      jr
//      jalr
//      mfhi
//      mflo
//      mtlo
//      mthi


//  instruction $register, $register
//      mult
//      multu
//      div
//      divu


//  instruction label
//      j
//      jal


//  instruction
//      syscall
//      nop

void SyntaxHL::init()
{
    instructionFormat.setForeground(QColor(Qt::blue).lighter(150));
    instructionFormat.setFontWeight(QFont::Bold);
    registerFormat.setForeground(Qt::yellow);
    registerFormat.setFontWeight(QFont::Bold);
    commentFormat.setForeground(Qt::green);
    stringFormat.setForeground(QColor(Qt::darkGreen).lighter(190));
    labelFormat.setForeground(Qt::cyan);
    macroFormat.setForeground(QColor(Qt::cyan).lighter(80));
    macroFormat.setFontItalic(true);
    immFormat.setForeground(QColor(Qt::white).darker(110));
    immFormat.setFontWeight(QFont::Bold);
    openStringFormat.setForeground(QColor(Qt::darkGreen).lighter(310));

    //Instructions Syntax

    syntax tempSyn;
    tempSyn.labelSyntax = false;




    tempSyn.pattern = QRegExp("\\b0x[0-9a-fA-F]+\\b|\\b[\\-\\d]+\\b|\\b0b[01]+\\b"); tempSyn.format = immFormat;
    syntaxes.append(tempSyn);

    instructionsList << "ADD" << "ADDI" << "XOR" << "LW" << "SW" << "BLE" << "J" << "SLT" << "JAL" << "JR";


    foreach(QString pattern, instructionsList)
    {
        tempSyn.pattern = QRegExp(QString("\\b" + pattern + "\\b"), Qt::CaseInsensitive); tempSyn.format = instructionFormat;
        syntaxes.append(tempSyn);
    }


    //Registers syntax.
    for (int i = 0; i < 32; i++)
         registersList.append(QString("\\$" + QString::number(i)));
    registersList << "\\$zero" << "\\$at" << "\\$v0" << "\\$v1" <<"\\$a0" << "\\$a1" << "\\$a2" << "\\$a3"
                  << "\\$t0" << "\\$t1" << "\\$t2" << "\\$t3" << "\\$t4" << "\\$t5" << "\\$t6" << "\\$t7"
                  << "\\$s0" << "\\$s1" << "\\$s2" << "\\$s3" << "\\$s4" << "\\$s5" << "\\$s6" << "\\$s7"
                  << "\\$t8" << "\\$t9" << "\\$gp" << "\\$fp" << "\\$ra" << "\\$sp";



    foreach(QString pattern, registersList)
    {
        tempSyn.pattern = QRegExp(pattern, Qt::CaseInsensitive); tempSyn.format = registerFormat;
        syntaxes.append(tempSyn);
    }

    //Directive's syntax
    macrosList << "\\.align" << "\\.ascii" << "\\.asciiz" << "\\.byte" << "\\.double" <<"\\.float" << "\\.half" << "\\.space" << "\\.word" << "\\.text" << "\\.data" << "\\.include";

    foreach(QString pattern, macrosList)
    {
        tempSyn.pattern = QRegExp(pattern, Qt::CaseInsensitive); tempSyn.format = macroFormat;
        syntaxes.append(tempSyn);
    }

    //Label's syntax.
    tempSyn.pattern = QRegExp("\\w+:"); tempSyn.format = labelFormat;
    syntaxes.append(tempSyn);

    //String's syntax
    tempSyn.pattern = QRegExp("\\\".*\\\"", Qt::CaseInsensitive);
    tempSyn.format = stringFormat;
    syntaxes.append(tempSyn);



    //Comment's syntax.
    tempSyn.pattern = QRegExp("#[^\n]*"); tempSyn.format = commentFormat;
    syntaxes.append(tempSyn);
}

SyntaxHL::SyntaxHL(QTextEdit *parent) :
    QSyntaxHighlighter(parent)
{
   init();
}

SyntaxHL::SyntaxHL(QObject *parent):QSyntaxHighlighter(parent){
    init();
}

SyntaxHL::SyntaxHL(QTextDocument *parent):QSyntaxHighlighter(parent){
    init();
}

void SyntaxHL::setLabelsList(QStringList &labels){
    for (int i = 0; i < syntaxes.size(); i++)
        if (syntaxes.at(i).labelSyntax)
            syntaxes.removeAt(i--);

    syntax tempSyn;
    tempSyn.labelSyntax = true;
    tempSyn.format = labelFormat;
    foreach(QString label, labels){
        tempSyn.pattern = QRegExp(QString("\\b" + label + "\\b"));
        syntaxes.prepend(tempSyn);
    }

}

void SyntaxHL::highlightBlock(const QString &text)
{
    foreach (syntax tempSyn, syntaxes)
    {
             QRegExp formatRegEx(tempSyn.pattern);
             int index = formatRegEx.indexIn(text);

             while (index >= 0) {
                 int length = formatRegEx.matchedLength();
                // if (tempSyn.format == labelFormat)
                    // qDebug() << "Found label: " << text.mid(index, length);
                 setFormat(index, length, tempSyn.format);
                 index = formatRegEx.indexIn(text, index + length);                
             }
   }
   if (text.contains("\"")){
       if ((text.count("\"")%2) != 0){
           int pos = text.lastIndexOf("\"");
           setFormat(pos, text.length() - pos, openStringFormat);
       }
   }
}
