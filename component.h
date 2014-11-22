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
    State _state;
    Component * _sourceComponent;
    int _id;
    static int _lastID;
public:
    Component(QObject * = 0, Component * = 0, State = State::FREE);

    virtual Buffer * getBuffer() const;

    void getID() const;

    void setState(State);
    State getState() const;

    void setSourceComponent(Component *);
    Component * getSourceComponent() const;

    virtual ~Component();
signals:

public slots:

};

#endif // COMPONENT_H
