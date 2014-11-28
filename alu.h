#ifndef ALU_H
#define ALU_H

#include "component.h"
#include "exmembuffer.h"

enum class Operation {UNDEF, ADD, SUB, OR, SLT, LE, NOP};

class ALU : public Component
{
    Q_OBJECT

    int _operandA;
    int _operandB;
    Operation _operation;
    EXMEMBuffer * _buffer;

public:

    ALU(QObject * = 0, Component * = 0, int = 0, int = 0, Operation = Operation::UNDEF);

    EXMEMBuffer *getBuffer() const;

    void setOperandA(int);
    int getOperandA() const;

    void setOperandB(int);
    int getOperandB() const;

    void setOperation(Operation);
    Operation getOperation()const;

    int getResult() const;

    bool getZeroFlag() const;

    ~ALU();

signals:

public slots:

};

#endif // ALU_H
