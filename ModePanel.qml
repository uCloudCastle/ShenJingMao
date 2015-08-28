import QtQuick 2.0

Item {
    id: container
    property int mode: 1
//    height: 140
//    width:  320
    state: "inactive"
    property int diameter: height * 3 / 7
    property int spacing: diameter / 3
    signal hardLevelChanged(int lv)

    states: [
        State {
            name: "active"
            PropertyChanges {
                target: mainRect; color: "orangered";
            }
            PropertyChanges {
                target: eazyRect; y: spacing+diameter;
            }
            PropertyChanges {
                target: nomalRect; y: spacing+diameter;
            }
            PropertyChanges {
                target: hardRect; y: spacing+diameter;
            }
        },
        State {
            name: "inactive"
            PropertyChanges {
                target: mainRect; color: "lightgrey"
            }
            PropertyChanges {
                target: eazyRect; y: 0
            }
            PropertyChanges {
                target: nomalRect; y: 0
            }
            PropertyChanges {
                target: hardRect; y: 0
            }
        }
    ]

    Rectangle {
        id: mainRect
        height: diameter
        width: diameter
        radius: diameter / 2
        anchors.left: container.left
        anchors.bottom: container.bottom
        anchors.leftMargin: spacing

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if ( container.state == "active" )
                    container.state = "inactive";
                else
                    container.state = "active";
            }
        }
        Text {
            id: maintxt
            anchors.centerIn: parent
            color: "white"
            font { pointSize: 16 }
            text: "欢乐\n模式"
        }
    }

    Rectangle {
        id: eazyRect
        y: 0
        height: diameter
        width: diameter
        radius: diameter / 2
        color: 'lightgrey'
        x: spacing*2 + diameter

        MouseArea {
            anchors.fill: parent
            onPressed: parent.color = 'orangered';
            onReleased: {
                parent.color = 'lightgrey';
                container.state = "inactive";
                maintxt.text = txt1.text
                if ( mode !== 1 ){
                    mode = 1;
                    hardLevelChanged(mode)
                }
            }
            onExited: {
                parent.color = 'lightgrey';
            }
        }

        Behavior on y {
            NumberAnimation {
                duration: 400
                easing {
                    type: Easing.OutBounce
                    amplitude: 1.0
                } }
        }

        Text {
            id: txt1
            anchors.centerIn: parent
            color: "white"
            font { pointSize: 16 }
            text: "欢乐\n模式"
        }
    }

    Rectangle {
        id: nomalRect
        y: 0
        height: diameter
        width: diameter
        radius: diameter / 2
        color: 'lightgrey'
        x: spacing*3 + diameter*2

        MouseArea {
            anchors.fill: parent
            onPressed: parent.color = 'orangered';
            onReleased: {
                parent.color = 'lightgrey';
                container.state = "inactive";
                maintxt.text = txt2.text
                if ( mode !== 2 ){
                    mode = 2;
                    hardLevelChanged(mode)
                }
            }
            onExited: {
                parent.color = 'lightgrey';
            }
        }

        Behavior on y {
            NumberAnimation {
                duration: 600
                easing {
                    type: Easing.OutBounce
                    amplitude: 1.0
                } }
        }

        Text {
            id: txt2
            anchors.centerIn: parent
            color: "white"
            font { pointSize: 16 }
            text: "普通\n模式"
        }
    }

    Rectangle {
        id: hardRect
        y: 0
        height: diameter
        width: diameter
        radius: diameter / 2
        color: 'lightgrey'
        x: spacing*4 + diameter*3

        MouseArea {
            anchors.fill: parent
            onPressed: parent.color = 'orangered';
            onReleased: {
                parent.color = 'lightgrey';
                container.state = "inactive";
                maintxt.text = txt3.text
                if ( mode !== 3 ){
                    mode = 3;
                    hardLevelChanged(mode)
                }
            }
            onExited: {
                parent.color = 'lightgrey';
            }
        }

        Behavior on y {
            NumberAnimation {
                duration: 800
                easing {
                    type: Easing.OutBounce
                    amplitude: 1.0
                } }
        }

        Text {
            id: txt3
            anchors.centerIn: parent
            color: "white"
            font { pointSize: 16 }
            text: "烧脑\n模式"
        }
    }
}
