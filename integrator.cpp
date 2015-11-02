#include "integrator.h"
#include "wavefunction.h"
#include <QDebug>

Wavefunction *Integrator::wavefunction() const
{
    return m_wavefunction;
}

QVector<double> Integrator::potential() const
{
    return m_potential;
}

double Integrator::mass() const
{
    return m_mass;
}

Integrator::IntegrationScheme Integrator::scheme() const
{
    return m_scheme;
}

Domain *Integrator::domain() const
{
    return m_domain;
}

int Integrator::speed() const
{
    return m_speed;
}

double Integrator::dt() const
{
    return m_dt;
}

void Integrator::setWavefunction(Wavefunction *wavefunction)
{
    if (m_wavefunction == wavefunction)
        return;

    m_wavefunction = wavefunction;
    emit wavefunctionChanged(wavefunction);
}

void Integrator::setPotential(QVector<double> potential)
{
    if (m_potential == potential)
        return;

    m_potential = potential;
    emit potentialChanged(potential);
}

void Integrator::setMass(double mass)
{
    if (m_mass == mass)
        return;

    markDirty();
    m_mass = mass;
    emit massChanged(mass);
}

void Integrator::setScheme(Integrator::IntegrationScheme scheme)
{
    if (m_scheme == scheme)
        return;

    m_scheme = scheme;
    emit schemeChanged(scheme);
}

void Integrator::setDt(double dt)
{
    if (m_dt == dt)
        return;

    markDirty();
    m_dt = dt;
    emit dtChanged(dt);
}

void Integrator::setDomain(Domain *domain)
{
    if (m_domain == domain)
        return;
    // qDebug() << "Setting domain on integrator";
    m_domain = domain;
    markDirty();
    emit domainChanged(domain);
    connect(m_domain, SIGNAL(stepsChanged(int)), this, SLOT(markDirty()));
    connect(m_domain, SIGNAL(xMaxChanged(double)), this, SLOT(markDirty()));
    connect(m_domain, SIGNAL(xMinChanged(double)), this, SLOT(markDirty()));
}

void Integrator::markDirty()
{
    // qDebug() << "Marking dirty";
    m_dirty = true;
}

void Integrator::setSpeed(int speed)
{
    if (m_speed == speed)
        return;

    m_speed = speed;
    emit speedChanged(speed);
}

void Integrator::createLU() {
    // Transforms A = LU, here the elements are m_l (off diagonal of L) and m_v (diagonal of U)
    int size = m_wavefunction->values.size();
    m_l.resize(size);
    m_v.resize(size);
    m_oneOverV.resize(size);
    m_y.resize(size);

    c imag(0.0, 1.0);
    double dx = m_domain->deltaX();
    c diagonal = 1.0 + imag*m_dt/(m_mass*dx*dx);
    c offDiagonal = -imag*m_dt/(2.0*m_mass*dx*dx);

    int N = size-2;
    m_v[1] = diagonal;
    m_oneOverV[1] = 1.0/m_v[1];
    for(int k=2; k<=N; k++) {
        m_l[k] = offDiagonal / m_v[k-1];
        m_v[k] = diagonal - m_l[k]*offDiagonal;
        m_oneOverV[k] = 1.0/m_v[k];
    }
}

void Integrator::backwardEuler() {
    // Based on http://www.webpages.uidaho.edu/~barannyk/Teaching/LU_factorization_tridiagonal.pdf
    // We solve Ax=b as LUx = b => Ly = b
    Wavefunction &psi = *m_wavefunction;
    if(m_dirty) {
        createLU();
        m_dirty = false;
    }

    c imag(0.0, 1.0);
    double dx = m_domain->deltaX();
    c offDiagonal = -imag*m_dt/(2.0*m_mass*dx*dx);

    int N = psi.values.size()-2;
    // First solve Ly=b
    m_y[1] = psi[1] - m_potential[1];
    for(int k=2; k<=N; k++) {
        m_y[k] = (psi[k] - m_potential[k]) - m_l[k]*m_y[k-1];
    }

    // Now solve Ux = y
    m_newWaveFunction[N] = m_y[N]/m_v[N];
    for(int k=N-1; k>=1; k--) {
        m_newWaveFunction[k] = (m_y[k] - offDiagonal*m_newWaveFunction[k+1])*m_oneOverV[k];
    }

    std::swap(m_wavefunction->values, m_newWaveFunction);
    m_wavefunction->updateProbabilityDistribution();
}

void Integrator::eulerChromer() {
    qDebug() << "NOT IMPLEMENTED";
}

void Integrator::forwardEuler() {
    Wavefunction &psi = *m_wavefunction;
    c imag(0.0, 1.0);
    double dx = m_domain->deltaX();
    double oneOver2MDx2 = 1.0/(2.0*m_mass*dx*dx);

    for(size_t k=1; k<psi.values.size()-1; k++) {
        m_newWaveFunction[k] = psi[k] + imag*m_dt*( oneOver2MDx2*(psi[k+1] - 2.0*psi[k] + psi[k-1]) - m_potential[k]);
    }

    std::swap(m_wavefunction->values, m_newWaveFunction);
    m_wavefunction->updateProbabilityDistribution();
}

void Integrator::tick()
{
    if(!m_wavefunction) return;

    Wavefunction &psi = *m_wavefunction;
    if(psi.domain() != domain()) {
        qWarning() << "Warning, wavefunction and integrator does not have the same domain.";
        return;
    }

    if(m_domain->steps() != m_potential.size()) {
        m_potential.resize(m_domain->steps());
        m_domain->iterate([&](int k, double x) {
            double fromCenter = x - m_domain->center();
            m_potential[k] = 0.0;//0.01*fromCenter*fromCenter;
        });
    }

    if(m_newWaveFunction.size() != psi.values.size()) {
        m_newWaveFunction.resize(psi.values.size());
    }

    for(int i=0; i<m_speed; i++) {
        switch(m_scheme) {
        case FORWARD_EULER:
            forwardEuler();
            break;
        case BACKWARD_EULER:
            backwardEuler();
            break;
        }
    }
}
