#ifndef POTENTIAL_H
#define POTENTIAL_H

#include <QObject>
#include "domain.h"
class Potential : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Domain* domain READ domain WRITE setDomain NOTIFY domainChanged)
private:
    Domain* m_domain = nullptr;

public:
    explicit Potential(QObject *parent = 0);
    Domain* domain() const;

signals:
    void domainChanged(Domain* domain);

public slots:
    void setDomain(Domain* domain);
};

#endif // POTENTIAL_H
