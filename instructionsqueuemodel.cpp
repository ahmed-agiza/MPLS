#include "instructionsqueuemodel.h"


QString getPaddedHex(int, int);

InstructioQueueModel::InstructioQueueModel(QObject *parent, QList<Instruction *> *instructionsList)
    :QAbstractTableModel(parent), _instructionsList(instructionsList){
}

void InstructioQueueModel::setInstructionsList(QList<Instruction *> *instructionsList){
    _instructionsList = instructionsList;
}

int InstructioQueueModel::rowCount(const QModelIndex &) const{
    return _instructionsList->size();
}

int InstructioQueueModel::columnCount(const QModelIndex &) const{
    return 1;
}

QVariant InstructioQueueModel::data(const QModelIndex &index, int role) const{

    if (!index.isValid() || index.row() >= _instructionsList->size() || index.row() < 0 || index.column() != 0)
        return QVariant();

    if (role == Qt::DisplayRole){
        if (index.column() == 0){
            return (_instructionsList->at(index.row()))->toString();
        }
        else
            return QVariant();
    }


    return QVariant();
}

QVariant InstructioQueueModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal){
        switch (section) {
        case 0:
            return QVariant("Instruction");

        default:
            return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags InstructioQueueModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index);
}

InstructioQueueModel::~InstructioQueueModel(){

}
