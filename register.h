#ifndef REGISTER_H
#define REGISTER_H

#include "component.h"

enum class RegisterName {UNDEF, ZERO, AT, V0, V1, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7, S0, S1, S2, S3, S4, S5, S6, S7, T8, T9, K0, K1, GP, SP, FP, RA, PC, BUFF, CUSTOM};


class Register : public Component
{
    Q_OBJECT

protected:
    int _value;
    RegisterName _name;

public:
    Register(QObject * = 0, Component * = 0, int = 0, RegisterName = RegisterName::UNDEF);

    Register(const Register &) = delete;

    void setValue(const int);
    int getValue() const;

    void setName(RegisterName);
    RegisterName getName() const;

    operator int();

    Register operator+(const int&);

    Register &operator+=(const int&) ;

    Register &operator-=(const int&);

    Register &operator++();

    Register operator++(int);

    Register &operator--();

    Register operator--(int);

    Register &operator=(const int&);

    Register operator-(const int&);

    Register operator*(const int&);

    Register operator/(const int &);

    Register operator&(const int &);

    Register operator~();

    virtual ~Register();

signals:

public slots:

};

#endif // REGISTER_H
