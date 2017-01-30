import QtQuick 2.2
import QtQuick.Window 2.2
import QtQuick.Scene3D 2.0
import QtQuick.Layouts 1.1

import net.piga.overlay 1.0

import "shared"

Item {
    property real defaultSpacing: 10

    ColumnLayout {
	anchors.fill: parent

	CardDrawer {
	    Layout.alignment: Qt.AlignCenter

	    centerCardWidth: Screen.width / 4
	    
	    height: (Screen.height / 3) * 2 // 2/3 of the screen should be covered.
	}
    }

    function displayNotification(note) {
        console.log("Notification Display! (QML)");
        console.log("Notification title: " + note.title + "; msg: " + note.msg);
    }
}
