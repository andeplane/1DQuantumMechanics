TEMPLATE = app

QT += qml quick
CONFIG += c++11

INCLUDEPATH += /usr/local/Cellar/boost/1.58.0/include

SOURCES += main.cpp \
    wavefunction.cpp \
    figure.cpp \
    linegraph.cpp \
    graph.cpp \
    integrator.cpp \
    potential.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    wavefunction.h \
    include.h \
    linegraph.h \
    figure.h \
    graph.h \
    integrator.h \
    domain.h \
    potential.h

