import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: control

    placeholderTextColor: "#BAC4D1"
    color: "#EEF0F4"
    font.bold: true
    font.pixelSize: 14
    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 36
        color: control.enabled ? "transparent" : "#3B4758"
        border.color: control.enabled ? "#26394d" : "transparent"
        border.width: 2
        radius: 6
    }
}
