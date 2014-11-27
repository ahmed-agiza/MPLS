#include "component.h"

int Component::_lastID;

Component::Component(QObject * parent, Component * _sourceComponent, State _state)
    :QObject(parent){
    this->_sourceComponent = _sourceComponent;
    this->_state = _state;
}

Buffer * Component::getBuffer() const{
    return 0;
}

int Component::getID() const{
    return this->_id;
}

void Component::setState(State _state){
    this->_state = _state;
}

State Component::getState() const{
    return _state;
}

void Component::setSourceComponent(Component * _sourceComponent){
    this->_sourceComponent = _sourceComponent;
}

Component * Component::getSourceComponent() const{
    return _sourceComponent;
}

Component::~Component(){

}
