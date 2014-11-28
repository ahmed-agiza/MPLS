#include "registerfile.h"



RegisterFile::RegisterFile(QObject *parent, Component *sourceComponent, size_t size, bool writeEnabled)
    :Component(parent, sourceComponent), _writeEnabled(writeEnabled){
    _registers.reserve(size);
    if (size >= 32)
        _initialzeDefaultRegisters();
    _buffer = new IDEXBuffer(this, this);
}

void RegisterFile::_initialzeDefaultRegisters(){
    for (int i = 0; i < 32; i++){
        _registers.append(new Register(this, this, 0, Register::getRegisterName(i)));
    }

    _registers[28]->setValue(0x10008000);
    _registers[29]->setValue(0x7FFFEFFC);
}

IDEXBuffer *RegisterFile::getBuffer() const{
    return _buffer;
}

void RegisterFile::setReadAddressA(size_t readAddressA){
    _readAddressA = readAddressA;
}

size_t RegisterFile::getReadAddressA() const{
    return _readAddressA;
}

void RegisterFile::setReadAddressB(size_t readAddressB){
    _readAddressB = readAddressB;
}

size_t RegisterFile::getReadAddressB() const{
    return _readAddressB;
}

void RegisterFile::setWriteAddress(size_t wrtieAddress){
    _writeAddress = wrtieAddress;
}

size_t RegisterFile::getWriteAddress() const{
    return _writeAddress;
}

void RegisterFile::setWriteEnabled(bool value){
    _writeEnabled = value;
}

bool RegisterFile::isWriteEnabled() const{
    return _writeEnabled;
}

int RegisterFile::getReadDataA() const{
    return _registers.at(_readAddressA)->getValue();
}

int RegisterFile::getReadDataB() const{
    return _registers.at(_readAddressB)->getValue();
}

void RegisterFile::setWriteData(int writeData){
    _writeData = writeData;
}

int RegisterFile::getWriteData() const{
    return _writeData;
}

bool RegisterFile::writeData(){
    if (!isWriteEnabled()){
        qDebug() << "Invalid write.";
        return false;
    }
    if ((int)_writeAddress >= _registers.size()){
        qDebug() << "Invalid index";
        return false;
    }
    *(_registers[_writeAddress]) = _writeData;
    return true;
}


RegisterFile::~RegisterFile(){

}
