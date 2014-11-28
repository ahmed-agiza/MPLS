#include "datamemory.h"

DataMemory::DataMemory(QObject *parent, Component *sourceComponent, size_t size, int defaultValue, bool writeEnabled, size_t address)
    :Component(parent, sourceComponent), _writeEnabled(writeEnabled), _address(address){
    _values.reserve(size);
    for(int i = 0; i < (int)size; i++)
        _values.append(defaultValue);
}

MEMWBBuffer *DataMemory::getBuffer() const{
    return _buffer;
}

void DataMemory::setAddress(size_t address){
    _address = address;
}

size_t DataMemory::getAddress() const{
    return _address;
}

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
}

int DataMemory::getData() const{
    return _values[_address];
}

void DataMemory::setWriteEnabled(bool value){
    _writeEnabled = value;
}

bool DataMemory::isWriteEnabled() const{
    return _writeEnabled;
}

DataMemory::~DataMemory(){

}
