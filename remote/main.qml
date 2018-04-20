import QtQuick 2.9
import QtQuick.Window 2.2
import io.qt.examples.backend 1.0

Window {

    visible: true
    width: 200
    height: 200
    title: qsTr("Remote Control")

    BackEnd{
        id:backend
    }

    Image {
        id: image1
        x: 85
        y: 46
        width: 30
        height: 30
        fillMode: Image.PreserveAspectFit
        source: "res/forwardImg.png"
        MouseArea {
                anchors.fill: parent
                onClicked: backend.moveW();
                onPressedChanged: parent.source = pressed ? "res/forwardImgPressed.png" : "res/forwardImg.png"
            }
    }

    Image {
        id: image2
        x: 114
        y: 75
        width: 30
        height: 30
        fillMode: Image.PreserveAspectFit
        source: "res/rightImg.png"
        MouseArea {
                anchors.fill: parent
                onClicked: backend.moveD();
                onPressedChanged: parent.source = pressed ? "res/rightImgPressed.png" : "res/rightImg.png"
            }

    }

    Image {
        id: image3
        x: 85
        y: 105
        width: 30
        height: 30
        fillMode: Image.PreserveAspectFit
        source: "res/reverseImg.png"
        MouseArea {
                anchors.fill: parent
                onClicked: backend.moveS();
                onPressedChanged: parent.source = pressed ? "res/reverseImgPressed.png" : "res/reverseImg.png"
            }

    }

    Image {
        id: image4
        x: 55
        y: 75
        width: 30
        height: 30
        fillMode: Image.PreserveAspectFit
        source: "res/leftImg.png"
        MouseArea {
            anchors.fill: parent
            onClicked: backend.moveA();
            onPressedChanged: parent.source = pressed ? "res/leftImgPressed.png" : "res/leftImg.png"
        }

    }

    Image {
        id: image
        x: 85
        y: 75
        width: 30
        height: 30
        source: "res/stopButton.png"
    }
}
