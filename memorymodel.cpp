#include "memorymodel.h"

QString getPaddedHex(int, int);

MemoryModel::MemoryModel(QObject *parent, DataMemory *memory)
    :QAbstractTableModel(parent), _memory(memory){
}

void MemoryModel::setMemory(DataMemory *memory){
    _memory = memory;
}

int MemoryModel::rowCount(const QModelIndex &) const{
    return _memory->_values.size();
}

int MemoryModel::columnCount(const QModelIndex &) const{
    return 2;
}

QVariant MemoryModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex()) || index.row() < 0 || index.column() < 0 || index.column() > 3)
        return QVariant();

    if (role == Qt::DisplayRole){
        if (index.column() == 0){
            return "0x" + getPaddedHex(index.row() * 4, 32);
        }else if (index.column() == 1){
            return "0x" + getPaddedHex(_memory->_values.at(index.row()), 32);
        }else
            return QVariant();
    }


    return QVariant();
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal){
        switch (section) {
        case 0:
            return QVariant("Address");

        case 1:
            return QVariant("Value");

        default:
            return QVariant();
        }
    }

    return QVariant();
}
Qt::ItemFlags MemoryModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index);
}

MemoryModel::~MemoryModel(){

}
