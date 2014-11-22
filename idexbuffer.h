#ifndef IDEXBUFFER_H
#define IDEXBUFFER_H

#include "buffer.h"
#include "programcounter.h"
#include "register.h"

class IDEXBuffer : public Buffer
{
    Q_OBJECT

    ProgramCounter * _pc;
    Register * _registerA;
    Register * _registerB;
    int _immediate;

public:
    IDEXBuffer(QObject * = 0, Component * = 0, ProgramCounter * = 0, Register * = 0, Register * = 0, int = 0);

    void clear() override;

    void setProgramCounter(ProgramCounter *);
    ProgramCounter* getProgramCounter() const;

    void setRegisterA(Register *);
    Register* getRegisterA() const;

    void setRegisterB(Register *);
    Register* getRegisterB() const;

    void setImmediate(int);
    int getImmediate() const;

    ~IDEXBuffer();

signals:

public slots:

};

#endif // IDEXBUFFER_H
