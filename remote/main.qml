import QtQuick 2.9
import QtQuick.Window 2.2
import io.qt.examples.backend 1.0

Window {

    visible: true
    width: 232
    height: 232
    title: qsTr("Remote Control")

    BackEnd{
        id:backend;
    }

    Image {
        id: image1
        x: 101
        y: 39
        width: 30
        height: 30
        fillMode: Image.PreserveAspectFit
        source: "res/resources/forwardImg.png"
        MouseArea {
            anchors.fill: parent
            //onClicked: backend.moveW();
            onPressedChanged: pressed ? backend.moveW() : backend.stop();
        }
    }

    Image {
        id: image2
        x: 137
        y: 75
        width: 30
        height: 30
        fillMode: Image.PreserveAspectFit
        source: "res/resources/rightImg.png"
        MouseArea {
            anchors.fill: parent
           // onClicked: backend.moveD();
            onPressedChanged: pressed ? backend.moveD() : backend.stop();
        }

    }

    Image {
        id: image3
        x: 101
        y: 111
        width: 30
        height: 30
        fillMode: Image.PreserveAspectFit
        source: "res/resources/reverseImg.png"
        MouseArea {
            anchors.fill: parent
            //onClicked: backend.moveS();
            onPressedChanged: pressed ? backend.moveS() : backend.stop();
        }

    }

    Image {
        id: image4
        x: 65
        y: 75
        width: 30
        height: 30
        fillMode: Image.PreserveAspectFit
        source: "res/resources/leftImg.png"
        MouseArea {
            anchors.fill: parent
            //onClicked: backend.moveA();
            onPressedChanged: pressed ? backend.moveA() : backend.stop();
        }

    }

    Image {
        id: image
        x: 101
        y: 75
        width: 30
        height: 30
        source: "res/resources/stopButton.png"
    }

    Text {
        id: text3
        x: 133
        y: 180
        width: 66
        height: 17
    }

    Rectangle {
        id: rectangle
        x: 37
        y: 180
        width: 66
        height: 17

        property variant win;

        gradient: Gradient {
            GradientStop {
                position: 0.901
                color: "#ffffff"
            }

            GradientStop {
                position: 0.956
                color: "#ffffff"
            }

            GradientStop {
                position: 1
                color: "#34ae12"
            }
        }
        Text {
            id: text2
            x: 5
            y: 2
            text: qsTr("Start V2V")
            font.family: "Verdana"
            font.pixelSize: 12
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                        backend.startV2V();
                        var component = Qt.createComponent("V2VPopUp.qml");
                        var win = component.createObject(this);
                        win.show();
                    }
        }
    }

    Rectangle {
        id: rectangle1
        x: 133
        y: 180
        width: 66
        height: 17
        gradient: Gradient {
            GradientStop {
                position: 0.952
                color: "#ffffff"
            }

            GradientStop {
                position: 1
                color: "#ae1212"
            }
        }
        Text {
            id: text4
            x: 5
            y: 2
            text: qsTr("Stop V2V")
            font.pixelSize: 12
            font.family: "Verdana"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: backend.stopV2V()
        }
    }
}
