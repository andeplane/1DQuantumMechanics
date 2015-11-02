#ifndef WAVEFUNCTION_H
#define WAVEFUNCTION_H
#include <QVector>
#include <vector>
#include "include.h"
#include "linegraph.h"
#include "domain.h"
using std::vector;
class Wavefunction : public LineGraphDataSource
{
    Q_OBJECT
    Q_PROPERTY(Domain* domain READ domain WRITE setDomain NOTIFY domainChanged)
private:
    Domain* m_domain = nullptr;

public:
    vector<c> values;
    Wavefunction();
    void initGauss(double x0, double stddev);
    void updateProbabilityDistribution();
    c operator[](int index) { return values[index]; }
    Domain* domain() const;
public slots:
    void setDomain(Domain* domain);
    void reset();

signals:
    void domainChanged(Domain* domain);
};

#endif // WAVEFUNCTION_H
