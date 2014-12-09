#include "instructionmodel.h"

QString getPaddedHex(int, int);

InstructionModel::InstructionModel(QObject *parent, QList<Instruction *> *instructionsList)
    :QAbstractTableModel(parent), _instructionsList(instructionsList){
}

void InstructionModel::setInstructionsList(QList<Instruction *> *instructionsList){
    _instructionsList = instructionsList;
}

int InstructionModel::rowCount(const QModelIndex &) const{
    return _instructionsList->size();
}

int InstructionModel::columnCount(const QModelIndex &) const{
    return 3;
}

QVariant InstructionModel::data(const QModelIndex &index, int role) const{

    if (!index.isValid() || index.row() >= _instructionsList->size() || index.row() < 0 || index.column() < 0 || index.column() > 2)
        return QVariant();

    if (role == Qt::DisplayRole){
        if (index.column() == 0){
            return "0x" + getPaddedHex(index.row() * 4, 32);
        }else if (index.column() == 1){
            switch(_instructionsList->at(index.row())->getState()){
            case ExecState::UNDEF:
                return QVariant("Initial");
            case ExecState::IF:
                return QVariant("IF");
            case ExecState::ID:
                return QVariant("ID");
            case ExecState::EX:
                return QVariant("EX");
            case ExecState::MEM:
                return QVariant("MEM");
            case ExecState::WB:
                return QVariant("WB");
            case ExecState::COMP:
                return QVariant("Complete");
            }

        }else if (index.column() == 2){
            return QString(*(_instructionsList->at(index.row())));
        }
        else
            return QVariant();
    }


    return QVariant();
}

QVariant InstructionModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal){
        switch (section) {
        case 0:
            return QVariant("Address");

        case 1:
            return QVariant("Last Execution Stage");

        case 2:
            return QVariant("Instruction");

        default:
            return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags InstructionModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index);
}

InstructionModel::~InstructionModel(){

}

QString getPaddedHex(int number, int padding){
    QString hexNum = QString::number(number, 16);
    return hexNum.rightJustified(padding/4, '0', true).toUpper();
}
