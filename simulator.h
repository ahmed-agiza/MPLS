#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QDebug>
#include "core.h"

class Simulator : public QObject
{
    Q_OBJECT

    QStringList _rawInstructions;
    QList<Instruction *> _instructions;
    Core *_core;
    bool _valid;

public:
    Simulator(QObject * = 0, QStringList = QStringList());

    bool parseInstructions();

    bool isReady() const;

    bool isComplete() const;

    void simulate();

    void nextCycle();

    int getCurrentCycle() const;

    ~Simulator();

    void setValid(bool valid);

signals:
    void ready(bool);

public slots:

};

#endif // SIMULATOR_H
