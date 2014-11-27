#include "buffer.h"

Buffer::Buffer(QObject * parent, Component * _sourceComponent)
    :Component(parent,_sourceComponent,State::FREE){
    this->_sourceComponent = _sourceComponent;
}

void Buffer::setContent(QString key, QString data){
    _contentMap[key] = data;
}


QString Buffer::getContent(QString key) const{
    return _contentMap[key];
}

bool Buffer::hasContent(QString key) const{
    return _contentMap.contains(key);
}

Buffer::~Buffer(){

}
