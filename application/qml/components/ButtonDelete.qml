import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: control

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: "#FFFFFF"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        color: control.down ? "#ff7d47" : "#e64a19"
        implicitWidth: 96
        implicitHeight: 32
        opacity: enabled ? 1 : 0.3
        radius: 6
    }
}
