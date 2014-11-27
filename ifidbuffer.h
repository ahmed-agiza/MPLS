#ifndef IFIDBUFFER_H
#define IFIDBUFFER_H

#include "buffer.h"
#include "programcounter.h"

class Instruction;

class IFIDBuffer : public Buffer
{
    Q_OBJECT

    Instruction *_instruction;
    ProgramCounter *_pc;    

public:
    IFIDBuffer(QObject * = 0, Component * = 0, Instruction * = 0, ProgramCounter * = 0);

    void clear() override;

    void setProgramCounter(ProgramCounter *);
    ProgramCounter *getProgramCounter() const;

    void setInstruction(Instruction *);
    Instruction *getInstruction() const;

    ~IFIDBuffer();

signals:

public slots:

};

#endif // IFIDBUFFER_H
