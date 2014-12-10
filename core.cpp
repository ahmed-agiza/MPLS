#include "core.h"




Core::Core(QObject * parent, QList<Instruction *> instructions)
    :Component(parent), _instructions(instructions), _cycle(0), _indexInstr(0), _stalls(0){
    _constructComponents();
}

void Core::_constructComponents(){
    _pc = new ProgramCounter(this, 0, true);
    _iMem = new InstructionMemory(this, _pc->getBuffer(), _instructions, _pc);
    _regFile = new RegisterFile(this, _iMem->getBuffer(), 32);
    _ALU = new ALU(this, _regFile->getBuffer());
    _dMem = new DataMemory(this, _ALU->getBuffer());
}

void printQueue(const QList<Instruction *> &q){
    if(q.size() == 0){
        qDebug() << "Empty.";
        return;
    }
    qDebug() << "{";
    for(int i = 0; i < q.size(); i++)
        qDebug() << i << *(q.at(i));
    qDebug() << "}";
}


bool Core::_execute(Instruction *instruction, int &index){
    bool proceed = false;
    qDebug() << "Recieved: " <<  *instruction << " at stage " << Instruction::stageString(instruction->getState()) << endl;
    if (instruction->isRInstruction())
        qDebug() << "R Instruction";
    else if (instruction->isIInstruction())
        qDebug() << "I Instruction";
    else if (instruction->isJInstruction())
        qDebug() << "J Instruction";
    else
        qDebug() << "Unkown Format";

    auto regBuf = _regFile->getBuffer();
    auto aluBuf = _ALU->getBuffer();
    auto memBuf = _dMem->getBuffer();

    //ADD, ADDI, XOR, LW, SW, BLE, J, SLT, JAL, JR

    switch (instruction->getState()){
    case ExecState::IF:{
        qDebug() << "ID: " << *instruction;
        emit currentStage("ID", _cycle + _stalls, _instructionNumbers[instruction], instruction);
        //Decode:
        if (instruction->isRInstruction()){
            qDebug() << "$rs = " << instruction->getRegisterRs();
            qDebug() << "$rt = " << instruction->getRegisterRt();
            qDebug() << "$rd = " << instruction->getRegisterRd();
            _regFile->setReadAddressA(instruction->getRegisterRs());
            _regFile->setReadAddressB(instruction->getRegisterRt());
           // _regFile->setWriteAddress(instruction->getRegisterRd());
            if (instruction->getName() == InstructionName::ADD || instruction->getName() == InstructionName::XOR || instruction->getName() == InstructionName::SLT)
                _regFile->_registers[instruction->getRegisterRd()]->setState(RegisterState::WRITING);
            proceed = true;
        }else if (instruction->isIInstruction()){
            qDebug() << "$rs = " << instruction->getRegisterRs();
            qDebug() << "$rt = " << instruction->getRegisterRt();
            qDebug() << "imm = " << instruction->getImmediate();
            _regFile->setReadAddressA(instruction->getRegisterRs());
            _regFile->setReadAddressB(0);
            //_regFile->setWriteAddress(instruction->getRegisterRt());
            if (instruction->getName() == InstructionName::ADDI || instruction->getName() == InstructionName::LW)
                _regFile->_registers[instruction->getRegisterRt()]->setState(RegisterState::WRITING);
            proceed = true;
        }else if (instruction->isJInstruction()){
            if (instruction->getName() == InstructionName::JAL){
               // _regFile->setWriteAddress(31);
                _regFile->setWriteData(*_pc);
                _regFile->_registers[31]->setState(RegisterState::WRITING);
            }
            proceed = true;
        }else{
            qDebug() << "Unkown instruction format" << *instruction;
            exit(1);
        }

        qDebug() << "PC: " << _pc->getDisplayValue();
        regBuf->setProgramCounter(_iMem->getBuffer()->getProgramCounter());
        regBuf->setRegisterA(*(_regFile->_registers[instruction->getRegisterRs()]));
        regBuf->setRegisterB(*(_regFile->_registers[instruction->getRegisterRt()]));
        regBuf->setImmediate(instruction->getImmediate());

        if (proceed)
            instruction->setState(ExecState::ID);
        else
            qDebug() << "Not procceeding from ID";
        qDebug() << "End of ID";
        return true;
        break;

    }case ExecState::ID:{
        //Execute:
         qDebug() << "EX: " << *instruction;
        emit currentStage("EX", _cycle + _stalls, _instructionNumbers[instruction], instruction);
        if (instruction->isRInstruction()){
            qDebug() << "Operand A: " << _regFile->getReadDataA();
            qDebug() << "Operand B: " << _regFile->getReadDataB();
            int operandA = _regFile->getReadDataA();
            int operandB = _regFile->getReadDataB();

            qDebug() << "Last target number: " << aluBuf->getTargetRegisterNumber();
            if (aluBuf->getTargetRegisterNumber() == instruction->getRegisterRs()){
                qDebug() << "*/*/*/*/*/*/*/*/*/*Rs is already being modified*/*/*/*/*/*/*/*/*/*";
                if (aluBuf->getLastInstruction() == TargetType::REGISTER){
                    qDebug() << "Old value: " << _regFile->getReadDataA();
                    qDebug() << "Correct value: " << aluBuf->getALUResult();
                    operandA = aluBuf->getALUResult();
                    emit forwarded("Forwared the correct value: " + QString::number(aluBuf->getALUResult()));
                }else if (aluBuf->getLastInstruction() == TargetType::MEMORY){
                    qDebug() << "The hazard source is memory";
                    qDebug() << "=+=+=++====+++++Stalling=+=+=++====+++++";
                    qDebug() << "XXXX";
                    aluBuf->setLastInstruction(TargetType::STALLED);
                    emit currentStage("Stall", _cycle + _stalls, _instructionNumbers[instruction], instruction);
                    _stalls++;
                    return false;
                }else{
                    qDebug() << "After stalling, correct value: " << memBuf->getMemoryData();
                    operandA = memBuf->getMemoryData();
                }
            }
            if (aluBuf->getTargetRegisterNumber() == instruction->getRegisterRt()){
                qDebug() << "*/*/*/*/*/*/*/*/*/*Rt is already being modified*/*/*/*/*/*/*/*/*/*";
                if (aluBuf->getLastInstruction() == TargetType::REGISTER){
                    qDebug() << "Old value: " << _regFile->getReadDataB();
                    qDebug() << "Correct value: " << aluBuf->getALUResult();
                    operandB = aluBuf->getALUResult();
                    emit forwarded("Forwared the correct value: " + QString::number(aluBuf->getALUResult()));

                }else if (aluBuf->getLastInstruction() == TargetType::MEMORY){
                    qDebug() << "The hazard source is memory";
                    qDebug() << "=+=+=++====+++++Stalling=+=+=++====+++++";
                    aluBuf->setLastInstruction(TargetType::STALLED);
                    emit currentStage("Stalll", _cycle + _stalls, _instructionNumbers[instruction], instruction);
                    _stalls++;
                    return false;
                }else{
                    qDebug() << "After stalling, correct value: " << memBuf->getMemoryData();
                    operandB = memBuf->getMemoryData();
                }
            }
            qDebug() << "Correct Operand A: " << operandA;
            qDebug() << "Correct Operand B: " << operandB;
            _ALU->setOperandA(operandA);
            _ALU->setOperandB(operandB);
            aluBuf->setTargetRegisterNumber(-1);
            aluBuf->setLastInstruction(TargetType::UNDEF);
            switch (instruction->getName()){
            case InstructionName::ADD:
                _ALU->setOperation(Operation::ADD);
                aluBuf->setTargetRegisterNumber(instruction->getRegisterRd());
                aluBuf->setLastInstruction(TargetType::REGISTER);
                break;
            case InstructionName::XOR:
                _ALU->setOperation(Operation::XOR);
                aluBuf->setTargetRegisterNumber(instruction->getRegisterRd());
                aluBuf->setLastInstruction(TargetType::REGISTER);
                break;
            case InstructionName::JR:
                _ALU->setOperation(Operation::NOP);
                _pc->setValue(_regFile->getReadDataA());
                break;
            case InstructionName::SLT:
                _ALU->setOperation(Operation::SLT);
                aluBuf->setTargetRegisterNumber(instruction->getRegisterRd());
                aluBuf->setLastInstruction(TargetType::REGISTER);
                break;
            default:
                qDebug() << "Invalid instruction: " << *instruction;
                exit(1);
            }
            proceed = true;
        }else if (instruction->isIInstruction()){
            qDebug() << "Operand A: " << _regFile->getReadDataA();
            qDebug() << "Operand B: " << instruction->getImmediate();
            int operandA = _regFile->getReadDataA();
            qDebug() << "Last target number: " << aluBuf->getTargetRegisterNumber();
            if (aluBuf->getTargetRegisterNumber() == instruction->getRegisterRs()){
                qDebug() << "*/*/*/*/*/*/*/*/*/*Rs is already being modified*/*/*/*/*/*/*/*/*/*";
                if (aluBuf->getLastInstruction() == TargetType::REGISTER){
                    qDebug() << "Register: "  << instruction->getRegisterRs();
                    qDebug() << "Old value: " << _regFile->getReadDataA();
                    qDebug() << "Correct value: " << aluBuf->getALUResult();
                    operandA = aluBuf->getALUResult();
                    emit forwarded("Forwared the correct value: " + QString::number(aluBuf->getALUResult()));
                }else if (aluBuf->getLastInstruction() == TargetType::MEMORY){
                    qDebug() << "The hazard source is memory";
                    qDebug() << "=+=+=++====+++++Stalling=+=+=++====+++++";
                    aluBuf->setLastInstruction(TargetType::STALLED);
                    emit currentStage("Stall", _cycle + _stalls, _instructionNumbers[instruction], instruction);
                    _stalls++;
                    return false;
                }else{
                    qDebug() << "After stalling, correct value: " << memBuf->getMemoryData();
                    operandA = memBuf->getMemoryData();
                }
            }

            _ALU->setOperandA(operandA);
            _ALU->setOperandB(instruction->getImmediate());
            switch (instruction->getName()){
            case InstructionName::ADDI:
                _ALU->setOperation(Operation::ADD);
                aluBuf->setTargetRegisterNumber(instruction->getRegisterRt());
                aluBuf->setLastInstruction(TargetType::REGISTER);
                break;

            case InstructionName::BLE:
            {
                _ALU->setOperation(Operation::LE);
                _regFile->setReadAddressB(instruction->getRegisterRt());
                int operandBLE = _regFile->getReadDataB();
                if (aluBuf->getTargetRegisterNumber() == instruction->getRegisterRt()){
                    qDebug() << "*/*/*/*/*/*/*/*/*/*Rs is already being modified*/*/*/*/*/*/*/*/*/*";
                    if (aluBuf->getLastInstruction() == TargetType::REGISTER){
                        qDebug() << "Register: "  << instruction->getRegisterRt();
                        qDebug() << "Old value: " << _regFile->getReadDataB();
                        qDebug() << "Correct value: " << aluBuf->getALUResult();
                        operandBLE = aluBuf->getALUResult();
                        emit forwarded("Forwared the correct value: " + QString::number(aluBuf->getALUResult()));
                    }else if (aluBuf->getLastInstruction() == TargetType::MEMORY){
                        qDebug() << "The hazard source is memory";
                        qDebug() << "=+=+=++====+++++Stalling=+=+=++====+++++";
                        aluBuf->setLastInstruction(TargetType::STALLED);
                        emit currentStage("Stall", _cycle + _stalls, _instructionNumbers[instruction], instruction);
                        _stalls++;
                        return false;
                    }else{
                        qDebug() << "After stalling, correct value: " << memBuf->getMemoryData();
                        operandA = memBuf->getMemoryData();
                    }
                }
                    _ALU->setOperandB(operandBLE);
                    qDebug() << "BLE: Current PC: " << _pc->getValue();
                    qDebug() << "BLE: Buffered PC: " << regBuf->getProgramCounter().getValue();
                    qDebug() << "Index before: " << index;
                    qDebug() << _ALU->getOperandA();
                    qDebug() << _ALU->getOperandB();
                    if (_ALU->getResult()){
                        _pc->setValue(regBuf->getProgramCounter().getValue() + instruction->getImmediate());
                        for(int i = index - 1; i >= 0; i--){
                            qDebug() << "Before dismiss: ";  printQueue(_instrQueue);

                            if(i < _instrQueue.size()){
                                _instrQueue.at(i)->setState(ExecState::UNDEF);
                                _instrQueue.removeAt(i);
                                index--;
                                qDebug() << "New queue: ";
                                printQueue(_instrQueue);
                            }
                        }
                        qDebug() << "New index: " << index;
                    }
                }break;
            case InstructionName::LW:
                 aluBuf->setTargetRegisterNumber(instruction->getRegisterRt());
                 aluBuf->setLastInstruction(TargetType::MEMORY);
            case InstructionName::SW:
                _ALU->setOperation(Operation::ADD);
                _dMem->setAddress(_ALU->getResult());
                break;
            default:
                qDebug() << "Invalid instruction: " << *instruction;
                exit(1);
            }
            proceed = true;
        }else if (instruction->isJInstruction()){
            if (instruction->getName() == InstructionName::JAL){
                qDebug() << "JAL:  " << regBuf->getProgramCounter().getValue();
                _ALU->setOperation(Operation::ADD);
                 aluBuf->setTargetRegisterNumber(31);
                _ALU->setOperandA(regBuf->getProgramCounter().getValue());
                _ALU->setOperandB(0);
                _ALU->setOperation(Operation::ADD);
            }
            qDebug() << "Jumping with " << *instruction;
            qDebug() << "Old PC: " << _pc->getDisplayValue();
            _pc->setValue(((int)(regBuf->getProgramCounter()) & 0xF0000000) | (instruction->getImmediate() & 0x0FFFFFFF));
            qDebug() << "New PC: " << _pc->getDisplayValue();
            proceed = true;
        }else{
            qDebug() << "Unkown instruction format" << *instruction;
            exit(1);
        }
        qDebug() << "ALU Result: " << _ALU->getResult();
        qDebug() << "Zero Buffer: " << _ALU->getZeroFlag();
        qDebug() << "Branch Target: " << _ALU->getResult();
        qDebug() << "Register B: " << _regFile->_registers.at(instruction->getRegisterRt())->getRegisterNameString() << "  " <<_regFile->_registers.at(instruction->getRegisterRt())->getValue();
        aluBuf->setALUResult(_ALU->getResult());
        aluBuf->setZeroFlag(_ALU->getZeroFlag());
        aluBuf->setBranchTarget(_ALU->getResult());
        aluBuf->setRegisterB(*(_regFile->_registers.at(instruction->getRegisterRt())));
        if (proceed)
            instruction->setState(ExecState::EX);
        else
            qDebug() << "Not procceeding from EX";
        qDebug() << "End of EX";
        return true;
        break;

    }case ExecState::EX:{
        //Memory Read/Write:
        qDebug() << "MEM: " << *instruction;
        emit currentStage("MEM", _cycle + _stalls, _instructionNumbers[instruction], instruction);
        if (instruction->isRInstruction()){
            qDebug() << "R-Instruction has no effect on memory";
            memBuf->setALUResult(aluBuf->getALUResult());
            memBuf->setMemoryData(_dMem->getData());
            proceed = true;
        }else if (instruction->isIInstruction()){
            switch (instruction->getName()) {
            case InstructionName::SW:{
                qDebug() << "SW: ";
                int writeData = _regFile->_registers.at(instruction->getRegisterRt())->getValue();
                qDebug() << "Write Data: " << writeData;
                qDebug() << "Write Address: " << aluBuf->getALUResult();
                qDebug() << aluBuf->getRegisterB().toString();

                _dMem->setAddress(aluBuf->getALUResult());
                _dMem->setWriteEnabled(true);
                _dMem->writeData(writeData);
                _dMem->setWriteEnabled(false);
                break;
            }case InstructionName::LW:
                qDebug() << "LW: ";
                qDebug() << "Read Address: " << aluBuf->getALUResult();
                 _dMem->setAddress(aluBuf->getALUResult());
                qDebug() << "Reg Write Data: " << _dMem->getData();
                _regFile->setWriteData(_dMem->getData());
                break;
            }
            qDebug() << "Mem Buf = " << aluBuf->getALUResult();
            memBuf->setALUResult(aluBuf->getALUResult());
            memBuf->setMemoryData(_dMem->getData());
            proceed = true;
        }else if (instruction->isJInstruction()){
            memBuf->setALUResult(aluBuf->getALUResult() * 4);
            memBuf->setMemoryData(_dMem->getData());
            qDebug() << "J-Instruction has no effect on memory";
            proceed = true;
        }else{
            qDebug() << "Unkown instruction format" << *instruction;
            exit(1);
        }

        if (proceed)
            instruction->setState(ExecState::MEM);
        else
            qDebug() << "Not procceeding from MEM";
        qDebug() << "End of MEM";
        return true;
        break;

    }case ExecState::MEM:{
        //Regfile WriteBack:
        qDebug() << "WB: " << *instruction;
        emit currentStage("WB", _cycle + _stalls, _instructionNumbers[instruction], instruction);
        if (instruction->isRInstruction()){
            if (instruction->getName() == InstructionName::ADD || instruction->getName() == InstructionName::XOR || instruction->getName() == InstructionName::SLT){
                qDebug() << "Write Data: " << memBuf->getALUResult();
                _regFile->setWriteData(memBuf->getALUResult());
                _regFile->setWriteAddress(instruction->getRegisterRd());
                _regFile->setWriteEnabled(true);
                _regFile->writeData();
                _regFile->setWriteEnabled(false);
                _regFile->_registers[instruction->getRegisterRd()]->setState(RegisterState::STABLE);
            }
            proceed = true;
        }else if (instruction->isIInstruction()){
            if (instruction->getName() == InstructionName::LW){
                qDebug() << "LW";
                qDebug() << "Write Address: " << instruction->getRegisterRt();
                qDebug() << "Writed Data: " << memBuf->getMemoryData();
                _regFile->setWriteAddress(instruction->getRegisterRt());
                _regFile->setWriteData(memBuf->getMemoryData());
                _regFile->setWriteEnabled(true);
                _regFile->writeData();
                _regFile->setWriteEnabled(false);
                _regFile->_registers[instruction->getRegisterRt()]->setState(RegisterState::STABLE);
            }else if (instruction->getName() == InstructionName::ADDI){
                qDebug() << "Write Data: " << memBuf->getALUResult();
                _regFile->setWriteAddress(instruction->getRegisterRt());
                _regFile->setWriteData(memBuf->getALUResult());
                _regFile->setWriteEnabled(true);
                _regFile->writeData();
                _regFile->setWriteEnabled(false);
                _regFile->_registers[instruction->getRegisterRt()]->setState(RegisterState::STABLE);
            }
            proceed = true;
        }else if (instruction->isJInstruction()){
            if (instruction->getName() == InstructionName::JAL){
                qDebug() << "Write Data: " << memBuf->getALUResult();
                _regFile->setWriteData(memBuf->getALUResult());
                _regFile->setWriteAddress(31);
                _regFile->setWriteEnabled(true);
                _regFile->writeData();
                _regFile->setWriteEnabled(false);
                _regFile->_registers[31]->setState(RegisterState::STABLE);

            }
            proceed = true;
        }else{
            qDebug() << "Unkown instruction format: " << *instruction;
            exit(1);
        }
        if (proceed){
            instruction->setState(ExecState::COMP);
            qDebug() << "Removing: " << *instruction << " == " << *(_instrQueue.last());
            _instrQueue.removeLast();
           // index- = ;
           // qDebug() << "Index: " << index;
            qDebug() << "New Queue: ";
            printQueue(_instrQueue);
        }else
            qDebug() << "Not procceeding from WB";
        qDebug() << "End of WB";
        return true;
        break;

    }case ExecState::COMP:
    case ExecState::WB:{
        qDebug() << "There is a bug! \\^-^/ Instruction is already executed..or not." << *instruction;
        break;

    }case ExecState::UNDEF:
    default:{
        qDebug() << "Unkown instruction execution stage: " << *instruction;
        exit(1);
    }
    }
    return false;
}


void Core::executeCycle(){

    int execution = 0;
    qDebug() << "Cycle: " << _cycle;
    qDebug() << "Queue before fetching: ";
    printQueue(_instrQueue);



    qDebug() << "Queue after fetching: ";
    printQueue(_instrQueue);

    for(int i = _instrQueue.size() - 1; i < _instrQueue.size() && i >= 0; i--){
        qDebug() << "*********** " << i << " ***********";
        auto inst = _instrQueue.at(i);
        qDebug() << "PC before instruction: " << *(inst) << ":  " <<  ((int)(*_pc)) << "\n\n\n\n\n";
        if (!_execute(inst, i)){
            qDebug() << "PC after instruction: "<< *(inst) << ":  " << ((int)(*_pc));
            qDebug() << "Stalled.";
            emit stalled();
            i++;
            //break;
        }else
            qDebug() << "PC after instruction: "<< *(inst) << ":  " << ((int)(*_pc));
        execution++;
        //qDebug() << "Numbers Map: " << Register::_regNumbers;
        qDebug() << "====================";
    }
    execution += _if();
    qDebug() << execution << " Operations.";
    if (execution > 0)
        _cycle++;
    else
        emit simulationComplete();
}



unsigned int Core::getCycle() const{
    return _cycle;
}

DataMemory *Core::getDataMemory() const{
    return _dMem;
}

RegisterFile *Core::getRegisterFile() const{
    return _regFile;
}

ProgramCounter *Core::getProgramCounter() const{
    return _pc;
}

QList< int > Core::getRegistersDump(int lowerBound, int higherBound) const{
    QList< int > values;
    for(int i = lowerBound; i <= higherBound && i <= _regFile->_registers.size(); i++)
        values.append(_regFile->_registers.at(i)->getValue());
    return values;
}

QList< int> Core::getMemoryDump(int lowerBound, int higherBound) const{
    QList< int > values;
    for(int i = lowerBound; i <= higherBound && i <= _regFile->_registers.size(); i++)
        values.append(_dMem->_values.at(i));
    return values;
}

IFIDBuffer *Core::getIFID() const{
    return _iMem->getBuffer();
}

IDEXBuffer *Core::getIDEX() const{
    return _regFile->getBuffer();
}

EXMEMBuffer *Core::getEXMEM() const{
    return _ALU->getBuffer();
}

MEMWBBuffer *Core::getMEMWB() const{
    return _dMem->getBuffer();
}

bool Core::_if(){
    if(_pc->getValue() >= _iMem->getMemorySize()){
        qDebug() << "PC is at last instruction.";
        return false;
    }
    if(_instrQueue.size() == 5){
        qDebug() << "Queue is full.";
        return false;
    }
    qDebug() << "*+*+*+*Fetch*+*+*+*";
    auto fetched = _iMem->fetchInstruction();
    _instrQueue.prepend(fetched);
    _instructionNumbers[fetched] = _indexInstr++;
    qDebug() << "Fetched: " << *(fetched);
    _iMem->fetchInstruction()->setState(ExecState::IF);
    auto iMemBuf = _iMem->getBuffer();
    iMemBuf->setInstruction(*fetched);
    qDebug() << "Fetching PC: " << _pc->getDisplayValue();
    _pc->increment();
    qDebug() << "New PC: " << _pc->getDisplayValue();
    iMemBuf->setProgramCounter(*_pc);
    emit currentStage("IF", _cycle + _stalls, _instructionNumbers[fetched], fetched);
    return true;
}


Core::~Core(){

}
