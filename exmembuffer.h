#ifndef EXMEMBUFFER_H
#define EXMEMBUFFER_H

#include "buffer.h"
#include "register.h"

class EXMEMBuffer : public Buffer
{
    Q_OBJECT

    int _branchTarget;
    int _ALUResult;
    bool _zeroFlag;
    Register *_registerB;
public:
    EXMEMBuffer(QObject * = 0, Component * = 0, int = 0, int = 0, bool = 0, Register * = 0);

    void clear() override;

    void setBranchTarget(int);
    int getBranchTarget() const;

    void setALUResult(int);
    int getALUResult() const;

    void setZeroFlag(bool);
    bool getZeroFlag() const;

    void setRegisterB(Register *);
    Register *getRegisterB() const;

    ~EXMEMBuffer();

signals:

public slots:

};

#endif // EXMEMBUFFER_H
