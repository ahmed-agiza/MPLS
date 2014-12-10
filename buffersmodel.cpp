#include "buffersmodel.h"

QString getPaddedHex(int, int);

#define SHOWB

BuffersModel::BuffersModel(QObject *parent, IFIDBuffer *ifid, IDEXBuffer *idex, EXMEMBuffer *exmem, MEMWBBuffer *memwb)
    :QAbstractTableModel(parent), _ifid(ifid), _idex(idex), _exmem(exmem), _memwb(memwb){
}


int BuffersModel::rowCount(const QModelIndex &) const{
    return 7;
}

int BuffersModel::columnCount(const QModelIndex &) const{
    return 5;
}

QVariant BuffersModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.row() < 0 || index.column() < 0 || index.column() > 3)
        return QVariant();

    if (role == Qt::DisplayRole){
        if (index.column() == 0){
            if (index.row() == 0)
                return "PC: ";
            else if (index.row() == 1)
                return "Instruction: ";
            else if (index.row() == 2)
                return "Register A: ";
            else if (index.row() == 3)
                return "Register B: ";
            else if (index.row() == 4)
                return "ALU Result: ";
            else if (index.row() == 5)
                return "Zero Flag: ";
            else if (index.row() == 6)
                return "Memory Data: ";
        }else if (index.column() == 1){
            if (index.row() == 0)
                return _ifid->getProgramCounter().getDisplayValue();
            else if (index.row() == 1)
                return _ifid->getInstruction().toString();
        }else if (index.column() == 2){
            if (index.row() == 0)
                return _idex->getProgramCounter().getDisplayValue();
            else if(index.row() == 1)
                return "N/A";
            else if (index.row() == 2)
                return _idex->getRegisterA().getValue();
            else if (index.row() == 3)
                return _idex->getRegisterB().getValue();
        }else if (index.column() == 3){
            if (index.row() < 3)
                return "N/A";
#ifdef SHOWB
            else if (index.row() == 3)
                return _exmem->getRegisterB().getValue();
#else
            else if (index.row() == 3)
                return "N/A";
#endif
            else if (index.row() == 4)
                return _exmem->getALUResult();
            else if (index.row() == 5)
                return _exmem->getZeroFlag();
        }else if (index.column() == 4){
            if (index.row() < 4 || index.row() == 5)
                return "N/A";
            else if (index.row() == 4)
                return _memwb->getALUResult();
            else if (index.row() == 6)
                return _memwb->getMemoryData();
        }
            return QVariant();
    }


    return QVariant();
}

QVariant BuffersModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal){
        switch (section) {
        case 0:
            return QVariant("Buffered Value");

        case 1:
            return QVariant("IF/ID");

        case 2:
            return QVariant("ID/EX");

        case 3:
            return QVariant("EX/MEM");

        case 4:
            return QVariant("MEM/WB");

        default:
            return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags BuffersModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index);
}

BuffersModel::~BuffersModel(){

}
