#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include "include.h"
#include "domain.h"
#include <QObject>
#include <QVector>
#include <vector>
using std::vector;

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
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
public:
    enum IntegrationScheme {
        FORWARD_EULER = 0,
        BACKWARD_EULER = 1
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
    vector<c> m_newWaveFunction;
    double m_mass = 1.0;
    double m_dt = 0.01;
    int m_speed = 1;
    bool m_dirty = true;
    IntegrationScheme m_scheme = BACKWARD_EULER;

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
    int speed() const;

signals:
    void wavefunctionChanged(Wavefunction* wavefunction);
    void potentialChanged(QVector<double> potential);
    void massChanged(double mass);
    void schemeChanged(IntegrationScheme scheme);
    void dtChanged(double dt);
    void domainChanged(Domain* domain);
    void speedChanged(int speed);

public slots:
    void setWavefunction(Wavefunction* wavefunction);
    void tick();
    void setPotential(QVector<double> potential);
    void setMass(double mass);
    void setScheme(IntegrationScheme scheme);
    void setDt(double dt);
    void setDomain(Domain* domain);
    void markDirty();
    void setSpeed(int speed);
};

#endif // INTEGRATOR_H
