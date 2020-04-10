import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Rectangle {
    id:root
    width: 1280
    height: 720
    visible: true

    color: "transparent"

    Item {
        width: 100
        height: 100

        x: parent.width / 2 - width/2
        anchors.bottom: parent.bottom

        Rectangle {
            id: tRect
            anchors.centerIn: parent
            width: 100
            height: 100
            border.color: "white"
            border.width: 2
            color: "transparent"

            Text{
                anchors.fill: parent
                text: "Exit"
                color: tRect.border.color
                font.bold: true
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onClicked: Qt.quit()
                onEntered: tRect.border.color = "orange"
                onExited: tRect.border.color = "white"
            }
        }
    }

    Text {
        id: txt

        anchors.fill: parent

        text: "H-ARF: Havelsan Augmented Reality Framework"
        color: "white"
        wrapMode: Text.WordWrap
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: Qt.application.font.pixelSize * 1.6
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: txtFps

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: 100

        text: "Fps: " + uiObject.fps
        color: "yellow"
        wrapMode: Text.WordWrap
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: Qt.application.font.pixelSize * 1.6
        horizontalAlignment: Text.AlignHCenter
    }
}
