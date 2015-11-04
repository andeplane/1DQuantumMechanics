#include "potential.h"

Potential::Potential(QObject *parent) : QObject(parent)
{

}

Domain *Potential::domain() const
{
    return m_domain;
}

void Potential::setDomain(Domain *domain)
{
    if (m_domain == domain)
        return;

    m_domain = domain;
    emit domainChanged(domain);
}

