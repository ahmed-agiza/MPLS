#include "simulator.h"



Simulator::Simulator(QObject *parent, QStringList rawInstructions)
    :QObject(parent), _rawInstructions(rawInstructions), _valid(false){

}

bool Simulator::parseInstructions(){

    return false;
}

void Simulator::setValid(bool valid){
    _valid = valid;
}

bool Simulator::isReady() const{
    return _valid;
}

bool Simulator::isComplete() const{

    return true;
}

void Simulator::simulate(){

}

void Simulator::nextCycle(){

}

int Simulator::getCurrentCycle() const{
    if (_core)
        return _core->getCycle();
    return 0;
}

Core *Simulator::getCore() const{
    return _core;
}

Simulator::~Simulator(){

}
