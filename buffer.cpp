#include "buffer.h"

Buffer::Buffer(QObject *parent, Component *sourceComponent)
    :Component(parent, sourceComponent){

}

void Buffer::setContent(QString key, QString value){
    _contentMap[key] = value;
}

QString Buffer::getContent(QString key) const{
    return _contentMap[key];
}

bool Buffer::hasContent(QString key) const{
    return _contentMap.contains(key);
}

Buffer::~Buffer(){

}
