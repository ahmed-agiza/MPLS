#ifndef IFIDBUFFER_H
#define IFIDBUFFER_H

#include "buffer.h"
#include "instruction.h"
#include "programcounter.h"

class IFIDBuffer : public Buffer
{
    Q_OBJECT

    ProgramCounter * _pc;
    Instruction * _instruction;

public:
    IFIDBuffer(QObject * = 0, Component * = 0, Instruction * = 0, ProgramCounter * = 0);

    void clear() override;

    void setProgramCounter(ProgramCounter *);
    ProgramCounter * getProgramCounter() const;

    void setInstruction(Instruction *);
    Instruction * getInstruction() const;

    ~IFIDBuffer();

signals:

public slots:

};

#endif // IFIDBUFFER_H
