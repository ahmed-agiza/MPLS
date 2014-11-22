#ifndef PROGRAMCOUNTER_H
#define PROGRAMCOUNTER_H

#include "register.h"

class ProgramCounter : public Register
{
    Q_OBJECT

    using Register::setName;
    bool _enabled;
public:
    ProgramCounter(QObject * = 0, Component * = 0, bool = false);

    void setEnabled(bool);
    bool isEnabled() const;

    void increment();

    int getDisplayValue() const;

    ~ProgramCounter();

signals:

public slots:

};

#endif // PROGRAMCOUNTER_H
