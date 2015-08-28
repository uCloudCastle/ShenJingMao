import QtQuick 2.0
import Algorithm 1.0

Image {
    id: container
    source: "qrc:///images/bg.jpg"
    width: 480
    height: 640

    property int steps
    property int catIndex: 40
    property var activeRoundArray: []
    property var allRoundEntityArray: []
    property int diameter: container.width / 12
    property int stepLen: container.width / 10

    CatAlgorithm {
        id: catAlgo
    }

    EndPanel {
        id: endPanel
        width: container.width * 3 / 4
        height: container.height / 2
        z: 99
        scale: 0.001
        steps: container.steps

        Behavior on scale {
            NumberAnimation { duration: 600 }
        }
        Behavior on x {
            NumberAnimation { duration: 600 }
        }
        Behavior on y {
            NumberAnimation { duration: 600 }
        }
    }
    Connections {
        target: endPanel
        onRestart: {
            console.log("restart")
            spriteCat.x = container.width / 2 - diameter
            spriteCat.y = container.height - 4*stepLen - diameter - spriteCat.height*spriteCat.scale
            spriteCat.jumpTo("cat");
            endPanel.x = 0
            endPanel.y = 0
            endPanel.scale = 0.001
            catIndex = 40
            steps = 0
            activeRoundArray.length = 0;
            for ( var j = 0; j < allRoundEntityArray.length; ++j )
            {
                allRoundEntityArray[j].flush();
            }
        }
    }

    ModePanel {
        id: modePanel
        width: container.width * 3 / 4
        height: container.height * 7 / 32
        y: - height * 3 / 7
        anchors.left: container.left
        onHardLevelChanged: {
            catAlgo.hardLv = lv;
            endPanel.restart()
        }
    }

    Column {
        id: column
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: diameter / 2
        anchors.bottomMargin: diameter / 2
        anchors.rightMargin: diameter

        spacing: diameter / 5

        Repeater {
            id: mainRepeater
            model: 9

            Row {
                id: row
                spacing: diameter / 5
                property int columnIndex: index

                Item {
                    id: stretch
                    width: ( columnIndex % 2 ) * diameter * 2 / 5;    // 16 + 8 = 24 = ( 40 + 8 ) / 2
                    height: width
                }

                Repeater {
                    id: rowRepeater
                    model: 9

                    Rectangle {
                        id: round
                        height: diameter
                        width:  height
                        radius: height / 2
                        color: active === true ? 'orangered' : 'lightgrey'

                        property int roundIndex: columnIndex * 9 + index
                        property bool active: roundIndex !== 40 && Math.random() * 100 < 10 ? true : false

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if ( round.active === false ) {
                                    round.active = true;
                                    container.update();
                                }
                            }
                        }
                        Component.onCompleted:{
                            allRoundEntityArray.push(round)
                        }

                        onActiveChanged: {
                            if ( round.active === true ) {
                                activeRoundArray.push(roundIndex)
                                //console.log(roundIndex)
                            }
                        }
                        function flush(){
                            // this Assignment is must because onActiveChanged must be toggled with active round
                            active = false;
                            active = roundIndex !== 40 && Math.random() * 100 < 10 ? true : false
                        }
                    }
                }
            }
        }
    }  // Column End

    SpriteSequence {
        id: spriteCat
        width: sprite.frameWidth
        height: sprite.frameHeight
        interpolate: false
        scale: container.width / 480
        transformOrigin: Item.TopLeft

        // To ensure the cat always set in center
        x: container.width / 2 - diameter
        y: container.height - 4*stepLen - diameter - height*scale
        Behavior on x {
            NumberAnimation{ duration: 200 }
        }
        Behavior on y {
            NumberAnimation{ duration: 200 }
        }

        sprites: [
            Sprite {
                id: sprite
                name: "cat"
                source: "qrc:///images/stay.png"
                frameWidth: 61
                frameHeight: 93
                frameCount: 16
                frameDuration: 120
            },
            Sprite {
                name: "weizhu"
                source: "qrc:///images/weizhu.png"
                frameWidth: 64
                frameHeight: 91
                frameCount: 15
                frameDuration: 80
            }
        ]
    }

    function update() {
        var dir = catAlgo.getNextDirection(catIndex, activeRoundArray);
        if ( dir === 1 )
            toleft();
        else if ( dir === 2 )
            toleftUp();
        else if ( dir === 3 )
            torightUp();
        else if ( dir === 4 )
            toright();
        else if ( dir === 5 )
            torightDown();
        else if ( dir === 6 )
            toleftDown();

        // 神经猫被围住
        else if ( dir === 0 ) {
            spriteCat.jumpTo("weizhu");
            endPanel.victory = true
            endPanel.scale = 1.0;
            endPanel.x = container.width / 8
            endPanel.y = container.height / 3
        }

        // 神经猫逃脱
        if ( catIndex < 9 || catIndex > 72 || catIndex % 9 === 0 || ( catIndex + 1 ) % 9 === 0 ) {
            endPanel.victory = false
            endPanel.scale = 1.0;
            endPanel.x = container.width / 8
            endPanel.y = container.height / 3
        }
        steps += 1;
    }

    // 运动方向函数
    function toleft() {
        spriteCat.x -= stepLen;
        catIndex -= 1;
    }
    function toleftUp() {
        spriteCat.x -= stepLen/2;
        spriteCat.y -= stepLen;
        if ( Math.floor(catIndex / 9) % 2 === 0 ) // 偶数行向左上运动减10,奇数行减9
            catIndex -= 10;
        else
            catIndex -= 9;
    }
    function torightUp() {
        spriteCat.x += stepLen/2;
        spriteCat.y -= stepLen;
        if ( Math.floor(catIndex / 9) % 2 === 0 ) // 偶数行向右上运动减9,奇数行减8
            catIndex -= 9;
        else
            catIndex -= 8;
    }
    function toright() {
        spriteCat.x += stepLen;
        catIndex += 1;
    }
    function torightDown() {
        spriteCat.x += stepLen/2;
        spriteCat.y += stepLen;
        if ( Math.floor(catIndex / 9) % 2 === 0 ) // 偶数行向右下运动加9,奇数行加10
            catIndex += 9;
        else
            catIndex += 10;
    }
    function toleftDown() {
        spriteCat.x -= stepLen/2;
        spriteCat.y += stepLen;
        if ( Math.floor(catIndex / 9) % 2 === 0 ) // 偶数行向左下运动加8,奇数行加9
            catIndex += 8;
        else
            catIndex += 9;
    }
}
