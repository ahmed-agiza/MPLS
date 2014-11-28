#ifndef REGISTERMODEL_H
#define REGISTERMODEL_H

#include <QAbstractTableModel>

#include "registerfile.h"
#include "programcounter.h"

class RegisterModel : public QAbstractTableModel
{
    Q_OBJECT
    RegisterFile *_regFile;
    ProgramCounter *_pc;
public:
    explicit RegisterModel(QObject * = 0, RegisterFile * = 0, ProgramCounter * = 0);

    void setRegisterFile(RegisterFile *);
    void setProgramCounter(ProgramCounter *);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;
    Qt::ItemFlags flags(const QModelIndex &) const;

    ~RegisterModel();
signals:

public slots:

};

#endif // REGISTERMODEL_H
