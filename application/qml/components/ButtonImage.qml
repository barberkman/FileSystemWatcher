import QtQuick 2.15

Image {
    id: control

    signal clicked

    opacity: mouseAreaID.pressed ? 0.6 : 1.0

    MouseArea {
        id: mouseAreaID
        anchors.fill: parent
        onClicked: { control.clicked() }
    }
}
