#ifndef DOMAIN_H
#define DOMAIN_H
#include <QObject>
#include <functional>
using std::function;

class Domain : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double xMin READ xMin WRITE setXMin NOTIFY xMinChanged)
    Q_PROPERTY(double xMax READ xMax WRITE setXMax NOTIFY xMaxChanged)
    Q_PROPERTY(int steps READ steps WRITE setSteps NOTIFY stepsChanged)
private:
    double m_xMin = 0;
    double m_xMax = 10;
    int m_steps = 1024;

public:
    double xMin() const
    {
        return m_xMin;
    }
    double xMax() const
    {
        return m_xMax;
    }

    int steps() const
    {
        return m_steps;
    }

    double deltaX() const
    {
        return (m_xMax - m_xMin) / (m_steps-1);
    }

    double midpoint() const
    {
        return 0.5*(m_xMax + m_xMin);
    }

    double range() const
    {
        return (m_xMax-m_xMin);
    }

    void iterate(function<void(int i, double x)> func) {
        double dx = deltaX();
        for(int i=0; i<m_steps; i++) {
            double x = m_xMin + i*dx;
            func(i, x);
        }
    }



public slots:
    void setXMin(double xMin)
    {
        if (m_xMin == xMin)
            return;

        m_xMin = xMin;
        emit xMinChanged(xMin);
    }
    void setXMax(double xMax)
    {
        if (m_xMax == xMax)
            return;

        m_xMax = xMax;
        emit xMaxChanged(xMax);
    }

    void setSteps(int steps)
    {
        if (m_steps == steps)
            return;

        m_steps = steps;
        emit stepsChanged(steps);
    }

signals:
    void xMinChanged(double xMin);
    void xMaxChanged(double xMax);
    void stepsChanged(int steps);
};

#endif // DOMAIN_H
