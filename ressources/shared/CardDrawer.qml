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
    }
    
    ListModel {
	id: games
	ListElement {
	    name: qsTr("Gamename")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	}
	ListElement {
	    name: qsTr("Gamename")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	}
	ListElement {
	    name: qsTr("Gamename")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	}
	ListElement {
	    name: qsTr("Gamename")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	}
	ListElement {
	    name: qsTr("Gamename")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	}
	ListElement {
	    name: qsTr("Gamename")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	}
	ListElement {
	    name: qsTr("Gamename")
	    coverUrl: "https://upload.wikimedia.org/wikipedia/commons/8/84/Example.svg"
	}
    }
}
