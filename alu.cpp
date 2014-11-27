#include "alu.h"

ALU::ALU(QObject * parent, Component * _sourceComponent, int _operandA, int _operandB, Operation _operation)
    : Component(parent, _sourceComponent, State::FREE){
    this->_sourceComponent = _sourceComponent;
    this->_operandA = _operandA;
    this->_operandB = _operandB;
    this->_operation = _operation;
}

EXMEMBuffer * ALU::getBuffer() const{
    return _buffer;
}

void ALU::setOperandA(int _operandA){
    this->_operandA = _operandA;
}

int ALU::getOperandA() const{
    return _operandA;
}

void ALU::setOperandB(int _operandB){
    this->_operandB = _operandB;
}

int ALU::getOperandB() const{
    return _operandB;
}

void ALU::setOperation(Operation _operation){
    this->_operation = _operation;
}

Operation ALU::getOperation() const{
    return _operation;
}

int ALU::getResult() const{
    return _result;
}

bool ALU::getZeroFlag() const{
    return _zeroFlag;
}

ALU::~ALU(){

}
