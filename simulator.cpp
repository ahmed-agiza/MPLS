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

void Simulator::simulate(){

}

Simulator::~Simulator(){

}
