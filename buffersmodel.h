#ifndef BUFFERSMODEL_H
#define BUFFERSMODEL_H

#include <QAbstractTableModel>

#include "ifidbuffer.h"
#include "idexbuffer.h"
#include "exmembuffer.h"
#include "memwbbuffer.h"

class BuffersModel : public QAbstractTableModel
{
    Q_OBJECT
    IFIDBuffer *_ifid;
    IDEXBuffer *_idex;
    EXMEMBuffer *_exmem;
    MEMWBBuffer *_memwb;

public:
    explicit BuffersModel(QObject * = 0, IFIDBuffer * = 0, IDEXBuffer * = 0, EXMEMBuffer * = 0, MEMWBBuffer * = 0);

    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;
    Qt::ItemFlags flags(const QModelIndex &) const;

    ~BuffersModel();
signals:

public slots:

};

#endif // BUFFERSMODEL_H
