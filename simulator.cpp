#include "simulator.h"

QString registerRegex("\\$((?:[12]?[\\d])|(?:3[012])|(?:zero)|(?:at)|(?:v[01])|(?:a[0-3])|(?:t\\d)|(?:s[0-7])|(?:k[01])|gp|fp|ra|sp)");
QString commentRegex("#.*");
QString labelRegex("[a-zA-Z_][a-zA-Z0-9]*");
QString labelRegexCap("(" + labelRegex + "):");
QString numberRegex("(0x[0-9a-fA-F]+|-?[0-9]+|0b[01]+)");
QString whiteSpaceRegex("[\\t ]");
QString optionalWhiteSpace = whiteSpaceRegex + "*";
QString commaa = optionalWhiteSpace + "," + optionalWhiteSpace;

Simulator::Simulator(QObject *parent, QStringList rawInstructions)
    :QObject(parent), _rawInstructions(rawInstructions), _valid(false){
    initializeInstuctionNamesMap();
}


int Simulator::getRegisterNumber(QString s){
    s = s.toLower();
    QRegExp numbers(QString("^[0-9]+$"));
    if(numbers.indexIn(s) == 0) return s.toInt();
    else return Register::getRegisterNumber(s);
}

QString Simulator::getInstructionRegex(QString instructionString){
    return (QString("^") + optionalWhiteSpace + "(?:" + labelRegexCap + ")?" + optionalWhiteSpace + instructionString + optionalWhiteSpace + "(?:" + commentRegex + ")?$");
}

bool Simulator::parseInstructions(){

    foreach(QString line, _rawInstructions)
    {
        qDebug() << line;
    }

    qDebug() << "Parsing.";
    QList<QRegExp> instructionFormats;
    instructionFormats.append(QRegExp(getInstructionRegex(QString("(add|xor|slt)") + whiteSpaceRegex + "+" + registerRegex + commaa + registerRegex + commaa + registerRegex), Qt::CaseInsensitive));
    instructionFormats.append(QRegExp(getInstructionRegex(QString("(addi|ble)") + whiteSpaceRegex + "+" + registerRegex + commaa + registerRegex + commaa + numberRegex), Qt::CaseInsensitive));
    instructionFormats.append(QRegExp(getInstructionRegex(QString("(sw|lw)") + whiteSpaceRegex + "+" + registerRegex + commaa + numberRegex + "?" + optionalWhiteSpace + "\\(" + optionalWhiteSpace + registerRegex + optionalWhiteSpace + "\\)"), Qt::CaseInsensitive));
    instructionFormats.append(QRegExp(getInstructionRegex(QString("(ble)") + whiteSpaceRegex + "+" + registerRegex + commaa + registerRegex + commaa + "(" + labelRegex +")"), Qt::CaseInsensitive));
    instructionFormats.append(QRegExp(getInstructionRegex(QString("(jr)") + whiteSpaceRegex + "+" + registerRegex), Qt::CaseInsensitive));
    instructionFormats.append(QRegExp(getInstructionRegex(QString("(j|jal)") + whiteSpaceRegex + "+(" + labelRegex + ")" ), Qt::CaseInsensitive));
    instructionFormats.append(QRegExp(getInstructionRegex(QString("(j|jal)") + whiteSpaceRegex + "+" + numberRegex ), Qt::CaseInsensitive));
    instructionFormats.append(QRegExp(getInstructionRegex(""), Qt::CaseInsensitive));

    QMap<QString, int> symbolTable;
    QList<_MissingLabel> missingJumps;
    QList<_MissingLabel> missingBranches;

    QList<_ParsingError> errorList;
    int instructionNumber = 0;
    int lineNumber = 0;
    foreach(QString line, _rawInstructions)
    {
        line = line.toLower();
        qDebug() << line;
        if(instructionFormats[0].indexIn(line) == 0){   // add slt xor
            if(instructionFormats[0].cap(1).size() > 0){
                symbolTable[instructionFormats[0].cap(1)] = instructionNumber;
            }
            _instructions.append(new Instruction(this,
                                                 _instructionNames[instructionFormats[0].cap(2)],
                                                 Simulator::getRegisterNumber(instructionFormats[0].cap(4)),
                                                 Simulator::getRegisterNumber(instructionFormats[0].cap(5)),
                                                 Simulator::getRegisterNumber(instructionFormats[0].cap(3)),
                                                 0,
                                                 ExecState::IF
                                                 )
                                 );
            instructionNumber++;
        }else if(instructionFormats[1].indexIn(line) == 0){ // addi ble
            if(instructionFormats[1].cap(1).size() > 0){
                symbolTable[instructionFormats[1].cap(1)] = instructionNumber;
            }
            _instructions.append(new Instruction(this,
                                                 _instructionNames[instructionFormats[1].cap(2)],
                                                 Simulator::getRegisterNumber(instructionFormats[1].cap(4)),
                                                 Simulator::getRegisterNumber(instructionFormats[1].cap(3)),
                                                 0,
                                                 Simulator::getNumber(instructionFormats[1].cap(5)),
                                                 ExecState::IF
                                                 )
                                 );
            instructionNumber++;
        }else if(instructionFormats[2].indexIn(line) == 0){ // sw lw
            if(instructionFormats[2].cap(1).size() > 0){
                symbolTable[instructionFormats[2].cap(1)] = instructionNumber;
            }
            _instructions.append(new Instruction(this,
                                                 _instructionNames[instructionFormats[2].cap(2)],
                                                 Simulator::getRegisterNumber(instructionFormats[2].cap(5)),
                                                 Simulator::getRegisterNumber(instructionFormats[2].cap(3)),
                                                 0,
                                                 Simulator::getNumber(instructionFormats[2].cap(4)),
                                                 ExecState::IF
                                                 )
                                 );
            instructionNumber++;
        }else if(instructionFormats[3].indexIn(line) == 0){ // ble
            if(instructionFormats[3].cap(1).size() > 0){
                symbolTable[instructionFormats[3].cap(1)] = instructionNumber;
            }
            _instructions.append(new Instruction(this,
                                                 _instructionNames[instructionFormats[3].cap(2)],
                                                 Simulator::getRegisterNumber(instructionFormats[3].cap(4)),
                                                 Simulator::getRegisterNumber(instructionFormats[3].cap(3)),
                                                 0,
                                                 0,
                                                 ExecState::IF
                                                 )
                                 );

            if(symbolTable.contains(instructionFormats[0].cap(5))){
                _instructions[_instructions.size() - 1]->setImmediate(symbolTable[instructionFormats[0].cap(5)] - (instructionNumber + 1));
            }else{
                missingBranches.append(_MissingLabel(instructionFormats[0].cap(5), instructionNumber, lineNumber));
            }
            instructionNumber++;
        }else if(instructionFormats[4].indexIn(line) == 0){ // jr
            if(instructionFormats[4].cap(1).size() > 0){
                symbolTable[instructionFormats[4].cap(1)] = instructionNumber;
            }
            _instructions.append(new Instruction(this,
                                                 _instructionNames[instructionFormats[4].cap(2)],
                                                 Simulator::getRegisterNumber(instructionFormats[4].cap(3)),
                                                 0,
                                                 0,
                                                 0,
                                                 ExecState::IF
                                                 )
                                 );
            instructionNumber++;
        }else if(instructionFormats[5].indexIn(line) == 0){ // j jal
            if(instructionFormats[5].cap(1).size() > 0){
                symbolTable[instructionFormats[5].cap(1)] = instructionNumber;
            }
            _instructions.append(new Instruction(this,
                                                 _instructionNames[instructionFormats[5].cap(2)],
                                                 0,
                                                 0,
                                                 0,
                                                 0,
                                                 ExecState::IF
                                                 )
                                 );

            if(symbolTable.contains(instructionFormats[0].cap(3))){
                _instructions[_instructions.size() - 1]->setImmediate(symbolTable[instructionFormats[0].cap(3)]);
            }else{
                missingJumps.append(_MissingLabel(instructionFormats[0].cap(3), instructionNumber, lineNumber));
            }
            instructionNumber++;
        }else if(instructionFormats[6].indexIn(line) == 0){ // j jal
            if(instructionFormats[6].cap(1).size() > 0){
                symbolTable[instructionFormats[6].cap(1)] = instructionNumber;
            }
            _instructions.append(new Instruction(this,
                                                 _instructionNames[instructionFormats[6].cap(2)],
                                                 0,
                                                 0,
                                                 0,
                                                 Simulator::getNumber(instructionFormats[6].cap(3)),
                                                 ExecState::IF
                                                 )
                                 );
            instructionNumber++;
        }else if (instructionFormats[7].indexIn(line) == 0){
            if(instructionFormats[7].cap(1).size() > 0){
                symbolTable[instructionFormats[7].cap(1)] = instructionNumber;
            }
        }else{
            errorList.append(_ParsingError("Syntax Error", lineNumber));
        }
        lineNumber++;
    }

    foreach(const _MissingLabel& missingjmp, missingJumps){
        if(symbolTable.contains(missingjmp.getLabel())){
            _instructions[missingjmp.getAddress()]->setImmediate(symbolTable[missingjmp.getLabel()]);
        }else{
            errorList.append(_ParsingError(QString("Label \"") + missingjmp.getLabel() + "\" was not found", missingjmp.getLineNumber()));
        }
    }

    foreach(const _MissingLabel& missingbrn, missingBranches){
        if(symbolTable.contains(missingbrn.getLabel())){
            _instructions[missingbrn.getAddress()]->setImmediate(symbolTable[missingbrn.getLabel()] - (missingbrn.getAddress() + 1));
        }else{
            errorList.append(_ParsingError(QString("Label \"") + missingbrn.getLabel() + "\" was not found", missingbrn.getLineNumber()));
        }
    }


    foreach (const _ParsingError& pError, errorList) {
        qDebug() << pError.getLineNumber() << "\t" << pError.getErrorMessage();
    }

    qDebug() << "Parsed.";

    foreach(Instruction *ins, _instructions){
        qDebug() << *ins;
    }

    return (errorList.size() == 0);
}

void Simulator::setValid(bool valid){
    _valid = valid;
}

void Simulator::initializeInstuctionNamesMap(){
    _instructionNames["add"] = InstructionName::ADD;
    _instructionNames["addi"] = InstructionName::ADDI;
    _instructionNames["ble"] = InstructionName::BLE;
    _instructionNames["j"] = InstructionName::J;
    _instructionNames["jal"] = InstructionName::JAL;
    _instructionNames["jr"] = InstructionName::JR;
    _instructionNames["lw"] = InstructionName::LW;
    _instructionNames["slt"] = InstructionName::SLT;
    _instructionNames["sw"] = InstructionName::SW;
    _instructionNames["xor"] = InstructionName::XOR;
}

int Simulator::getNumber(QString s){
    if(s.isEmpty()) return 0;
    bool f;
    s = s.toLower();
    if(s.startsWith("0x"))
        return s.mid(2).toUInt(&f,16);
    else if(s.startsWith("0b"))
        return s.mid(2).toUInt(&f,2);
    else return s.toInt();
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
    if(_core)
        _core->executeCycle();
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
