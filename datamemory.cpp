#include "datamemory.h"

DataMemory::DataMemory(QObject * parent, Component * _sourceComponent, size_t , int _data, bool _writeEnabled, size_t _address)
    :Component(parent, _sourceComponent){
    this->_address = _address;

}

MEMWBBuffer *DataMemory::getBuffer() const{
    return _buffer;
}

void DataMemory::setAddress(size_t _address){
    this->_address = _address;
}

size_t DataMemory::getAddress() const{
    return _address;
}

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
}

int DataMemory::getData() const{

}

void DataMemory::setWriteEnabled(bool _writeEnabled){
    this->_writeEnabled = _writeEnabled;
}

bool DataMemory::isWriteEnabled() const{
    return _writeEnabled;
}

DataMemory::~DataMemory(){

}
