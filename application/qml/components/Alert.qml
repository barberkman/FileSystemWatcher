import QtQuick 2.15

Item {
    id: root

    /*
        0: primary
        1: secondary
        2: success
        3: danger
        4: warning
        else: info
    */
    property int alertID: 0

    property alias text: textID.text

    Rectangle {
        id: backgroundID
        anchors.fill: parent
        color: alertID == 0 ? "#319DFF"
                            : alertID == 1 ? "#7D8FA9"
                                           : alertID == 2 ? "#10D096"
                                                          : alertID == 3 ? "#FF316A"
                                                                         : alertID == 4 ? "#FFAA04"
                                                                                        : "#129199"
    }

    Image {
        id: iconID
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        source: {
            if (alertID == 3 || alertID == 4) { "qrc:/components/assets/warning.svg" }
            else { "qrc:/components/assets/info.svg" }
        }
    }

    Text {
        id: textID
        anchors.left: iconID.right
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        font.bold: true
        font.pixelSize: 14
        color: "#EEF0F4"
    }

}
