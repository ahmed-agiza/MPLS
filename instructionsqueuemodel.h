#ifndef INSTRUCTIONSQUEUEMODEL_H
#define INSTRUCTIONSQUEUEMODEL_H

#include <QAbstractTableModel>

#include "instruction.h"

class InstructioQueueModel : public QAbstractTableModel
{
    Q_OBJECT
    QList<Instruction *> *_instructionsList;

public:
    explicit InstructioQueueModel(QObject * = 0, QList<Instruction *> * = 0);

    void setInstructionsList(QList<Instruction *> *);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;
    Qt::ItemFlags flags(const QModelIndex &) const;

    ~InstructioQueueModel();

signals:

public slots:

};

#endif // INSTRUCTIONSQUEUEMODEL_H
