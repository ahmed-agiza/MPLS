#include "instruction.h"

QMap<InstructionName, QString> Instruction::_instrNames;


Instruction::Instruction(QObject * parent, InstructionName name, Register *rs, Register *rt, Register *rd, int immediate, ExecState state)
    :QObject(parent), _name(name), _rs(rs), _rt(rt), _rd(rd), _immediate(immediate), _state(state){
    if(_instrNames.size() < 10)
        initializeNamesMap();
}

Instruction::operator QString(){
    QString name = _instrNames[_name];
    if (isRInstruction()){

    }else if (isIInstruction()){
        if (_name == InstructionName::LW || _name == InstructionName::SW){

        }
    }else if (isJInstruction()){

    }

    return name;
}

void Instruction::initializeNamesMap(){
    _instrNames[InstructionName::ADD] = "ADD";
    _instrNames[InstructionName::ADDI] = "ADDI";
    _instrNames[InstructionName::BLE] = "BLE";
    _instrNames[InstructionName::J] = "J";
    _instrNames[InstructionName::JAL] = "JAL";
    _instrNames[InstructionName::JR] = "JR";
    _instrNames[InstructionName::LW] = "LW";
    _instrNames[InstructionName::SLT] = "SLT";
    _instrNames[InstructionName::SW] = "SW";
    _instrNames[InstructionName::UNDEF] = "UNDEF";
}

bool Instruction::isRInstruction() const{
    return (RCHECK);
}

bool Instruction::isIInstruction() const{
    return (ICHECK);
}

bool Instruction::isJInstruction() const{
    return (JCHECK);
}

void Instruction::setName(InstructionName name){
    _name = name;
}

InstructionName Instruction::getName() const{
    return _name;
}

void Instruction::setState(ExecState state){
    _state = state;
}

ExecState Instruction::getState() const{
    return _state;
}

void Instruction::setRegisterRs(Register *rs){
    _rs = rs;
}

Register *Instruction::getRegisterRs() const{
    return _rs;
}

void Instruction::setRegisterRt(Register *rt){
    _rt = rt;
}

Register *Instruction::getRegisterRt() const{
    return _rt;
}

void Instruction::setRegisterRd(Register *rd){
    _rd = rd;
}

Register *Instruction::getRegisterRd() const{
    return _rd;
}

void Instruction::setImmediate(int immediate){
    _immediate = immediate;
}

int Instruction::getImmediate() const{
    return _immediate;
}


Instruction::~Instruction(){

}
