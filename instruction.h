#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QObject>
#include <QDebug>

#include <functional>

#include "alu.h"
#include "datamemory.h"
#include "programcounter.h"
#include "register.h"


enum class ExecState {UNDEF, IF, ID, EX, MEM, COMP};

#define INSTR_FUNC_PARAMS ProgramCounter *, ALU *, Register *, Register *, Register *, int, DataMemory *, ExecState

#define INSTR_FUNC_TEMPL int(INSTR_FUNC_PARAMS)

#define INSTR_FUNC std::function<INSTR_FUNC_TEMPL>


class Instruction :	public QObject {
    Q_OBJECT

    ExecState _state;
    ProgramCounter * _pc;
    ALU * _alu;
    Register * _Rs;
    Register * _Rt;
    Register * _Rd;
    DataMemory * _memory;
    int _immediate;
    INSTR_FUNC _func;

public:

    Instruction(QObject * = 0, ExecState = ExecState::IF, ProgramCounter * = 0, ALU * = 0, Register * = 0, Register * = 0, Register * = 0, int = 0, DataMemory * = 0, INSTR_FUNC = [](INSTR_FUNC_PARAMS)->int {return 0;});

    void execute();

    void setState(ExecState);
    ExecState getState() const;

    void setProgramCounter(ProgramCounter *);
    ProgramCounter * getProgramCounter() const;

    void setALU(ALU *);
    ALU * getALU() const;

    void setRegisterRs(Register *);
    Register * getRegisterRs() const;

    void setRegisterRt(Register *);
    Register * getRegisterRt(Register *);

    void setRegisterRd(Register *);
    Register * getRegisterRd();

    void setImmediate(int);
    int getImmediate() const;

    void setDataMemory(DataMemory *);
    DataMemory * getDataMemory() const;

    void setFunction(INSTR_FUNC);
    INSTR_FUNC getFunction() const;

    ~Instruction();

signals:

public slots:

};
#endif // INSTRUCTION_H
