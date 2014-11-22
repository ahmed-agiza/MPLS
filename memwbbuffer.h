#ifndef MEMWBBUFFER_H
#define MEMWBBUFFER_H

#include "buffer.h"

class MEMWBBuffer : public Buffer
{
    Q_OBJECT

    int _memData;
    int _ALUResult;
public:
    MEMWBBuffer(QObject * = 0, Component * = 0, int = 0, int = 0);

    void clear() override;

    void setMemoryData(int);
    int getMemoryData() const;

    void setALUResult(int);
    int getALUResult() const;

    ~MEMWBBuffer();

signals:

public slots:

};

#endif // MEMWBBUFFER_H
