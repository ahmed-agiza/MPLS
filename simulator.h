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

public:
    Simulator(QObject * = 0, QStringList = QStringList());

    bool parseInstructions();

    void simulate();

    ~Simulator();

signals:

public slots:

};

#endif // SIMULATOR_H
