#include "component.h"

int Component::_lastID;

Component::Component(QObject *, Component *, State){

}

Buffer * Component::getBuffer() const{
    return 0;
}

void Component::getID() const{

}

void Component::setState(State){

}

State Component::getState() const{

}

void Component::setSourceComponent(Component *){

}

Component * Component::getSourceComponent() const{

}

Component::~Component(){

}
