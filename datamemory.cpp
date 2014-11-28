#include "datamemory.h"

<<<<<<< HEAD
DataMemory::DataMemory(QObject * parent, Component * _sourceComponent, size_t , int _data, bool _writeEnabled, size_t _address)
    :Component(parent, _sourceComponent){
    this->_address = _address;

=======
DataMemory::DataMemory(QObject *parent, Component *sourceComponent, size_t size, int defaultValue, bool writeEnabled, size_t address)
    :Component(parent, sourceComponent), _writeEnabled(writeEnabled), _address(address){
    _values.reserve(size);
    for(int i = 0; i < (int)size; i++)
        _values.append(defaultValue);
>>>>>>> origin/master
}

MEMWBBuffer *DataMemory::getBuffer() const{
    return _buffer;
}

<<<<<<< HEAD
void DataMemory::setAddress(size_t _address){
    this->_address = _address;
=======
void DataMemory::setAddress(size_t address){
    _address = address;
>>>>>>> origin/master
}

size_t DataMemory::getAddress() const{
    return _address;
}

<<<<<<< HEAD
bool DataMemory::writeData(int _data){
    if(_writeEnabled)
    {
        _values[_address] = _data;
        return true;
    }
    else
    {
        return false;
    }
=======
bool DataMemory::writeData(int data){
    if (!isWriteEnabled()){
        qDebug() << "Inalid write.";
        return false;
    }
    if ((int)_address >= _values.size()){
        qDebug() << "Invalid index: " << _address;
        return false;
    }
    _values[_address] = data;
    return true;
>>>>>>> origin/master
}

int DataMemory::getData() const{
    return _values[_address];
}

<<<<<<< HEAD
void DataMemory::setWriteEnabled(bool _writeEnabled){
    this->_writeEnabled = _writeEnabled;
=======
void DataMemory::setWriteEnabled(bool value){
    _writeEnabled = value;
>>>>>>> origin/master
}

bool DataMemory::isWriteEnabled() const{
    return _writeEnabled;
}

DataMemory::~DataMemory(){

}
