#ifndef CORE_H
#define CORE_H

#include "component.h"
#include "programcounter.h"
#include "instructionmemory.h"
#include "registerfile.h"
#include "alu.h"
#include "datamemory.h"


class Core : public Component
{
    Q_OBJECT

    QList< Instruction * > _instructions;
    ProgramCounter *_pc;
    InstructionMemory *_iMem;
    RegisterFile *_regFile;
    ALU *_ALU;
    DataMemory * _dMem;
    unsigned int _cycle;
    QList < Instruction *> _instrQueue;
    bool _if();
    bool _id(int);
    bool _ex(int);
    bool _mem(int);
    bool _wb(int);

public:
    Core(QObject * = 0);

    void run();

    void nextCycle();

    void executeCycle();

    void gotoCycle(unsigned int);

    unsigned int getCycle() const;

    DataMemory *getDataMemory() const;

    RegisterFile *getRegisterFile() const;

    QList< int > getRegistersDump(int, int) const;

    QList< int > getMemoryDump(int, int) const;

    ~Core();

signals:

public slots:

};

#endif // CORE_H
