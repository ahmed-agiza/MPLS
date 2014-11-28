#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QObject>
#include <QDebug>

#include <functional>

#include "alu.h"
#include "datamemory.h"
#include "programcounter.h"

class Core;


enum class InstructionName{UNDEF, ADD, ADDI, XOR, LW, SW, BLE, J, SLT, JAL, JR};

enum class ExecState {UNDEF, IF, ID, EX, MEM, WB, COMP};

#define INSTR_FUNC_PARAMS ProgramCounter *, ALU *, Register *, Register *, Register *, int, DataMemory *, ExecState

#define INSTR_FUNC_TEMPL int(INSTR_FUNC_PARAMS)

#define INSTR_FUNC std::function<INSTR_FUNC_TEMPL>

#define RCHECK _name == InstructionName::ADD || _name == InstructionName::JR || _name == InstructionName::SLT

#define ICHECK _name == InstructionName::BLE || _name == InstructionName::LW || _name == InstructionName::SW

#define JCHECK _name == InstructionName::J || _name == InstructionName::JAL

class Instruction :	public QObject {
    Q_OBJECT

    InstructionName _name;
    int _rs;
    int _rt;
    int _rd;
    int _immediate;
    ExecState _state;

    static QMap<InstructionName, QString> _instrNames;
    void initializeNamesMap();
public:

    Instruction(QObject * = 0, InstructionName = InstructionName::UNDEF, int = 0, int = 0, int = 0, int = 0, ExecState = ExecState::IF);
    Instruction(const Instruction &);

    bool isRInstruction() const;
    bool isIInstruction() const;
    bool isJInstruction() const;

    operator QString();

    Instruction &operator=(const Instruction &);

    void setName(InstructionName);
    InstructionName getName() const;

    void setState(ExecState);
    ExecState getState() const;

    void setRegisterRs(int);
    int getRegisterRs() const;

    void setRegisterRt(int);
    int getRegisterRt() const;

    void setRegisterRd(int);
    int getRegisterRd() const;

    void setImmediate(int);
    int getImmediate() const;

    ~Instruction();

signals:

public slots:

};
#endif // INSTRUCTION_H
