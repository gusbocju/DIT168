import QtQuick 2.0
import QtQuick.Window 2.1 // needed for the Window component

Window {

    width: 200
    height: 150

    Text {
        id: text1
        x: 27
        y: 29
        text: qsTr("Please enter the car ID:")
        wrapMode: Text.NoWrap
        font.pixelSize: 14
    }
    
    Rectangle {
        id: rectangle
        x: 50
        y: 62
        width: 101
        height: 27
        color: "#888a85"
        
        TextInput {
            id: textInput
            x: 8
            y: 4
            width: 80
            height: 20
            opacity: 1
            font.pixelSize: 12
            focus: true

        }
    }

    Rectangle {
        id: rectangle1
        x: 122
        y: 108
        width: 60
        height: 23
        color: "#838c73"

        Text {
            id: text2
            x: 8
            y: 4
            width: 52
            height: 15
            color: "#ffffff"
            text: qsTr("Connect")
            font.pixelSize: 12
        }

        MouseArea {
            id: mouseArea
            x: 0
            y: 0
            width: 60
            height: 23
        }
    }
}
