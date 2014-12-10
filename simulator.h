#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include "core.h"

class Simulator : public QObject
{
    Q_OBJECT

    class _MissingLabel{
    private:
        QString _label;
        int _address;
        int _lineNumber;
    public:
        _MissingLabel(QString label, int address, int lineNumber): _label(label), _address(address), _lineNumber(lineNumber){}
        QString getLabel() const{return _label;}
        int getAddress() const{return _address;}
        int getLineNumber() const{return _lineNumber;}
    };

    class _ParsingError{
    private:
        QString _errorMessage;
        int _lineNumber;
    public:
        _ParsingError(QString errorMessage, int lineNumber): _errorMessage(errorMessage), _lineNumber(lineNumber){}
        QString getErrorMessage() const{return _errorMessage;}
        int getLineNumber() const{return _lineNumber;}
        operator QString(){
            return toString();
        }
        QString toString() const{
            QString result = QString::number(_lineNumber);
            result += ":  ";
            result += _errorMessage;
            return result;
        }
    };

    QStringList _rawInstructions;
    QList<Instruction *> _instructions;
    Core *_core;
    bool _valid;
    QMap< QString, InstructionName> _instructionNames;
    int getRegisterNumber(QString s);

    QString getInstructionRegex(QString);
    QList<_ParsingError> _errorList;

    friend class MainWindow;
public:
    Simulator(QObject * = 0, QStringList = QStringList());

    bool parseInstructions();

    bool isReady() const;

    bool isComplete() const;

    void simulate();

    void nextCycle();

    int getCurrentCycle() const;

    Core *getCore() const;

    QStringList getErrors() const;

    ~Simulator();

    void setValid(bool valid);

    void initializeInstuctionNamesMap();

    static int getNumber(QString);
signals:
    void ready(bool);

public slots:

};

#endif // SIMULATOR_H
