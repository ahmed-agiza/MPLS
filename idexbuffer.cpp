#include "idexbuffer.h"

IDEXBuffer::IDEXBuffer(QObject *parent, Component *sourceComponent)
    :Buffer(parent, sourceComponent){
}

void IDEXBuffer::clear(){
    _pc = 0;
    _registerA = 0;
    _registerB = 0;
    _immediate = 0;
}

void IDEXBuffer::setProgramCounter(ProgramCounter pc){
    _pc = pc;
}

ProgramCounter IDEXBuffer::getProgramCounter() const{
    return _pc;
}

void IDEXBuffer::setRegisterA(Register registerA){
    _registerA = registerA;
}

Register IDEXBuffer::getRegisterA() const{
    return _registerA;
}

void IDEXBuffer::setRegisterB(Register registerB){
    _registerB = registerB;
}

Register IDEXBuffer::getRegisterB() const{
    return _registerB;
}

void IDEXBuffer::setImmediate(int immediate){
    _immediate = immediate;
}

int IDEXBuffer::getImmediate() const{
    return _immediate;
}

IDEXBuffer::~IDEXBuffer(){

}
