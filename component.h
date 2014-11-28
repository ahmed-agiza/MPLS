#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>
#include <QDebug>

enum class State{FULL_BUSY, HALF_BUSY, FREE};

class Buffer;

class Component : public QObject
{
    Q_OBJECT
protected:
Component *_sourceComponent;
    State _state;
    int _id;
    static int _lastID;
    int _lockOwner;
public:
    Component(QObject * = 0, Component * = 0);

    virtual Buffer * getBuffer() const;

    bool lock(int);
    bool halfLock(int);
    bool unlock(int);
    int getLockOwnerID() const;
    bool isLocked() const;
    bool isHalfLocked() const;

    int getID() const;

    void setState(State);
    State getState() const;

    void setSourceComponent(Component *);
    Component * getSourceComponent() const;

    virtual ~Component();
signals:

public slots:

};

#endif // COMPONENT_H
