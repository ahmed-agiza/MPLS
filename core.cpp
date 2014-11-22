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

}

DataMemory * Core::getDataMemory() const{

}

RegisterFile *Core::getRegisterFile() const{

}

QList< int > Core::getRegistersDump(size_t, size_t) const{

}

QList< int> Core::getMemoryDump(size_t, size_t) const{

}

Core::~Core(){

}
