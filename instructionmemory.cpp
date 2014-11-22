#include "instructionmemory.h"


InstructionMemory::InstructionMemory(QObject *, Component *, QList< Instruction * >, ProgramCounter *){

}

Instruction * InstructionMemory::fetchInstruction() const{

}

IFIDBuffer * InstructionMemory::getBuffer() const{

}

void InstructionMemory::setInstructions(QList< Instruction * >){

}

QList< Instruction * > InstructionMemory::getInstructions() const{

}

void InstructionMemory::setProgramCounter(ProgramCounter *){

}

ProgramCounter * InstructionMemory::getProgramCounter() const{

}

InstructionMemory::~InstructionMemory(){

}
