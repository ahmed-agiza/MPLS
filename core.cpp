#include "core.h"

Core::Core(QObject * parent)
    :Component(parent){

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

DataMemory * Core::getDataMemory() const{
    return (_dMem);
}

RegisterFile *Core::getRegisterFile() const{
    return (_regFile);
}

QList< int > Core::getRegistersDump(size_t, size_t) const{

}

QList< int> Core::getMemoryDump(size_t, size_t) const{

}

Core::~Core(){

}
