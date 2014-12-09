#ifndef EXMEMBUFFER_H
#define EXMEMBUFFER_H

#include "buffer.h"
#include "register.h"

enum class TargetType{REGISTER, MEMORY, STALLED, UNDEF};

class EXMEMBuffer : public Buffer
{
    Q_OBJECT

    int _branchTarget;
    int _ALUResult;
    bool _zeroFlag;
    int _targetRegisterNumber;
    TargetType _lastInstruction;
    Register _registerB;
public:
    EXMEMBuffer(QObject * = 0, Component * = 0);

    void clear() override;

    void setBranchTarget(int);
    int getBranchTarget() const;

    void setALUResult(int);
    int getALUResult() const;

    void setZeroFlag(bool);
    bool getZeroFlag() const;

    void setRegisterB(Register);
    Register getRegisterB() const;

    int getTargetRegisterNumber() const;
    void setTargetRegisterNumber(int value);

    TargetType getLastInstruction() const;
    void setLastInstruction(const TargetType &getLastInstruction);

    ~EXMEMBuffer();




signals:

public slots:

};

#endif // EXMEMBUFFER_H
