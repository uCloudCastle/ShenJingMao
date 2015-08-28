import QtQuick 2.0

Image {
    id: containerImg
    source: victory === true ? "qrc:///images/victory.png" : "qrc:///images/failed.png"

    property int steps: 0
    property bool victory: true
    signal restart()

    FontLoader { id: localFont; source: "qrc:///ttf/MFDingDing.otf" }

    Text {
        id: text
        x: containerImg.width / 20
        y: containerImg.height / 2
        font { family: localFont.name; pointSize: 30 }
        color: "orangered"

        text: victory === true ? "你真棒! 一共用了 " + steps + " 步!" : "你没有抓住神经猫 ~(^(OO)^)~"
    }

    Image {
        id: btn
        anchors.bottom: containerImg.bottom
        anchors.right: containerImg.right
        anchors.margins: 40

        width: containerImg.width / 2
        height: containerImg.height / 4
        source: "qrc:///images/replay.png"

        MouseArea {
            anchors.fill: btn
            onClicked: {
                containerImg.restart();
            }
        }
    }
}
