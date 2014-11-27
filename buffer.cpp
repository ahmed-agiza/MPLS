#include "buffer.h"

Buffer::Buffer(QObject * parent, Component * _sourceComponent)
    :Component(parent,_sourceComponent,State::FREE){
    this->_sourceComponent = _sourceComponent;
}

void Buffer::setContent(QString key, QString data){
    _contentMap[key] = data;
}

<<<<<<< HEAD
QString Buffer::getContent(QString key) const{
    return _contentMap[key];
=======
QString Buffer::getContent(QString) const{

>>>>>>> origin/master
}

bool Buffer::hasContent(QString key) const{
    return _contentMap.contains(key);
}

Buffer::~Buffer(){

}
