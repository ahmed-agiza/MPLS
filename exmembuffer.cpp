#include "exmembuffer.h"

EXMEMBuffer::EXMEMBuffer(QObject *parent, Component *sourceComponent, int branchTarget, int ALUResult, bool zeroFlag, Register *registerB)
    :Buffer(parent, sourceComponent), _branchTarget(branchTarget), _ALUResult(ALUResult), _zeroFlag(zeroFlag), _registerB(registerB){

}

void EXMEMBuffer::clear(){
    _branchTarget = 0;
    _ALUResult = 0;
    _zeroFlag = 0;
    _registerB = 0;
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

void EXMEMBuffer::setRegisterB(Register *registerB){
    _registerB = registerB;
}

Register *EXMEMBuffer::getRegisterB() const{
    return _registerB;
}

EXMEMBuffer::~EXMEMBuffer(){

}
