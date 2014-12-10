#include "registermodel.h"

QString getPaddedHex(int, int);

RegisterModel::RegisterModel(QObject *parent, RegisterFile *source, ProgramCounter *pc)
    :QAbstractTableModel(parent), _regFile(source), _pc(pc){
}

void RegisterModel::setRegisterFile(RegisterFile *source){
    _regFile = source;
}

void RegisterModel::setProgramCounter(ProgramCounter *pc){
    _pc = pc;
}

int RegisterModel::rowCount(const QModelIndex &) const{
    return (_regFile->_registers.size() + (_pc != 0));
}

int RegisterModel::columnCount(const QModelIndex &) const{
    return 3;
}

QVariant RegisterModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.row() < 0 || index.column() < 0 || index.column() > 3)
        return QVariant();
    qDebug() << _pc << "  " << _pc->getValue() << "  " << _pc->getDisplayValue();
    if (role == Qt::DisplayRole){
        if (index.column() == 0){
            if (index.row() != 32)
                return Register::getRegisterNameString(index.row());
            else if (_pc)
                return "PC";
        }else if (index.column() == 1){
            if (index.row() < 32)
                return index.row();
            else
                return QVariant();
        }else if (index.column() == 2){
            if (index.row() != 32)
                return "0x" + getPaddedHex(_regFile->_registers.at(index.row())->getValue(), 32);
            else if (_pc)
                return "0x" +  getPaddedHex(_pc->getDisplayValue(), 32);;
        }
            return QVariant();
    }


    return QVariant();
}

QVariant RegisterModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal){
        switch (section) {
        case 0:
            return QVariant("Register Name");

        case 1:
            return QVariant("Register Number");

        case 2:
            return QVariant("Value");

        case 3:
            return QVariant("State");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index);
}

RegisterModel::~RegisterModel(){

}
