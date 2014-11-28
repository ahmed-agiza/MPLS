#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include <QAbstractTableModel>

#include "datamemory.h"

class MemoryModel : public QAbstractTableModel
{
    Q_OBJECT
    DataMemory *_memory;
public:
    explicit MemoryModel(QObject *parent = 0, DataMemory * = 0);

    void setMemory(DataMemory *);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;
    Qt::ItemFlags flags(const QModelIndex &) const;

    ~MemoryModel();
signals:

public slots:

};

#endif // MEMORYMODEL_H
