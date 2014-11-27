#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QObject>
#include <QDebug>

#include <functional>

#include "alu.h"
#include "datamemory.h"
#include "programcounter.h"
#include "register.h"

class Core;


enum class InstructionName{UNDEF, ADD, ADDI, XOR, LW, SW, BLE, J, SLT, JAL, JR};

enum class ExecState {UNDEF, IF, ID, EX, MEM, COMP};

#define INSTR_FUNC_PARAMS ProgramCounter *, ALU *, Register *, Register *, Register *, int, DataMemory *, ExecState

#define INSTR_FUNC_TEMPL int(INSTR_FUNC_PARAMS)

#define INSTR_FUNC std::function<INSTR_FUNC_TEMPL>

#define RCHECK _name == InstructionName::ADD || _name == InstructionName::JR || _name == InstructionName::SLT

#define ICHECK _name == InstructionName::BLE || _name == InstructionName::LW || _name == InstructionName::SW

#define JCHECK _name == InstructionName::J || _name == InstructionName::JAL

class Instruction :	public QObject {
    Q_OBJECT

    InstructionName _name;
    Register *_rs;
    Register *_rt;
    Register *_rd;
    int _immediate;
    ExecState _state;

    static QMap<InstructionName, QString> _instrNames;
    void initializeNamesMap();
public:

    Instruction(QObject * = 0, InstructionName = InstructionName::UNDEF, Register * = 0, Register * = 0, Register * = 0, int = 0, ExecState = ExecState::IF);

    bool isRInstruction() const;
    bool isIInstruction() const;
    bool isJInstruction() const;

    operator QString();

    void setName(InstructionName);
    InstructionName getName() const;

    void setState(ExecState);
    ExecState getState() const;

    void setRegisterRs(Register *);
    Register *getRegisterRs() const;

    void setRegisterRt(Register *);
    Register *getRegisterRt() const;

    void setRegisterRd(Register *);
    Register *getRegisterRd() const;

    void setImmediate(int);
    int getImmediate() const;

    ~Instruction();

signals:

public slots:

};
#endif // INSTRUCTION_H
