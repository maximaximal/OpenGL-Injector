import QtQuick 2.2
import QtQuick.Window 2.2
import QtQuick.Scene3D 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

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

	    onChangeState: function(newState) {
		if(newState == "menuGames") {
		    menuArea.push({item: cardDrawer, replace: true})
		    cardDrawer.focus = true
		} else if(newState == "menuSettings") {
		    menuArea.push({item: settings, replace: true})
		    settings.focus = true
		}
	    }
	}
	StackView {
	    id: menuArea
	    state: headerBar.globalState

	    width: Screen.width
	    height: Screen.height / 6 * 5
	    Layout.fillWidth: true

	    initialItem: cardDrawer

	    focus: true

	    CardDrawer {
		property int step: 0
		id: cardDrawer

		width: Screen.width
		height: Screen.height / 3 * 2
		
		KeyNavigation.up: headerBar
		KeyNavigation.priority: KeyNavigation.BeforeItem
	    }
	    Settings {
		property int step: 1
		id: settings

		width: Screen.width
		height: Screen.height / 3 * 2

		KeyNavigation.up: headerBar
		KeyNavigation.priority: KeyNavigation.BeforeItem

		// Opacity to 0 because of initial state.
		opacity: 0
	    }

	    delegate: StackViewDelegate {
		function getTransition(properties) {
		    if(properties.enterItem.step < properties.exitItem.step) {
			return leftToRight
		    } else {
			return rightToLeft
		    }
		}

		property Component leftToRight: StackViewTransition {
		    ParallelAnimation {
			ParallelAnimation {
			    PropertyAnimation {
				target: exitItem
				property: "opacity"
				from: exitItem.opacity
				to: 0
				duration: 600
			    }
			    PropertyAnimation {
				target: exitItem
				property: "x"
				from: 0
				to: Screen.width
				duration: 700
			    }
			}
			ParallelAnimation {
			    PropertyAnimation {
				target: enterItem
				property: "opacity"
				from: enterItem.opacity
				to: 1
				duration: 600
			    }
			    PropertyAnimation {
				target: enterItem
				property: "x"
				from: -Screen.width
				to: 0
				duration: 700
			    }
			}
		    }
		}
		property Component rightToLeft: StackViewTransition {
		    ParallelAnimation {
			ParallelAnimation {
			    PropertyAnimation {
				target: exitItem
				property: "opacity"
				from: exitItem.opacity
				to: 0
				duration: 600
			    }
			    PropertyAnimation {
				target: exitItem
				property: "x"
				from: 0
				to: -Screen.width
				duration: 700
			    }
			}
			ParallelAnimation {
			    PropertyAnimation {
				target: enterItem
				property: "opacity"
				from: enterItem.opacity
				to: 1
				duration: 600
			    }
			    PropertyAnimation {
				target: enterItem
				property: "x"
				from: Screen.width
				to: 0
				duration: 700
			    }
			}
		    }
		}
	    }
	}
    }

    function displayNotification(note) {
        console.log("Notification Display! (QML)");
        console.log("Notification title: " + note.title + "; msg: " + note.msg);
    }
}
