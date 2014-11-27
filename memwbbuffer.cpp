#include "memwbbuffer.h"

MEMWBBuffer::MEMWBBuffer(QObject *parent, Component *sourceComponent, int memData, int ALUResult)
    :Buffer(parent, sourceComponent), _memData(memData), _ALUResult(ALUResult){

}

void MEMWBBuffer::clear(){
    _memData = 0;
    _ALUResult = 0;
}

void MEMWBBuffer::setMemoryData(int memData){
    _memData = memData;
}

int MEMWBBuffer::getMemoryData() const{
    return _memData;
}

void MEMWBBuffer::setALUResult(int ALUResult){
    _ALUResult = ALUResult;
}

int MEMWBBuffer::getALUResult() const{
    return _ALUResult;
}

MEMWBBuffer::~MEMWBBuffer(){

}
