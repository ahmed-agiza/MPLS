#include "core.h"




Core::Core(QObject * parent, QList<Instruction *> instructions)
    :Component(parent), _instructions(instructions), _cycle(0){
    _constructComponents();
}

void Core::_constructComponents(){
    _pc = new ProgramCounter(this, 0, true);
    _iMem = new InstructionMemory(this, _pc->getBuffer(), _instructions, _pc);
    _regFile = new RegisterFile(this, _iMem->getBuffer(), 32);
    _ALU = new ALU(this, _regFile->getBuffer());
    _dMem = new DataMemory(this, _ALU->getBuffer());
}

void printQueue(QList<Instruction *> &q){
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

    switch (instruction->getState()){
    case ExecState::IF:{
        qDebug() << "ID: " << *instruction;
        //Decode:
        if (instruction->isRInstruction()){
            qDebug() << "$rs = " << instruction->getRegisterRs();
            qDebug() << "$rt = " << instruction->getRegisterRt();
            qDebug() << "$rd = " << instruction->getRegisterRd();
            _regFile->setReadAddressA(instruction->getRegisterRs());
            _regFile->setReadAddressB(instruction->getRegisterRt());
            _regFile->setWriteAddress(instruction->getRegisterRd());
            if (instruction->getName() == InstructionName::ADD || instruction->getName() == InstructionName::SLT)
                _regFile->_registers[instruction->getRegisterRd()]->setState(RegisterState::WRITING);
            proceed = true;
        }else if (instruction->isIInstruction()){
            qDebug() << "$rs = " << instruction->getRegisterRs();
            qDebug() << "$rt = " << instruction->getRegisterRt();
            qDebug() << "imm = " << instruction->getImmediate();
            _regFile->setReadAddressA(instruction->getRegisterRs());
            _regFile->setReadAddressB(0);
            _regFile->setWriteAddress(instruction->getRegisterRt());
            if (instruction->getName() == InstructionName::ADDI || instruction->getName() == InstructionName::LW)
                _regFile->_registers[instruction->getRegisterRt()]->setState(RegisterState::WRITING);
            proceed = true;
        }else if (instruction->isJInstruction()){
            if (instruction->getName() == InstructionName::JAL){
                _regFile->setWriteAddress(31);
                _regFile->setWriteData(*_pc);
                _regFile->_registers[31]->setState(RegisterState::WRITING);
            }
            proceed = true;
        }else{
            qDebug() << "Unkown instruction format" << *instruction;
            exit(1);
        }

        qDebug() << "PC: " << _pc->getDisplayValue();
        regBuf->setProgramCounter(*_pc);
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
        if (instruction->isRInstruction()){
            qDebug() << "Operand A: " << _regFile->getReadDataA();
            qDebug() << "Operand B: " << _regFile->getReadDataB();
            _ALU->setOperandA(_regFile->getReadDataA());
            _ALU->setOperandB(_regFile->getReadDataB());
            switch (instruction->getName()){
            case InstructionName::ADD:
                _ALU->setOperation(Operation::ADD);
                break;
            case InstructionName::JR:
                _ALU->setOperation(Operation::NOP);
                _pc->setValue(_regFile->getReadDataA());
                break;
            case InstructionName::SLT:
                _ALU->setOperation(Operation::SLT);
                break;
            default:
                qDebug() << "Invalid instruction: " << *instruction;
                exit(1);
            }
            proceed = true;
        }else if (instruction->isIInstruction()){
            qDebug() << "Operand A: " << _regFile->getReadDataA();
            qDebug() << "Operand B: " << instruction->getImmediate();
            _ALU->setOperandA(_regFile->getReadDataA());
            _ALU->setOperandB(instruction->getImmediate());
            switch (instruction->getName()){
            case InstructionName::ADDI:
                _ALU->setOperation(Operation::ADD);
                break;
            case InstructionName::BLE:
                _ALU->setOperation(Operation::LE);
                if (_ALU->getResult())
                    _pc->setValue((*_pc) + instruction->getImmediate());
                break;
            case InstructionName::LW:
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
            qDebug() << "Jumping with " << *instruction;
            qDebug() << "Old PC: " << _pc->getDisplayValue();
            _pc->setValue(((int)(*_pc) & 0xF0000000) | (instruction->getImmediate() & 0x0FFFFFFF));
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
            instruction->setState(ExecState::MEM);
        else
            qDebug() << "Not procceeding from EX";
        qDebug() << "End of EX";
        return true;
        break;

    }case ExecState::MEM:{
        //Memory Read/Write:
        qDebug() << "MEM: " << *instruction;
        if (instruction->isRInstruction()){
            qDebug() << "R-Instruction has no effect on memory";
            proceed = true;
        }else if (instruction->isIInstruction()){
            switch (instruction->getName()) {
            case InstructionName::SW:
                qDebug() << "SW: ";
                qDebug() << "Write Data: " << _ALU->getBuffer()->getRegisterB().getValue();
                _dMem->setWriteEnabled(true);
                _dMem->writeData(_ALU->getBuffer()->getRegisterB());
                _dMem->setWriteEnabled(false);
                break;
            case InstructionName::LW:
                qDebug() << "LW: ";
                qDebug() << "Reg Write Data: " << _dMem->getData();
                _regFile->setWriteData(_dMem->getData());
                break;
            }           
            memBuf->setALUResult(_ALU->getResult());
            memBuf->setMemoryData(_dMem->getData());
            proceed = true;
        }else if (instruction->isJInstruction()){
            qDebug() << "J-Instruction has no effect on memory";
            proceed = true;
        }else{
            qDebug() << "Unkown instruction format" << *instruction;
            exit(1);
        }

        if (proceed)
            instruction->setState(ExecState::WB);
        else
            qDebug() << "Not procceeding from MEM";
        qDebug() << "End of MEM";
        return true;
        break;

    }case ExecState::WB:{
        //Regfile WriteBack:
        qDebug() << "WB: " << *instruction;
        if (instruction->isRInstruction()){
            if (instruction->getName() == InstructionName::ADD || instruction->getName() == InstructionName::SLT){
                qDebug() << "Write Data: " << _ALU->getResult();
                _regFile->setWriteData(_ALU->getResult());
                _regFile->setWriteEnabled(true);
                _regFile->writeData();
                _regFile->setWriteEnabled(false);
                _regFile->_registers[instruction->getRegisterRd()]->setState(RegisterState::STABLE);
            }
            proceed = true;
        }else if (instruction->isIInstruction()){
            if (instruction->getName() == InstructionName::LW){
                qDebug() << "Write Data: " << _ALU->getResult();
                _regFile->setWriteData(_ALU->getResult());
                _regFile->setWriteEnabled(true);
                _regFile->writeData();
                _regFile->setWriteEnabled(false);
                _regFile->_registers[instruction->getRegisterRt()]->setState(RegisterState::STABLE);
            }
            proceed = true;
        }else if (instruction->isJInstruction()){
            if (instruction->getName() == InstructionName::JAL){
                qDebug() << "Write Data(PC): " << _regFile->getWriteData();
                _regFile->setWriteEnabled(true);
                _regFile->writeData();
                _regFile->setWriteEnabled(false);
                _regFile->_registers[instruction->getRegisterRt()]->setState(RegisterState::STABLE);

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

    }case ExecState::COMP:{
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
    qDebug() << "Cycle: " << _cycle;
    qDebug() << "Queue before fetching: ";
    printQueue(_instrQueue);

    _if();

    qDebug() << "Queue after fetching: ";
    printQueue(_instrQueue);

    for(int i = _instrQueue.size() - 1; i > 0; i--){
        qDebug() << "*********** " << i << " ***********";
        if (!_execute(_instrQueue.at(i), i))
            break;
        qDebug() << "Numbers Map: " << Register::_regNumbers;
        qDebug() << "====================";
    }
    _cycle++;
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

bool Core::_if(){
    _instrQueue.prepend(_iMem->fetchInstruction());
    qDebug() << "Fetched: " << *(_iMem->fetchInstruction());
    _iMem->fetchInstruction()->setState(ExecState::IF);
    auto iMemBuf = _iMem->getBuffer();
    iMemBuf->setInstruction(*(_iMem->fetchInstruction()));
    qDebug() << "Fetching PC: " << _pc->getDisplayValue();
    _pc->increment();
    qDebug() << "New PC: " << _pc->getDisplayValue();
    iMemBuf->setProgramCounter(*_pc);
    return true;
}


Core::~Core(){

}
