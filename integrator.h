#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include "include.h"
#include "domain.h"
#include <QObject>
#include <QVector>
#include <boost/numeric/odeint.hpp>
using namespace boost::numeric::odeint;
namespace pl = std::placeholders;
typedef QVector<c> state_type;

class Wavefunction;
class Integrator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Wavefunction* wavefunction READ wavefunction WRITE setWavefunction NOTIFY wavefunctionChanged)
    Q_PROPERTY(QVector<double> potential READ potential WRITE setPotential NOTIFY potentialChanged)
    Q_PROPERTY(double mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(IntegrationScheme scheme READ scheme WRITE setScheme NOTIFY schemeChanged)
    Q_PROPERTY(double dt READ dt WRITE setDt NOTIFY dtChanged)
    Q_PROPERTY(Domain* domain READ domain WRITE setDomain NOTIFY domainChanged)
public:
    enum IntegrationScheme {
        FORWARD_EULER = 0,
        BACKWARD_EULER = 1,
        BOOST_ODE = 2
    };
    Q_ENUMS(IntegrationScheme)
private:
    Wavefunction* m_wavefunction = nullptr;
    Domain* m_domain = nullptr;
    QVector<double> m_potential;
    QVector<c> m_y;
    QVector<c> m_l;
    QVector<c> m_v;
    QVector<c> m_oneOverV;
    QVector<c> m_newWaveFunction;
    void rhs(const state_type &x, state_type &dxdt, double t);
    void callback(const state_type &x, const double t);
    double m_mass = 1.0;
    double m_dt = 0.01;
    bool m_dirty = true;
    IntegrationScheme m_scheme = FORWARD_EULER;

    void forwardEuler();
    void backwardEuler();
    void eulerChromer();
    void createLU();

public:
    double mass() const;
    double dt() const;
    Wavefunction* wavefunction() const;
    QVector<double> potential() const;
    IntegrationScheme scheme() const;
    Domain* domain() const;

signals:
    void wavefunctionChanged(Wavefunction* wavefunction);
    void potentialChanged(QVector<double> potential);
    void massChanged(double mass);
    void schemeChanged(IntegrationScheme scheme);
    void dtChanged(double dt);
    void domainChanged(Domain* domain);

public slots:
    void setWavefunction(Wavefunction* wavefunction);
    void tick();
    void setPotential(QVector<double> potential);
    void setMass(double mass);
    void setScheme(IntegrationScheme scheme);
    void setDt(double dt);
    void setDomain(Domain* domain);
    void markDirty();
};

#endif // INTEGRATOR_H
