import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import "." // for Style.qml
import "components" as CustomControls

Item {
    id: root

    onWidthChanged: eventWatcherTableID.forceLayout()

//    Material.theme: Material.Dark
//    Material.accent: Material.Amber
//    Material.primary: Material.Teal
//    Material.background: Material.Teal

    Rectangle {
        id: backgroundID
        anchors.fill: parent
        color: Style.backgroundColor
    }

    CustomControls.ButtonPrimary {
        id: addButtonID
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 10
        text: "Add"
        onClicked: { addFolder(searchPathID.text); }
    }

    CustomControls.ButtonImage {
        id: folderSelectID
        anchors.right: addButtonID.left
        anchors.rightMargin: 10
        anchors.verticalCenter: addButtonID.verticalCenter
        source: "qrc:/components/assets/folder_button.svg"
        onClicked: { fileDialogID.open(); }
    }

    CustomControls.SearchField {
        id: searchPathID
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: folderSelectID.left
        anchors.rightMargin: 10
        anchors.verticalCenter: addButtonID.verticalCenter
        placeholderText: "Add path to watch"
        Keys.onReturnPressed: { addFolder(searchPathID.text); }
    }

    CustomControls.TextHeader {
        id: wathedDirsHeaderID
        text: qsTr("Watched paths")
        anchors.left: searchPathID.left
        anchors.leftMargin: 10
        anchors.top: searchPathID.bottom
        anchors.topMargin: 20
    }

    ListView {
        id: wathedDirsListID
        anchors.left: wathedDirsHeaderID.left
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: wathedDirsHeaderID.bottom
        anchors.topMargin: 10
        height: 150
        model: FSWatcherModel
        clip: true

        delegate: Item {
            id: wathedDirDelegateID
            width: parent.width
            height: 40

            Rectangle {
                id: wathcedDirsBackgroundID
                anchors.fill: parent
                color: index % 2 == 0 ? Style.rowEvenColor : Style.rowOddColor
            }

            Text {
                id: wathcedDirsTextID
                text: path
                font.bold: true
                font.pixelSize: 14
                color: "#EEF0F4"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
            }

            CustomControls.ButtonDelete {
                id: deleteButtonID
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                text: "Remove"
                onClicked: { FSWatcherModel.removeFolder(index); }
            }
        }
    }

    Row {
        id: eventWatcherTableHeaderRowID
        y: eventWatcherTableID.contentY

        anchors.top: wathedDirsListID.bottom
        anchors.topMargin: 20
        anchors.left: wathedDirsListID.left
        anchors.right: wathedDirsListID.right

        Repeater {
            model: eventWatcherTableID.columns > 0 ? eventWatcherTableID.columns : 1
            Item {
                id: eventWatcherTableHeaderID
                height: 30
                width: index === 0 ? eventWatcherTableID.width * eventWatcherTableID.col0
                                                              : index === 1 ? eventWatcherTableID.width * eventWatcherTableID.col1
                                                                                   : index === 2 ? eventWatcherTableID.width * eventWatcherTableID.col2
                                                                                                        : eventWatcherTableID.width * eventWatcherTableID.col3

                Rectangle {
                    anchors.fill: parent
                    color: "#333333"
                    border.width: 1
                    border.color: "#26394d"
                }

                Text {
                    text: FSEventWatcherModel.headerData(modelData, Qt.Horizontal)
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                    font.pixelSize: 14
                    color: "#aaaaaa"
                    wrapMode: Text.Wrap
                }
            }
        }
    }

    TableView {
        id: eventWatcherTableID

        readonly property real col0: 0.2
        readonly property real col1: 0.4
        readonly property real col2: 0.2
        readonly property real col3: 0.2

        anchors.top: eventWatcherTableHeaderRowID.bottom
        anchors.topMargin: 0
        anchors.left: eventWatcherTableHeaderRowID.left
        anchors.right: eventWatcherTableHeaderRowID.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        model: FSEventWatcherModel
        clip: true

        delegate: Item {
            id: eventWatcherTableDelegateID
            implicitWidth: model.column === 0 ? eventWatcherTableID.width * eventWatcherTableID.col0
                                              : model.column === 1 ? eventWatcherTableID.width * eventWatcherTableID.col1
                                                                   : model.column === 2 ? eventWatcherTableID.width * eventWatcherTableID.col2
                                                                                        : eventWatcherTableID.width * eventWatcherTableID.col3
            implicitHeight: 30

            Rectangle {
                id: eventWatcherTableBackgroundID
                anchors.fill: parent
                color: model.row % 2 == 0 ? Style.rowEvenColor : Style.rowOddColor
                border.width: 1
                border.color: "#26394d"
            }

            Text {
                id: eventWatcherTableTextID
                text: display
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                maximumLineCount: 1
                font.bold: true
                font.pixelSize: 12
                color: "#EEF0F4"
                wrapMode: Text.WrapAnywhere
            }
        }
    }

    CustomControls.ButtonSecondary {
        id: clearButtonID
        anchors.left: wathedDirsHeaderID.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        text: "Clear"
        onClicked: { FSWatcherModel.clearFolders(); }
    }

    CustomControls.ButtonSecondary {
        id: startButtonID
        anchors.left: clearButtonID.right
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        text: "Start"
        onClicked: { FSWatcherModel.startWatching(); }
    }

    CustomControls.ButtonSecondary {
        id: stopButtonID
        anchors.left: startButtonID.right
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        text: "Stop"
        onClicked: { FSWatcherModel.stopWatching(); }
    }

    CustomControls.Alert {
        id: alertID
        anchors.left: stopButtonID.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: stopButtonID.verticalCenter
        alertID: FSWatcherModel.status === 0 ? 2
                                             : 5
        text: FSWatcherModel.status === 0 ? "Folders watching"
                                          : "Folders not watching"
    }

    FileDialog {
        id: fileDialogID
        title: "Directory Selector"
        folder: shortcuts.documents
        selectFolder: true
        onAccepted: { searchPathID.text = folder; }
    }

    function addFolder(url) {
        searchPathID.clear();
        FSWatcherModel.addFolder(url);
    }
}
