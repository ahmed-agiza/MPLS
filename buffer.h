#ifndef BUFFER_H
#define BUFFER_H
#include "component.h"


class Buffer : public Component
{
    Q_OBJECT
protected:
    QMap< QString, QString > _contentMap;

public:
    Buffer(QObject * = 0, Component * = 0);

    void setContent(QString, QString);
    QString getContent() const;
    bool hasContent(QString) const;

    virtual void clear() = 0;

    virtual ~Buffer();

signals:

public slots:

};

#endif // BUFFER_H
