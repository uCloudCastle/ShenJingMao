import QtQuick 2.0

Image {
    id: container
    source: "qrc:///images/btn_start.png"
    width: 480
    height: 640
    signal start()

    MouseArea {
        width: container.width / 2
        height: container.height / 5

        anchors.centerIn: parent
        anchors.verticalCenterOffset: container.height / 3

        onClicked: {
            container.start();
        }
    }
}
