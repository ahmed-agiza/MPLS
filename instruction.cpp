#include "instruction.h"

QMap<InstructionName, QString> Instruction::_instrNames;


Instruction::Instruction(QObject * parent, InstructionName name, int rs, int rt, int rd, int immediate, ExecState state)
    :QObject(parent), _name(name), _rs(rs), _rt(rt), _rd(rd), _immediate(immediate), _state(state){
    if(_instrNames.size() < 10)
        initializeNamesMap();
}

Instruction::Instruction(const Instruction &source)
    :QObject(source.parent()), _name(source._name), _rs(source._rs), _rt(source._rt), _rd(source._rd), _immediate(source._immediate), _state(source._state){

}

Instruction::operator QString(){
    return toString();
}

void Instruction::initializeNamesMap(){
    _instrNames[InstructionName::ADD] = "ADD";
    _instrNames[InstructionName::ADDI] = "ADDI";
    _instrNames[InstructionName::XOR] = "XOR";
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

QString Instruction::toString() const{
    QString name = _instrNames[_name];
    name += " ";
    if (isRInstruction()){
        name += Register::getRegisterNameString(_rd);
        name += ", ";
        name += Register::getRegisterNameString(_rs);
        name += ", ";
        name += Register::getRegisterNameString(_rt);
    }else if (isIInstruction()){
        if (_name == InstructionName::LW || _name == InstructionName::SW){
            name += Register::getRegisterNameString(_rt);
            name += ", ";
            name += QString::number(_immediate);
            name +="(";
            name += Register::getRegisterNameString(_rs);
            name += ")";
        }else{
            name += Register::getRegisterNameString(_rt);
            name += ", ";
            name += Register::getRegisterNameString(_rs);
            name += ", ";
            name += QString::number(_immediate);
        }
    }else if (isJInstruction()){
        name += QString::number(_immediate * 4);
    }

    return name;
}

Instruction &Instruction::operator=(const Instruction &source){
     _name = source._name;
     _rs = source._rs;
     _rt = source._rt;
     _rd = source._rd;
     _immediate = source._immediate;
     _state = source._state;

     return *this;
}

void Instruction::setName(InstructionName name){
    _name = name;
}

InstructionName Instruction::getName() const{
    return _name;
}

void Instruction::setState(ExecState state){
    qDebug() << "Setting " << *this << " to " << stageString(state);
    _state = state;
}

ExecState Instruction::getState() const{
    return _state;
}

void Instruction::setRegisterRs(int rs){
    _rs = rs;
}

int Instruction::getRegisterRs() const{
    return _rs;
}

void Instruction::setRegisterRt(int rt){
    _rt = rt;
}

int Instruction::getRegisterRt() const{
    return _rt;
}

void Instruction::setRegisterRd(int rd){
    _rd = rd;
}

int Instruction::getRegisterRd() const{
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

QString Instruction::stageString(ExecState state){
    switch(state){
    case ExecState::IF:
        return "IF";
    case ExecState::ID:
        return "ID";
    case ExecState::EX:
        return "EX";
    case ExecState::MEM:
        return "MEM";
    case ExecState::WB:
        return "WB";
    case ExecState::COMP:
        return "COMP";
    case ExecState::UNDEF:
    default:
        return "UNDEF";
    }
}
