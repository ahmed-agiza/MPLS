#include "ifidbuffer.h"

IFIDBuffer::IFIDBuffer(QObject *parent, Component *sourceComponent)
    :Buffer(parent, sourceComponent){

}

void IFIDBuffer::clear(){
    _instruction = Instruction(_instruction.parent(), InstructionName::UNDEF);
    _pc = 0;
}

void IFIDBuffer::setProgramCounter(ProgramCounter pc){
    _pc = pc;
}

ProgramCounter IFIDBuffer::getProgramCounter() const{
    return _pc;
}

void IFIDBuffer::setInstruction(Instruction instruction){
    _instruction = instruction;
}

Instruction IFIDBuffer::getInstruction() const{
    return _instruction;
}

IFIDBuffer::~IFIDBuffer(){

}
