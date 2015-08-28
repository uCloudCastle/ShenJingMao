import QtQuick 2.2
import QtQuick.Window 2.1

Window {
    id: window
    visible: true
    width: 480
    height: 640

    Loader { id: startPageLoader; anchors.fill: parent; source: 'qrc:///startPage.qml' }
    Loader { id: runPageLoader; anchors.fill: parent }

    Connections {
        target: startPageLoader.item
        onStart: {
            runPageLoader.source = 'qrc:///runPage.qml';
            startPageLoader.source = '';
        }
    }
}
