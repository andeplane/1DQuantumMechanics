import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QMLPlot 1.0
import QM 1.0

Window {
    visible: true
    width: 1024
    height: 768
    ColumnLayout {
        anchors.fill: parent
        Figure {
            id: figure
            Layout.fillHeight: true
            Layout.fillWidth: true
            xMin: myDomain.xMin
            xMax: myDomain.xMax
            yMin: 0
            yMax: 0.1

            LineGraph {
                dataSource: wavefunction.dataSource
            }

            Button {
                text: "Tick"
                onClicked: {
                    integrator.tick()
                }
            }
        }
        Slider {
            id: slider
            Layout.fillWidth: true
            tickmarksEnabled: true
            value: -5
            stepSize: 0.1
            minimumValue: -5.0
            maximumValue: 0.0
        }
        Slider {
            id: slider2
            Layout.fillWidth: true
            tickmarksEnabled: true
            value: 1
            stepSize: 1
            minimumValue: 1
            maximumValue: 1000.0
        }
    }

    Integrator {
        id: integrator
        wavefunction: Wavefunction {
            id: wavefunction
            domain: myDomain
        }
        dt: Math.pow(10, slider.value)
        speed: slider2.value
        domain: Domain {
            id: myDomain
            xMin: 0
            xMax: 100
            steps: 1024
        }
        scheme: Integrator.BACKWARD_EULER
    }

    Timer {
        running: true
        repeat: true
        interval: 100
        onTriggered: {
            integrator.tick()
        }
    }
}

