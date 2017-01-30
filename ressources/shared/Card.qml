import QtQuick 2.0

Item {
    id: root

    property var gameColor: "green"
    property var gameName: "Unnamed"
    property var gameCoverURL: "https://cdn.shopify.com/s/files/1/0972/6232/files/no-image-placeholder.png"

    property int expandedWidth: 400
    property int collapsedWidth: 80

    property int numberOfCards: 3
    property int currentCard: 1
    property int ownIndex: 1

    states: [
	State {
	    name: "collapsedLeft"
	    PropertyChanges {
		target: root
		width: collapsedWidth
	    }
	    PropertyChanges {
		target: imageRotation
		angle: 30
		origin {
		    x: cover.x
		    y: cover.y + cover.height / 2
		}
	    }
	},
	State {
	    name: "collapsedRight"
	    PropertyChanges {
		target: root
		width: collapsedWidth
	    }
	    PropertyChanges {
		target: imageRotation
		angle: -30
		origin {
		    x: cover.x + cover.width
		    y: cover.y + cover.height / 2
		}
	    }
	}
    ]
    transitions: [
	Transition {
	    ParallelAnimation {
		PropertyAnimation {
		    properties: "width,x,y,origin.x,origin.y"
		    easing.type: Easing.InOutQuad
		    duration: 1000
		}
		RotationAnimation {
		    duration: 1000
		}
	    }
	}
    ]


    Image {
	id: cover
	anchors.fill: root
	asynchronous: true
	source: gameCoverURL

	transform: Rotation {
	    id: imageRotation
	    axis {
		x: 0
		y: 1
		z: 0
	    }
	    angle: 0
	}
    }
    onCurrentCardChanged: function(focus) {
	if(currentCard === ownIndex) {
	    root.state = ""
	}
	else if(currentCard > ownIndex) {
	    // Left side!
	    root.state = "collapsedLeft";
	} else {
	    // Right side!
	    root.state = "collapsedRight";
	}
    }
}
