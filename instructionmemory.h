#ifndef INSTRUCTIONMEMORY_H
#define INSTRUCTIONMEMORY_H

#include "component.h"
#include "programcounter.h"
#include "instruction.h"
#include "ifidbuffer.h"

class InstructionMemory : public Component
{
    Q_OBJECT

    QList< Instruction * > _instructions;
    ProgramCounter *_pc;
    IFIDBuffer *_buffer;
public:
    InstructionMemory(QObject * = 0, Component * = 0, QList< Instruction * > = QList< Instruction * >(), ProgramCounter * = 0);

    Instruction *fetchInstruction() const;

    IFIDBuffer *getBuffer() const;

    int getMemorySize() const;


    void setInstructions(QList< Instruction * >);
    QList< Instruction *> getInstructions() const;

    void setProgramCounter(ProgramCounter *) ;
    ProgramCounter *getProgramCounter() const;

    ~InstructionMemory();

signals:

public slots:

};

#endif // INSTRUCTIONMEMORY_H
