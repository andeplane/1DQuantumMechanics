#include "wavefunction.h"
#include <cmath>
#include <QDebug>
#include <iostream>
using std::cout; using std::endl;

Wavefunction::Wavefunction()
{
    m_saveMemory = false;
}

void Wavefunction::initGauss(double x0, double stddev)
{
    for(int i=0; i<m_domain->steps(); i++) {
        double x = m_domain->xMin() + i*m_domain->deltaX();
        values[i] = exp(-(x - x0)*(x - x0)/(2.0*stddev));
    }
    values.front() = 0;
    values.back() = 0;
}

void Wavefunction::updateProbabilityDistribution()
{
    // qDebug() << "Updating proability distribution";
    m_points.resize(m_domain->steps());

    // cout << "P(x) = [";
    for(int i=0; i<m_domain->steps(); i++) {
        // P(x) = |Psi|^2
        double x = m_domain->xMin() + i*m_domain->deltaX();
        double P = values[i].real()*values[i].real() + values[i].imag()*values[i].imag();
        m_points[i] = QPointF(x, P);
        // cout << P << " ";
    }
    // cout << "]" << endl;
    emit dataChanged();
}

Domain *Wavefunction::domain() const
{
    return m_domain;
}

void Wavefunction::reset() {
    if(!m_domain) return;
    // qDebug() << "Reset wavefunction";

    values.resize(m_domain->steps());
    double x0 = m_domain->center();
    double standardDeviation = 0.005*m_domain->range();
    initGauss(x0, standardDeviation);
    updateProbabilityDistribution();
}

void Wavefunction::setDomain(Domain *domain)
{
    if (m_domain == domain)
        return;

    // qDebug() << "Setting domain on wavefunction";
    m_domain = domain;
    reset();
    emit domainChanged(domain);
    connect(m_domain, SIGNAL(stepsChanged(int)), this, SLOT(reset()));
    connect(m_domain, SIGNAL(xMaxChanged(double)), this, SLOT(reset()));
    connect(m_domain, SIGNAL(xMinChanged(double)), this, SLOT(reset()));
}

