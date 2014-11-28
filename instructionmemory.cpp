#include "instructionmemory.h"


InstructionMemory::InstructionMemory(QObject *parent, Component *sourceComponent, QList< Instruction * > instructions, ProgramCounter *pc)
    :Component(parent, sourceComponent), _instructions(instructions), _pc(pc){
    _buffer = new IFIDBuffer(this, this);
}

Instruction *InstructionMemory::fetchInstruction() const{
    return _instructions.at(*_pc);
}

IFIDBuffer *InstructionMemory::getBuffer() const{
    return _buffer;
}

void InstructionMemory::setInstructions(QList< Instruction * > instructions){
    _instructions = instructions;
}

QList< Instruction * > InstructionMemory::getInstructions() const{
    return _instructions;
}

void InstructionMemory::setProgramCounter(ProgramCounter *pc){
    _pc = pc;
}

ProgramCounter *InstructionMemory::getProgramCounter() const{
    return _pc;
}

InstructionMemory::~InstructionMemory(){

}
