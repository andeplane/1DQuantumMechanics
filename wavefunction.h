#ifndef WAVEFUNCTION_H
#define WAVEFUNCTION_H
#include <QVector>
#include <vector>
#include "include.h"
#include "linegraph.h"
#include "domain.h"
using std::vector;
class Wavefunction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Domain* domain READ domain WRITE setDomain NOTIFY domainChanged)
    Q_PROPERTY(LineGraphDataSource* dataSource READ dataSource NOTIFY dataSourceChanged)
private:
    Domain* m_domain = nullptr;
    LineGraphDataSource m_dataSource;

public:
    vector<c> values;
    Wavefunction();
    void initGauss(double x0, double stddev);
    void updateProbabilityDistribution();
    c operator[](int index) { return values[index]; }
    Domain* domain() const;
    LineGraphDataSource* dataSource();

public slots:
    void setDomain(Domain* domain);
    void reset();

signals:
    void domainChanged(Domain* domain);
    void dataSourceChanged(LineGraphDataSource *dataSource);
};

#endif // WAVEFUNCTION_H
