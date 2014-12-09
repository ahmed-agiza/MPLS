#include "instructionmemory.h"

void printQueue(const QList<Instruction *> &);

InstructionMemory::InstructionMemory(QObject *parent, Component *sourceComponent, QList< Instruction * > instructions, ProgramCounter *pc)
    :Component(parent, sourceComponent), _instructions(instructions), _pc(pc){
    _buffer = new IFIDBuffer(this, this);
}

Instruction *InstructionMemory::fetchInstruction() const{
    //qDebug() << "F" << (int)(*_pc) << "  ~~ " << _instructions.size();
    //printQueue(_instructions);
    int fetchingAddress = *_pc;
    qDebug() << "Fetching PC: " << fetchingAddress;
    if (fetchingAddress >= _instructions.size()){
        qDebug() << fetchingAddress << " >= " << _instructions.size();
        exit(1);
        return _instructions.last();
    }
    return _instructions.at(*_pc);
}

IFIDBuffer *InstructionMemory::getBuffer() const{
    return _buffer;
}

int InstructionMemory::getMemorySize() const{
    return _instructions.size();
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
