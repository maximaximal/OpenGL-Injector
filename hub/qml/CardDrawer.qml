import QtQuick 2.1
import QtQuick.Layouts 1.1

Item {
    id: root

    property int expandedCoverWidth: root.width / 4

    onActiveFocusChanged: {
	if(focus) 
	    coverflow.focus = true
    }
    
    ColumnLayout {
	id: rootLayout
	anchors.fill: parent

	ListView {
	    id: coverflow

	    Layout.alignment: Qt.AlignCenter

	    Layout.preferredHeight: root.height / 4 * 3
	    Layout.fillWidth: true
	    spacing: - root.width / 16

	    orientation: Qt.Horizontal

	    model: games
	    focus: true

	    currentIndex: games.count / 2

	    highlightMoveDuration: 800
	    highlightResizeDuration: 800
	    highlightMoveVelocity: 100
	    highlightResizeVelocity: 100
	    preferredHighlightBegin: width / 2 - expandedCoverWidth / 2
	    preferredHighlightEnd: width / 2 + expandedCoverWidth / 2
	    highlightRangeMode: ListView.StrictlyEnforceRange
	    highlightFollowsCurrentItem: true

	    delegate: Item {

		width: expandedCoverWidth

		height: root.height / 4 * 3

		Image {
		    id: coverImage
		    source: activeFocus ? coverUrl : "https://www.trythisforexample.com/images/example_logo.png"

		    anchors.centerIn: parent
		    width: parent.width
		    height: parent.height

		    transform: Rotation {
			id: coverImageRotation
			origin.x: {
			    if(index < coverflow.currentIndex) {
				return coverImage.x
			    } else if(coverflow.currentIndex < index) {
				return coverImage.x + coverImage.width
			    } else {
				return coverImage.x + coverImage.width / 2
			    }
			}
			origin.y: coverImage.y + coverImage.height / 2
			axis {x: 0; y: 1; z: 0}
			angle: {
			    if(index < coverflow.currentIndex) {
				return 30
			    } else if(coverflow.currentIndex < index) {
				return -30
			    } else {
				return 0
			    }
			}

			Behavior on angle {
			    RotationAnimation {
				duration: 600
				easing.type: Easing.InOutQuad
			    }
			}
			Behavior on origin.x {
			    NumberAnimation {
				duration: 600
				easing.type: Easing.InOutQuad
			    }
			}
		    }
		    Behavior on width {
			NumberAnimation {
			    duration: 600
			}
		    }
		}
	    }
	}
	Rectangle {
	    Layout.preferredHeight: root.height / 4
	    Layout.fillWidth: true
	    color: "transparent"
	    HeaderText {
		id: gameName
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		text: games.get(coverflow.currentIndex).name
		font.pointSize: 50
		font.bold: true
		color: "darkgrey"
		horizontalAlignment: Text.AlignHCenter
	    }
	    GridLayout {
		id: gameInfo
		anchors.top: gameName.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		columns: 2
		Author {
		    id: gameAuthor
		    Layout.preferredWidth: parent.width / 3
		    Layout.maximumWidth: parent.width / 2
		    Layout.margins: 20
		    width: parent.width / 2
		    author: games.get(coverflow.currentIndex).author
		}
	    }
	}
    }
    
    ListModel {
	id: games
	ListElement {
	    name: qsTr("Gamename")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	    author: "Hello"
	}
	ListElement {
	    name: qsTr("Gamename 2")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	    author: "World"
	}
	ListElement {
	    name: qsTr("Gamename 3")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	    author: "LOL"
	}
	ListElement {
	    name: qsTr("Gamename 4")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	    author: "This"
	}
	ListElement {
	    name: qsTr("Gamename 5")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	    author: "Is"
	}
	ListElement {
	    name: qsTr("Gamename 6")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	    author: "Awesome"
	}
	ListElement {
	    name: qsTr("Gamename 7")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	    author: "Exclamation"
	}
    }
}
