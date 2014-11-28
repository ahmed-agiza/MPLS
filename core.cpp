#include "core.h"




Core::Core(QObject * parent, QList<Instruction *> instructions)
    :Component(parent), _instructions(instructions), _cycle(0){
    _constructComponents();
}

void Core::_constructComponents(){
    _pc = new ProgramCounter(this, 0, true);
    _iMem = new InstructionMemory(this, _pc, _instructions, _pc);
    _regFile = new RegisterFile(this, _iMem);
    _ALU = new ALU(this, _regFile);
    _dMem = new DataMemory(this, _ALU);
}

bool Core::_execute(Instruction *instruction, int &index){
    bool proceed = false;

    switch (instruction->getState()){
    case ExecState::IF:{
        //Decode:
        if (instruction->isRInstruction()){
            _regFile->setReadAddressA(instruction->getRegisterRs());
            _regFile->setReadAddressB(instruction->getRegisterRt());
            _regFile->setWriteAddress(instruction->getRegisterRd());
            if (instruction->getName() == InstructionName::ADD || instruction->getName() == InstructionName::SLT)
                _regFile->_registers[instruction->getRegisterRd()]->setState(RegisterState::WRITING);
            proceed = true;
        }else if (instruction->isIInstruction()){
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

        IDEXBuffer *regBuf = _regFile->getBuffer();
        regBuf->setProgramCounter(*_pc);
        regBuf->setRegisterA(*(_regFile->_registers[instruction->getRegisterRs()]));
        regBuf->setRegisterB(*(_regFile->_registers[instruction->getRegisterRt()]));
        regBuf->setImmediate(instruction->getImmediate());

        if (proceed)
            instruction->setState(ExecState::ID);
        return true;
        break;

    }case ExecState::ID:{
        //Execute:
        if (instruction->isRInstruction()){
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
        }else if (instruction->isIInstruction()){
            _ALU->setOperandA(_regFile->getReadDataA());
            _ALU->setOperandB(instruction->getImmediate());
            switch (instruction->getName()){
            case InstructionName::ADDI:
                _ALU->setOperation(Operation::ADD);
                break;
            case InstructionName::BLE:
                _ALU->setOperation(Operation::LE);
                if (_ALU->getResult())
                    _pc->setValue((*_pc) + instruction->getImmediate() * 4);
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
            _pc->setValue(((*_pc) & 0xF0000000) | ((instruction->getImmediate() * 4) & 0x0FFFFFFF));
            proceed = true;
        }else{
            qDebug() << "Unkown instruction format" << *instruction;
            exit(1);
        }
        EXMEMBuffer *aluBuf = _ALU->getBuffer();
        aluBuf->setALUResult(_ALU->getResult());
        aluBuf->setZeroFlag(_ALU->getZeroFlag());
        aluBuf->setBranchTarget(_ALU->getResult());
        aluBuf->setRegisterB(*(_regFile->_registers.at(instruction->getRegisterRt())));
        if (proceed)
            instruction->setState(ExecState::MEM);
        return true;
        break;

    }case ExecState::MEM:{
        //Memory Read/Write:
        if (instruction->isRInstruction()){
            if (instruction->getName() == InstructionName::ADD || instruction->getName() == InstructionName::SLT)
                _regFile->setWriteData(_ALU->getResult());
            proceed = true;
        }else if (instruction->isIInstruction()){
            switch (instruction->getName()) {
            case InstructionName::SW:
                _dMem->setWriteEnabled(true);
                _dMem->writeData(_ALU->getBuffer()->getRegisterB());
                _dMem->setWriteEnabled(false);
                break;
            case InstructionName::LW:
                _regFile->setWriteData(_dMem->getData());
                auto buf = _dMem->getBuffer();
                buf->setALUResult(_ALU->getResult());
                buf->setMemoryData(_dMem->getData());
                break;
            }
            proceed = true;
        }else if (instruction->isJInstruction()){
            proceed = true;
        }else{
            qDebug() << "Unkown instruction format" << *instruction;
            exit(1);
        }

        if (proceed)
            instruction->setState(ExecState::WB);
        return true;
        break;

    }case ExecState::WB:{
        //Regfile WriteBack:
        if (instruction->isRInstruction()){
            if (instruction->getName() == InstructionName::ADD || instruction->getName() == InstructionName::SLT){
                _regFile->setWriteEnabled(true);
                _regFile->writeData();
                _regFile->setWriteEnabled(false);
                _regFile->_registers[instruction->getRegisterRd()]->setState(RegisterState::STABLE);
            }
            proceed = true;
        }else if (instruction->isIInstruction()){
            if (instruction->getName() == InstructionName::LW){
                _regFile->setWriteEnabled(true);
                _regFile->writeData();
                _regFile->setWriteEnabled(false);
                _regFile->_registers[instruction->getRegisterRt()]->setState(RegisterState::STABLE);
            }
            proceed = true;
        }else if (instruction->isJInstruction()){
            proceed = true;
        }else{
            qDebug() << "Unkown instruction format: " << *instruction;
            exit(1);
        }
        if (proceed){
            instruction->setState(ExecState::COMP);
            _instrQueue.removeFirst();
            index--;
        }
        return true;
        break;

    }case ExecState::COMP:{
        qDebug() << "There is a bug! \^-^/ Instruction is already executed! " << *instruction;
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
    _if();
    for(int i = 1; i < 5 && i < _instrQueue.size(); i++)
        if (!_execute(_instrQueue.at(i), i))
            break;
    _cycle++;
}

void Core::gotoCycle(unsigned int){

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
    _iMem->fetchInstruction()->setState(ExecState::ID);
    _pc->increment();
    auto buf = _iMem->getBuffer();
    buf->setInstruction(*(_iMem->fetchInstruction()));
    buf->setProgramCounter(*_pc);
    return true;
}


Core::~Core(){

}
