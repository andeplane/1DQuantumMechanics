#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "wavefunction.h"
#include "figure.h"
#include "linegraph.h"
#include "integrator.h"
#include "domain.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<Figure>("QMLPlot", 1, 0, "Figure");
    qmlRegisterType<LineGraph>("QMLPlot", 1, 0, "LineGraph");
    qmlRegisterType<Wavefunction>("QM", 1, 0, "Wavefunction");
    qmlRegisterType<Integrator>("QM", 1, 0, "Integrator");
    qmlRegisterType<Domain>("QM", 1, 0, "Domain");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

