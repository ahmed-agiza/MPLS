#include "alu.h"

ALU::ALU(QObject *parent, Component *sourceComponent, int operandA, int operandB, Operation operation):
Component(parent, sourceComponent), _operandA(operandA), _operandB(operandB), _operation(operation){
    _buffer = new EXMEMBuffer(this, this);
}

EXMEMBuffer *ALU::getBuffer() const{
    return _buffer;
}

void ALU::setOperandA(int operandA){
    _operandA = operandA;
}

int ALU::getOperandA() const{
    return _operandA;
}

void ALU::setOperandB(int operandB){
    _operandB = operandB;
}

int ALU::getOperandB() const{
    return _operandB;
}

void ALU::setOperation(Operation operation){
    _operation = operation;
}

Operation ALU::getOperation() const{
    return _operation;
}

int ALU::getResult() const{
    switch(_operation){
    case Operation::ADD:
        return (_operandA + _operandB);
        break;
    case Operation::SUB:
        return (_operandA - _operandB);
        break;
    case Operation::OR:
        return (_operandA | _operandB);
        break;
    case Operation::SLT:
        return (_operandA < _operandB);
        break;
    case Operation::LE:
        return (_operandA <= _operandB);
        break;
    case Operation::NOP:
        return 0;
    case Operation::UNDEF:
    default:
        qDebug() << "Undefined operation";
        return 0;
    }
}

bool ALU::getZeroFlag() const{
    return (_operandA == _operandB);
}

ALU::~ALU(){

}
