#include "instruction.h"

namespace InstructionFunction {
    INSTR_FUNC defaultFunc = [](INSTR_FUNC_PARAMS)->int {return 0;};
}


Instruction::Instruction(QObject *, ExecState, ProgramCounter *, ALU *, Register *, Register *, Register *, int, DataMemory *, INSTR_FUNC){

}

void Instruction::execute(){

}

void Instruction::setState(ExecState){

}

ExecState Instruction::getState() const{

}

void Instruction::setProgramCounter(ProgramCounter *){

}

ProgramCounter * Instruction::getProgramCounter() const{

}

void Instruction::setALU(ALU *){

}

ALU *Instruction::getALU() const{

}

void Instruction::setRegisterRs(Register *){

}

Register *Instruction::getRegisterRs() const{

}

void Instruction::setRegisterRt(Register *){

}

Register *Instruction::getRegisterRt(Register *){

}

void Instruction::setRegisterRd(Register *){

}

Register *Instruction::getRegisterRd(){

}

void Instruction::setImmediate(int){

}

int Instruction::getImmediate() const{

}

void Instruction::setDataMemory(DataMemory *){

}

DataMemory * Instruction::getDataMemory() const{

}

void Instruction::setFunction(INSTR_FUNC){

}

Instruction::~Instruction(){

}
