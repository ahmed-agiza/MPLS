#include "simulator.h"



Simulator::Simulator(QObject *parent, QStringList rawInstructions)
    :QObject(parent), _rawInstructions(rawInstructions), _valid(false){

}

bool Simulator::parseInstructions(){

    QRegExp registerRegex(QString("\\$((?:[12]?[\\d])|(?:3[012])|(?:zero)|(?:at)|(?:v[01])|(?:a[0-3])|(?:t\\d)|(?:s[0-7])|(?:k[01])|gp|fp|ra|sp)"), Qt::CaseInsensitive);
    QRegExp commentRegex(QString("#.*"));
    QRegExp labelRegex(QString("[a-zA-Z_]\\w*"));
    QRegExp numberRegex(QString("(0x[0-9a-fA-F]+|-?\\d+|0b[01]+)"));

    foreach(const QString& line, _rawInstructions)
    {
        _instructions.append(new Instruction(this,InstructionName::ADD));
    }
    return false;
}

void Simulator::setValid(bool valid){
    _valid = valid;
}

bool Simulator::isReady() const{
    return _valid;
}

bool Simulator::isComplete() const{

    return true;
}

void Simulator::simulate(){

}

void Simulator::nextCycle(){

}

int Simulator::getCurrentCycle() const{
    if (_core)
        return _core->getCycle();
    return 0;
}

Core *Simulator::getCore() const{
    return _core;
}

Simulator::~Simulator(){

}
