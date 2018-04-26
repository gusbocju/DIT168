import QtQuick 2.0
import QtQuick.Window 2.1 // needed for the Window component

Window {

    id: window
    width: 220
    height: 80
    title: qsTr("Error Message")

    Text {
        id: text1
        x: 10
        y: 16
        text: qsTr("Connection not working!")
        font.pixelSize: 18
        color: "red"
    }
    Text {
        id: text2
        x: 71
        y: 42
        text: qsTr("Try again.")
        font.pixelSize: 18
        color: "red"
    }
}
