#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "component.h"
#include "register.h"
#include "idexbuffer.h"


class RegisterFile : public Component
{
    Q_OBJECT

    QList< Register > _registers;
    size_t _readAddressA, _readAddressB, _writeAddress;
    bool _writeEnabled;
    IDEXBuffer * _buffer;
    static QMap< unsigned int, RegisterName > _regNums;
    static QMap< unsigned int, RegisterName > _regNumsStrings;
    static QMap< RegisterName, QString > _regNamesStrings;
    void _initializeRegisters();

public:
    RegisterFile(QObject * = 0, Component * = 0, size_t = 16, bool = false);

    IDEXBuffer * getBuffer() const override;

    void setReadAddressA(size_t);
    size_t getReadAddressA() const;

    void setReadAddressB(size_t);
    size_t getReadAddressB() const;

    void setWriteAddress(size_t);
    size_t getWriteAddress() const;

    void setWriteEnabled(bool);
    bool isWriteEnabled() const;

    int getReadDataA() const;

    int getReadDataB() const;

    bool writeData(int data);
    int getWriteData() const;

    ~RegisterFile() ;

    static QString getRegisterName(int);

    static QString getRegisterName(RegisterName);

signals:

public slots:

};

#endif // REGISTERFILE_H
