#include "exmembuffer.h"


EXMEMBuffer::EXMEMBuffer(QObject *parent, Component *sourceComponent)
    :Buffer(parent, sourceComponent), _targetRegisterNumber(-1), _lastInstruction(TargetType::UNDEF){
    
}

TargetType EXMEMBuffer::getLastInstruction() const{
    return _lastInstruction;
}

void EXMEMBuffer::setLastInstruction(const TargetType &lastInstruction){
    _lastInstruction = lastInstruction;
}

void EXMEMBuffer::clear(){
    _branchTarget = 0;
    _ALUResult = 0;
    _zeroFlag = 0;
    _registerB = 0;
    _targetRegisterNumber = -1;
}

void EXMEMBuffer::setBranchTarget(int branchTarget){
    _branchTarget = branchTarget;
}

int EXMEMBuffer::getBranchTarget() const{
    return _branchTarget;
}

void EXMEMBuffer::setALUResult(int ALUResult){
    _ALUResult = ALUResult;
}

int EXMEMBuffer::getALUResult() const{
    return _ALUResult;
}

void EXMEMBuffer::setZeroFlag(bool zeroFlag){
    _zeroFlag = zeroFlag;
}

bool EXMEMBuffer::getZeroFlag() const{
    return _zeroFlag;
}

void EXMEMBuffer::setRegisterB(Register registerB){
    _registerB = registerB;
}

Register EXMEMBuffer::getRegisterB() const{
    return _registerB;
}


void EXMEMBuffer::setTargetRegisterNumber(int value){
    _targetRegisterNumber = value;
}

int EXMEMBuffer::getTargetRegisterNumber() const{
    return _targetRegisterNumber;
}




EXMEMBuffer::~EXMEMBuffer(){

}
