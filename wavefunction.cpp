#include "wavefunction.h"
#include <cmath>
#include <QDebug>
#include <iostream>
using std::cout; using std::endl;

Wavefunction::Wavefunction()
{
    m_dataSource.setSaveMemory(false);
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
    QVector<QPointF> &points = m_dataSource.points();
    points.resize(m_domain->steps());

    for(int i=0; i<m_domain->steps(); i++) {
        // P(x) = |Psi|^2
        double x = m_domain->xMin() + i*m_domain->deltaX();
        double P = values[i].real()*values[i].real() + values[i].imag()*values[i].imag();
        points[i] = QPointF(x, P);
    }
    emit m_dataSource.dataChanged();
}

Domain *Wavefunction::domain() const
{
    return m_domain;
}

LineGraphDataSource *Wavefunction::dataSource()
{
    return &m_dataSource;
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

