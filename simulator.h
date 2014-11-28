#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include "core.h"

class Simulator : public QObject
{
    Q_OBJECT

    QStringList _rawInstructions;
    QList<Instruction *> _instructions;
    Core *_core;
    bool _valid;
    QMap< QString, InstructionName> _instructionNames;
    int getRegisterNumber(QString s);

    QString getInstructionRegex(QString);
public:
    Simulator(QObject * = 0, QStringList = QStringList());

    bool parseInstructions();

    bool isReady() const;

    bool isComplete() const;

    void simulate();

    void nextCycle();

    int getCurrentCycle() const;

    Core *getCore() const;

    ~Simulator();

    void setValid(bool valid);

    void initializeInstuctionNamesMap();

    static int getNumber(QString);
signals:
    void ready(bool);

public slots:

};

#endif // SIMULATOR_H
