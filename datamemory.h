#ifndef DATAMEMORY_H
#define DATAMEMORY_H

#include "component.h"
#include "memwbbuffer.h"

class DataMemory : public Component
{
    Q_OBJECT

    QList< int > _values;
    bool _writeEnabled;
    size_t _address;
    MEMWBBuffer *_buffer;

    friend class Core;
public:
    DataMemory(QObject * = 0, Component * = 0, size_t = 16, int = 0, bool = false, size_t = 0);

    MEMWBBuffer *getBuffer() const override;

    void setAddress(size_t);
    size_t getAddress() const;

    bool writeData(int);

    int getData() const;

    void setWriteEnabled(bool);
    bool isWriteEnabled() const;

    ~DataMemory();

signals:

public slots:

};

#endif // DATAMEMORY_H
