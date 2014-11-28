#include "programcounter.h"

ProgramCounter::ProgramCounter(QObject *parent, Component *sourceComponent, bool enabled)
    :Register(parent, sourceComponent, 0, RegisterName::PC), _enabled(enabled){

}

void ProgramCounter::setEnabled(bool value){
    _enabled = value;
}

bool ProgramCounter::isEnabled() const{
    return _enabled;
}

void ProgramCounter::increment(){
    _value++;
}

int ProgramCounter::getDisplayValue() const{
    return _value * 4;
}

ProgramCounter::~ProgramCounter(){

}
