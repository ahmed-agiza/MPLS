#include "component.h"

int Component::_lastID;

Component::Component(QObject *parent, Component *sourceComponent):
    QObject(parent), _sourceComponent(sourceComponent), _state(State::FREE), _id(_lastID++), _lockOwner(-1){
}

Buffer * Component::getBuffer() const{
    return 0;
}

bool Component::lock(int id){
    if(_state == State::FREE || _lockOwner == id){
        _state = State::FULL_BUSY;
        _lockOwner = id;
        return true;
    }

    return false;
}

bool Component::halfLock(int id){
    if(_state == State::FREE || _lockOwner == id){
        _state = State::HALF_BUSY;
        _lockOwner = id;
        return true;
    }

    return false;
}

bool Component::unlock(int id){
    if(_lockOwner == id){
        _state = State::FREE;
        _lockOwner = -1;
        return true;
    }

    return false;
}

int Component::getLockOwnerID() const{
    return _lockOwner;
}

bool Component::isLocked() const{
    return (_state == State::FULL_BUSY);
}

bool Component::isHalfLocked() const{
    return (_state == State::HALF_BUSY);
}

int Component::getID() const{
    return _id;
}

void Component::setState(State state){
    _state = state;
}

State Component::getState() const{
    return _state;
}

void Component::setSourceComponent(Component *sourceComponent){
    _sourceComponent = sourceComponent;
}

Component *Component::getSourceComponent() const{
    return _sourceComponent;
}

Component::~Component(){

}
