#include "ifidbuffer.h"

IFIDBuffer::IFIDBuffer(QObject *parent, Component *sourceComponent, Instruction *instruction, ProgramCounter *pc)
    :Buffer(parent, sourceComponent), _instruction(instruction), _pc(pc){

}

void IFIDBuffer::clear(){
    _instruction = 0;
    _pc = 0;
}

void IFIDBuffer::setProgramCounter(ProgramCounter *pc){
    _pc = pc;
}

ProgramCounter *IFIDBuffer::getProgramCounter() const{
    return _pc;
}

void IFIDBuffer::setInstruction(Instruction *instruction){
    _instruction = instruction;
}

Instruction *IFIDBuffer::getInstruction() const{
    return _instruction;
}

IFIDBuffer::~IFIDBuffer(){

}
