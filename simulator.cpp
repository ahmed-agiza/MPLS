#include "simulator.h"

Simulator::Simulator(QObject *parent, QStringList rawInstructions)
    :QObject(parent), _rawInstructions(rawInstructions){

}

bool Simulator::parseInstructions(){

    return false;
}

void Simulator::simulate(){

}

Simulator::~Simulator(){

}
