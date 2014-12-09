#include "register.h"

const QMap< unsigned int, RegisterName > Register::_regNums{
    IntNameP(0, RegisterName::ZERO), IntNameP(1, RegisterName::AT),
    IntNameP(2, RegisterName::V0), IntNameP(3, RegisterName::V1),
    IntNameP(4, RegisterName::A0), IntNameP(5, RegisterName::A1),
    IntNameP(6, RegisterName::A2), IntNameP(7, RegisterName::A3),
    IntNameP(8, RegisterName::T0), IntNameP(9, RegisterName::T1),
    IntNameP(10, RegisterName::T2), IntNameP(11, RegisterName::T3),
    IntNameP(12, RegisterName::T4), IntNameP(13, RegisterName::T5),
    IntNameP(14, RegisterName::T6), IntNameP(15, RegisterName::T7),
    IntNameP(16, RegisterName::S0), IntNameP(17, RegisterName::S1),
    IntNameP(18, RegisterName::S2), IntNameP(19, RegisterName::S3),
    IntNameP(20, RegisterName::S4), IntNameP(21, RegisterName::S5),
    IntNameP(22, RegisterName::S6), IntNameP(23, RegisterName::S7),
    IntNameP(24, RegisterName::T8), IntNameP(25, RegisterName::T9),
    IntNameP(26, RegisterName::K0), IntNameP(27, RegisterName::K1),
    IntNameP(28, RegisterName::GP), IntNameP(29, RegisterName::SP),
    IntNameP(30, RegisterName::FP), IntNameP(31, RegisterName::RA),
    IntNameP(32, RegisterName::PC), IntNameP(33, RegisterName::BUFF),
    IntNameP(34, RegisterName::CUSTOM), IntNameP(-1, RegisterName::UNDEF),
    IntNameP(35, RegisterName::UNDEF)
};
const QMap< RegisterName, QString > Register::_regNames{
    NameStringP(RegisterName::ZERO, "$zero"), NameStringP(RegisterName::AT, "$at"),
    NameStringP(RegisterName::V0, "$v0"), NameStringP(RegisterName::V1, "$v1"),
    NameStringP(RegisterName::A0, "$a0"), NameStringP(RegisterName::A1, "$a1"),
    NameStringP(RegisterName::A2, "$a2"), NameStringP(RegisterName::A3, "$a3"),
    NameStringP(RegisterName::T0, "$t0"), NameStringP(RegisterName::T1, "$t1"),
    NameStringP(RegisterName::T2, "$t2"), NameStringP(RegisterName::T3, "$t3"),
    NameStringP(RegisterName::T4, "$t4"), NameStringP(RegisterName::T5, "$t5"),
    NameStringP(RegisterName::T6, "$t6"), NameStringP(RegisterName::T7, "$t7"),
    NameStringP(RegisterName::S0, "$s0"), NameStringP(RegisterName::S1, "$s1"),
    NameStringP(RegisterName::S2, "$s2"), NameStringP(RegisterName::S3, "$s3"),
    NameStringP(RegisterName::S4, "$s4"), NameStringP(RegisterName::S5, "$s5"),
    NameStringP(RegisterName::S6, "$s6"), NameStringP(RegisterName::S7, "$s7"),
    NameStringP(RegisterName::T8, "$t8"), NameStringP(RegisterName::T9, "$t9"),
    NameStringP(RegisterName::K0, "$k0"), NameStringP(RegisterName::K1, "$k1"),
    NameStringP(RegisterName::GP, "$gp"), NameStringP(RegisterName::SP, "$sp"),
    NameStringP(RegisterName::FP, "$fp"), NameStringP(RegisterName::RA, "$ra"),
    NameStringP(RegisterName::PC, "PC"), NameStringP(RegisterName::BUFF, "BUFF"),
    NameStringP(RegisterName::BUFF, "BUFF"), NameStringP(RegisterName::CUSTOM, "CUSTOM"),
    NameStringP(RegisterName::UNDEF, "UNDEF")
};
const QMap< QString, int > Register::_regNumbers{
    StringIntP("zero", 0), StringIntP("at", 1),
    StringIntP("v0", 2), StringIntP("v1", 2),
    StringIntP("a0", 4), StringIntP("a1", 5),
    StringIntP("a2", 6), StringIntP("a3", 7),
    StringIntP("t0", 8), StringIntP("t1", 9),
    StringIntP("t2", 10), StringIntP("t3", 11),
    StringIntP("t4", 12), StringIntP("t5", 13),
    StringIntP("t6", 14), StringIntP("t7", 15),
    StringIntP("s0", 16), StringIntP("s1", 17),
    StringIntP("s2", 18), StringIntP("s3", 19),
    StringIntP("s4", 20), StringIntP("s5", 21),
    StringIntP("s6", 22), StringIntP("s7", 23),
    StringIntP("t8", 24), StringIntP("t9", 25),
    StringIntP("k0", 26), StringIntP("k1", 27),
    StringIntP("gp", 28), StringIntP("sp", 29),
    StringIntP("fp", 30), StringIntP("ra", 31)
};

Register::Register(QObject *parent, Component *sourceComponent, int value, RegisterName name)
    :Component(parent, sourceComponent), _value(value), _accessors(0), _name(name), _state(RegisterState::STABLE){
}

Register::Register(const Register &source)
    :Component(source.parent(), source.getSourceComponent()), _value(source._value), _accessors(0), _name(source._name), _state(source._state){

}

QString Register::toString() const{
    QString name = getRegisterNameString();
    name += " - " + QString::number(getRegisterNumber(getRegisterNameString()));
    name += "(" + QString::number(_value) + ")";
   return name;
}

bool Register::is(const Register &other) const{
    return _name != RegisterName::UNDEF && _name != RegisterName::BUFF && _name != RegisterName::CUSTOM && _name == other._name;
}

QString Register::getRegisterNameString() const{
    return _regNames[_name];
}

void Register::setState(RegisterState state){
    if (state == RegisterState::WRITING){
        _accessors++;
        _state = state;
    }else if (_state != RegisterState::STABLE){
        if (!(--_accessors))
            _state = state;
    }
}

RegisterState Register::getState() const{
    return _state;
}


/*void Register::_initializeRegistersMap(){
    _regNums[0] = RegisterName::ZERO;
    _regNums[1] = RegisterName::AT;
    _regNums[2] = RegisterName::V0;
    _regNums[3] = RegisterName::V1;
    _regNums[4] = RegisterName::A0;
    _regNums[5] = RegisterName::A1;
    _regNums[6] = RegisterName::A2;
    _regNums[7] = RegisterName::A3;
    _regNums[8] = RegisterName::T0;
    _regNums[9] = RegisterName::T1;
    _regNums[10] = RegisterName::T2;
    _regNums[11] = RegisterName::T3;
    _regNums[12] = RegisterName::T4;
    _regNums[13] = RegisterName::T5;
    _regNums[14] = RegisterName::T6;
    _regNums[15] = RegisterName::T7;
    _regNums[16] = RegisterName::S0;
    _regNums[17] = RegisterName::S1;
    _regNums[18] = RegisterName::S2;
    _regNums[19] = RegisterName::S3;
    _regNums[20] = RegisterName::S4;
    _regNums[21] = RegisterName::S5;
    _regNums[22] = RegisterName::S6;
    _regNums[23] = RegisterName::S7;
    _regNums[24] = RegisterName::T8;
    _regNums[25] = RegisterName::T9;
    _regNums[26] = RegisterName::K0;
    _regNums[27] = RegisterName::K1;
    _regNums[28] = RegisterName::GP;
    _regNums[29] = RegisterName::SP;
    _regNums[30] = RegisterName::FP;
    _regNums[31] = RegisterName::RA;
    _regNums[32] = RegisterName::PC;
    _regNums[33] = RegisterName::BUFF;
    _regNums[34] = RegisterName::CUSTOM;
    _regNums[35] = _regNums[-1] = RegisterName::UNDEF;

    _regNames[RegisterName::ZERO] = "$zero";
    _regNames[RegisterName::AT] = "$at";
    _regNames[RegisterName::V0] = "$v0";
    _regNames[RegisterName::V1] = "$v1";
    _regNames[RegisterName::A0] = "$a0";
    _regNames[RegisterName::A1] = "$a1";
    _regNames[RegisterName::A2] = "$a2";
    _regNames[RegisterName::A3] = "$a3";
    _regNames[RegisterName::T0] = "$t0";
    _regNames[RegisterName::T1] = "$t1";
    _regNames[RegisterName::T2] = "$t2";
    _regNames[RegisterName::T3] = "$t3";
    _regNames[RegisterName::T4] = "$t4";
    _regNames[RegisterName::T5] = "$t5";
    _regNames[RegisterName::T6] = "$t6";
    _regNames[RegisterName::T7] = "$t7";
    _regNames[RegisterName::S0] = "$s0";
    _regNames[RegisterName::S1] = "$s1";
    _regNames[RegisterName::S2] = "$s2";
    _regNames[RegisterName::S3] = "$s3";
    _regNames[RegisterName::S4] = "$s4";
    _regNames[RegisterName::S5] = "$s5";
    _regNames[RegisterName::S6] = "$s6";
    _regNames[RegisterName::S7] = "$s7";
    _regNames[RegisterName::T8] = "$t8";
    _regNames[RegisterName::T9] = "$t9";
    _regNames[RegisterName::K0] = "$k0";
    _regNames[RegisterName::K1] = "$k1";
    _regNames[RegisterName::GP] = "$gp";
    _regNames[RegisterName::SP] = "$sp";
    _regNames[RegisterName::FP] = "$fp";
    _regNames[RegisterName::RA] = "$ra";
    _regNames[RegisterName::PC] = "PC";
    _regNames[RegisterName::BUFF] = "BUFF";
    _regNames[RegisterName::CUSTOM] = "CUSTOM";
    _regNames[RegisterName::UNDEF] = "UNDEFs";

    _regNumbers["zero"] = 0;
    _regNumbers["at"] = 1;
    _regNumbers["v0"] = 2;
    _regNumbers["v1"] = 3;
    _regNumbers["a0"] = 4;
    _regNumbers["a1"] = 5;
    _regNumbers["a2"] = 6;
    _regNumbers["a3"] = 7;
    _regNumbers["t0"] = 8;
    _regNumbers["t1"] = 9;
    _regNumbers["t2"] = 10;
    _regNumbers["t3"] = 11;
    _regNumbers["t4"] = 12;
    _regNumbers["t5"] = 13;
    _regNumbers["t6"] = 14;
    _regNumbers["t7"] = 15;
    _regNumbers["s0"] = 16;
    _regNumbers["s1"] = 17;
    _regNumbers["s2"] = 18;
    _regNumbers["s3"] = 19;
    _regNumbers["s4"] = 20;
    _regNumbers["s5"] = 21;
    _regNumbers["s6"] = 22;
    _regNumbers["s7"] = 23;
    _regNumbers["t8"] = 24;
    _regNumbers["t9"] = 25;
    _regNumbers["k0"] = 26;
    _regNumbers["k1"] = 27;
    _regNumbers["gp"] = 28;
    _regNumbers["sp"] = 29;
    _regNumbers["fp"] = 30;
    _regNumbers["ra"] = 31;
}*/

void Register::setValue(int value){
    _value = value;
}

int Register::getValue() const{
    return _value;
}

void Register::setName(RegisterName name){
    _name = name;
}

RegisterName Register::getName() const{
    return _name;
}

Register &Register::operator=(const Register &source){
    _value = source._value;
    _state = source._state;

    return *this;
}

Register::operator QString(){
   return toString();
}



Register::operator int(){
    return _value;
}

Register Register::operator+(const int &value){
    Register resultReg(parent(), getSourceComponent(), _value + value, RegisterName::CUSTOM);
    return resultReg;
}

Register &Register::operator+=(const int &value){
    _value += value;
    return *this;
}

Register &Register::operator-=(const int &value){
    _value -= value;
    return *this;
}

Register &Register::operator++(){
    _value++;
    return *this;
}

Register &Register::operator--(){
    _value--;
    return *this;
}

Register Register::operator--(int){
    Register temp(*this);
    _value--;
    return temp;
}

Register &Register::operator=(const int &value){
    _value = value;
    return *this;
}

Register Register::operator-(const int &value){
    Register resultReg(parent(), getSourceComponent(), _value - value, RegisterName::CUSTOM);
    return resultReg;
}

Register Register::operator*(const int &value){
    Register resultReg(parent(), getSourceComponent(), _value * value, RegisterName::CUSTOM);
    return resultReg;
}

Register Register::operator/(const int &value){
    Register resultReg(parent(), getSourceComponent(), _value / value, RegisterName::CUSTOM);
    return resultReg;
}

Register Register::operator&(const int &value){
    Register resultReg(parent(), getSourceComponent(), _value & value, RegisterName::CUSTOM);
    return resultReg;
}

Register Register::operator~(){
    Register resultReg(parent(), getSourceComponent(), ~_value, RegisterName::CUSTOM);
    return resultReg;
}

Register::~Register(){

}

RegisterName Register::getRegisterName(int number){
   // if(_regNums.size() < 36)
      //  _initializeRegistersMap();
    return _regNums[number];
}

QString Register::getRegisterNameString(int number){
   // if(_regNums.size() < 36)
      //  _initializeRegistersMap();
    return _regNames[_regNums[number]];
}

QString Register::getRegisterNameString(RegisterName name){
    //if(_regNums.size() < 36)
       // _initializeRegistersMap();
    return _regNames[name];
}

int Register::getRegisterNumber(QString name){
    //if(_regNums.size() < 36)
       // _initializeRegistersMap();
    if(name.startsWith("$"))
        name.remove(0, 1);
    return _regNumbers[name];
}
