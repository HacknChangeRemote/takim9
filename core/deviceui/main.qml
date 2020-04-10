import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

ApplicationWindow {
    width: 640
    height: 480
    visible: true

    color: "gray"

    Item {
        width: 100
        height: 100

//        x: parent.width / 2 - width/2
        anchors.right: parent.right
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

    ColumnLayout {
        anchors.fill: parent

        Text {
            Layout.fillWidth: true

            id: txt

            text: "H-ARF: Havelsan Augmented Reality Framework"
            color: "black"
            wrapMode: Text.WordWrap
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            Layout.fillWidth: true

            id: txtManufacturer
            text: "Manufacturer: " + mainData.manufacturer

            color: "black"
            wrapMode: Text.WordWrap
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: Qt.application.font.pixelSize * 1.3
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            Layout.fillWidth: true

            id: txtModel
            text: "Model: " + mainData.model

            color: "black"
            wrapMode: Text.WordWrap
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: Qt.application.font.pixelSize * 1.3
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            Layout.fillWidth: true

            id: txtDataPath
            text: "Data Path: " + mainData.dataPath

            color: "black"
            wrapMode: Text.WordWrap
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: Qt.application.font.pixelSize * 1.3
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            Layout.fillWidth: true

            id: txtDeviceType
            text: "Device Category: " + mainData.deviceType

            color: "black"
            wrapMode: Text.WordWrap
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: Qt.application.font.pixelSize * 1.3
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
