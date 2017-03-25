import QtQuick 2.0

LimitedAreaText {
    id: root
    property var author: qsTr("Unnamed")
    text: qsTr("Made by %1").arg(root.author)

    font.family: "Lato"
    font.pointSize: 30
    color: "lightgrey"
}
