import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

ColumnLayout {
    id: root
    anchors.fill: parent

    property var globalState: "menuGames"
    signal changeState(var name)

    onActiveFocusChanged: {
	if(focus) 
	    headerMenuList.focus = true
    }

    ListView {
	Layout.alignment: Qt.AlignCenter
	
	Layout.minimumWidth: 300 * 3 + 40
        Layout.preferredHeight: 40
	spacing: 20

	Layout.fillWidth: true

	id: headerMenuList
	orientation: Qt.Horizontal
	state: currentIndex

	Component.onCompleted: {
	    currentIndex = 0
	    changeState("menuGames")
	}

	model: headerActions
	delegate: Item {
	    width: 300
	    height: 40

	    id: headerMenuDelegateItem
	    HeaderText {
		id: headerMenuDelegateText
		text: name
		anchors.centerIn: parent
		font.pointSize: 30
	    }

	    onActiveFocusChanged: {
		if(headerMenuList.currentIndex === index) {
		    headerMenuDelegateItem.state = "selected"
		    changeState(targetState)
		    globalState = targetState
		}
		else 
		    headerMenuDelegateItem.state = ""
	    }

	    states: [
		State {
		    name: "selected"
		    PropertyChanges {
			target: headerMenuDelegateText
			font.bold: true
			font.pointSize: 40
		    }
		}
	    ]
	}

	preferredHighlightBegin: width / 2 - 100
	preferredHighlightEnd: width / 2 + 100
	highlightRangeMode: ListView.StrictlyEnforceRange
    }

    ListModel {
	id: headerActions
	ListElement {
	    name: qsTr("Games")
	    targetState: "menuGames"
	}
	ListElement {
	    name: qsTr("Settings")
	    targetState: "menuSettings"
	}
    }
}
