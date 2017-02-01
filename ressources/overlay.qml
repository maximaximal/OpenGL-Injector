import QtQuick 2.2
import QtQuick.Window 2.2
import QtQuick.Scene3D 2.0
import QtQuick.Layouts 1.3

import net.piga.overlay 1.0

import "shared"

Item {
    property real defaultSpacing: 10

    ColumnLayout {
	anchors.fill: parent

	HeaderBar {
	    id: headerBar
	    
	    Layout.fillWidth: true
	    Layout.preferredHeight: Screen.height / 6

	    KeyNavigation.down: cardDrawer
	    KeyNavigation.priority: KeyNavigation.BeforeItem

	    onChangeState: {
		console.log(name)
	    }
	}
	StackLayout {
	    currentIndex: 0

	    id: menuArea

	    width: Screen.width
	    height: Screen.height / 6 * 5
	    Layout.fillWidth: true
	    
	    CardDrawer {
		id: cardDrawer
		Layout.alignment: Qt.AlignCenter

		Layout.preferredWidth: menuArea.width
		Layout.preferredHeight: menuArea.height / 3 * 2

		KeyNavigation.up: headerBar
		KeyNavigation.priority: KeyNavigation.BeforeItem
	    }
	}
    }

    function displayNotification(note) {
        console.log("Notification Display! (QML)");
        console.log("Notification title: " + note.title + "; msg: " + note.msg);
    }
}
