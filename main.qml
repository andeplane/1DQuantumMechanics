import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QMLPlot 1.0
import QM 1.0

Window {
    visible: true
    width: 1024
    height: 768
    Figure {
        id: figure
        anchors.fill: parent
        xMin: myDomain.xMin
        xMax: myDomain.xMax
        yMin: 0
        yMax: 1

        LineGraph {
            dataSource: wavefunction
        }

        Button {
            text: "Tick"
            onClicked: {
                integrator.tick()
            }
        }
    }

    Integrator {
        id: integrator
        wavefunction: Wavefunction {
            id: wavefunction
            domain: myDomain
        }
        dt: 0.001
        domain: Domain {
            id: myDomain
            xMin: 0
            xMax: 100
            steps: 128
        }

        // scheme: Integrator.BOOST_ODE
    }

    Timer {
        running: true
        repeat: true
        interval: 1
        onTriggered: {
            // integrator.tick()
        }
    }
}

