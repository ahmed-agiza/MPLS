#include "registerfile.h"

QMap< unsigned int, RegisterName > RegisterFile::_regNums;
QMap< unsigned int, RegisterName > RegisterFile::_regNumsStrings;
QMap< RegisterName, QString > RegisterFile::_regNamesStrings;

RegisterFile::RegisterFile(QObject *, Component *, size_t, bool){

}

IDEXBuffer * RegisterFile::getBuffer() const{

}

void RegisterFile::setReadAddressA(size_t){

}

size_t RegisterFile::getReadAddressA() const{

}

void RegisterFile::setReadAddressB(size_t){

}

size_t RegisterFile::getReadAddressB() const{

}

void RegisterFile::setWriteAddress(size_t){

}

size_t RegisterFile::getWriteAddress() const{

}

void RegisterFile::setWriteEnabled(bool){

}

bool RegisterFile::isWriteEnabled() const{

}

int RegisterFile::getReadDataA() const{

}

int RegisterFile::getReadDataB() const{

}

bool RegisterFile::writeData(int data){

}

int RegisterFile::getWriteData() const{

}

RegisterFile::~RegisterFile(){

}

QString RegisterFile::getRegisterName(int){

}

QString RegisterFile::getRegisterName(RegisterName){

}
