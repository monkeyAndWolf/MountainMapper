import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15

import QtLocation 5.15
import QtPositioning 5.15

Item {

    RowLayout {
        width: parent.width
        height: 10
        TextArea {
            id: texto
            Layout.fillWidth: true
        }
        Button {
            id: button
            text: "Open a File"
            onClicked: {
                fd.open()
            }
        }
    }

    Connections {
        target: mdb
        function onDataTypesChanged(){
            // let's make the MAP happen
            mapBoy.visible = true
            comboBoy.model = mdb.getDataTypes();
            var doob = mdb.getACenterPoint();
            mapBoy.center = QtPositioning.coordinate(doob[0], doob[1])
            setUpNewType()
        }

        function onTypeChanged() {
            setUpNewType()
        }

        function onErrorMsg(error) {
            texto.text = error;
        }
    }

    function setUpNewType() {
        console.log("Shart")
    }

    Map {
        id: mapBoy
        anchors.fill: parent
        visible: false
        plugin: Plugin {
            id: mapPlugin
            name: "osm"
        }

        MapItemView {
            model: mdb
//            autoFitViewport: true
            anchors.fill: parent
            transitions: Transition {
                NumberAnimation { properties: "opacity"; from: 0; to: 1; duration: 300; easing.type: Easing.InOutCubic }
            }

            delegate: MapQuickItem {
                autoFadeIn: true
                coordinate: QtPositioning.coordinate(lat, longi)
                sourceItem: Item {
                    height: 6; width: 10
                    Rectangle {
                        height: 6; width: 6
                        radius: 3; color: "#781514"
                    }
                    Text {
                        text: name
                        x: 7
                        color: "#781614"
                    }
                }

            }
        }
    }

    ComboBox {
        id: comboBoy
        editable: false
        onCurrentTextChanged: {
            mdb.setType(currentText)
        }
    }

    FileDialog {
        id: fd
        selectMultiple: false
        onAccepted: {
            mdb.openFile(fd.fileUrl)
        }
    }
}
