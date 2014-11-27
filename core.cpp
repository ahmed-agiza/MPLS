#include "core.h"



Core::Core(QObject *){

}

void Core::run(){

}

void Core::nextCycle(){

}

void Core::executeCycle(){

}

void Core::gotoCycle(unsigned int){

}

unsigned int Core::getCycle() const{
    return _cycle;
}

DataMemory *Core::getDataMemory() const{
    return _dMem;
}

RegisterFile *Core::getRegisterFile() const{
    return _regFile;
}

QList< int > Core::getRegistersDump(int lowerBound, int higherBound) const{
    QList< int > values;
    for(int i = lowerBound; i <= higherBound && i <= _regFile->_registers.size(); i++)
        values.append(_regFile->_registers.at(i)->getValue());
    return values;
}

QList< int> Core::getMemoryDump(int lowerBound, int higherBound) const{
    QList< int > values;
    for(int i = lowerBound; i <= higherBound && i <= _regFile->_registers.size(); i++)
        values.append(_dMem->_values.at(i));
    return values;
}

bool Core::_if(){
    _instrQueue.prepend(_instructions.at(*_pc));
    _instructions.at(*_pc)->setState(ExecState::ID);
    return true;
}

bool Core::_id(int index){
    Q_UNUSED(index);

    return false;
}

bool Core::_ex(int index){
    Q_UNUSED(index);

    return false;
}

bool Core::_mem(int index){
    Q_UNUSED(index);

    return false;
}

bool Core::_wb(int index){
    Q_UNUSED(index);

    return false;
}

Core::~Core(){

}
