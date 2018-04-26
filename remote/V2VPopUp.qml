import QtQuick 2.0
import QtQuick.Window 2.1 // needed for the Window component
import QtQuick.Controls 2.1



Window {

    id: window
    width: 200
    height: 150
    title: qsTr("V2V Connection")

    Text {
        id: text1
        x: 21
        y: 21
        text: qsTr("Please choose the car ID:")
        wrapMode: Text.NoWrap
        font.pixelSize: 14
    }

        Button {
            id: b1
            x: 30
            y: 55
            width: 23
            height: 20
            text: qsTr("1")
            onClicked: if (backend.group1()){

                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }

        Button {
            id: b2
            x: 59
            y: 55
            width: 23
            height: 20
            text: qsTr("2")
            onClicked: if (backend.group2()){

                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }
        Button {
            id: b3
            x: 88
            y: 55
            width: 23
            height: 20
            text: qsTr("3")
            onClicked: if (backend.group3()){

                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }

        Button {
            id: b4
            x: 118
            y: 55
            width: 23
            height: 20
            text: qsTr("4")
            onClicked: if (backend.group4()){

                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }

        Button {
            id: b5
            x: 147
            y: 55
            width: 23
            height: 20
            text: qsTr("5")
            onClicked: if (backend.group5()){
                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }

        Button {
            id: b6
            x: 45
            y: 81
            width: 23
            height: 20
            text: qsTr("6")
            onClicked: if (backend.group6()){
                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }

        Button {
            id: b7
            x: 74
            y: 81
            width: 23
            height: 20
            text: qsTr("7")
            onClicked: if (backend.group7()){
                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }

        Button {
            id: b8
            x: 103
            y: 81
            width: 23
            height: 20
            text: qsTr("8")
            onClicked: if (backend.group8()){
                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }
        Button {
            id: b9
            x: 133
            y: 81
            width: 23
            height: 20
            text: qsTr("9")
            onClicked: if (backend.group9()){
                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }

        Button {
            id: b10
            x: 39
            y: 107
            width: 35
            height: 20
            text: qsTr("10")
            onClicked: if (backend.group10()){

                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }

        Button {
            id: b11
            x: 83
            y: 107
            width: 35
            height: 20
            text: qsTr("11")
            onClicked: if (backend.group11()){

                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }

        Button {
            id: b12
            x: 127
            y: 107
            width: 35
            height: 20
            text: qsTr("12")
            onClicked: if (backend.group12()){

                       window.close();
                       } else {
                           var component = Qt.createComponent("errorMsg.qml");
                           var win = component.createObject(this);
                           win.show();
                       }
        }



}
