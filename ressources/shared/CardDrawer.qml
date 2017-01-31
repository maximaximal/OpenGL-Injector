import QtQuick 2.0

Item {
    id: root

    property int centerCardWidth: 400
    property int centerCardHeight: root.height * 0.95
    property int collapsedCardWidth: 90

    onActiveFocusChanged: function(focus) {
	if(focus) 
	    cardList.focus = true
    }

    Component {
        id: cardDelegate

        Card {
            id: card
            gameName: name
            gameColor: "purple"

	    width: centerCardWidth
	    height: centerCardHeight

	    expandedWidth: centerCardWidth
	    collapsedWidth: collapsedCardWidth

	    numberOfCards: cardList.count
	    currentCard: cardList.currentIndex
	    ownIndex: index
        }
    }

    ListView {
        id: cardList

        model: testModel

	anchors.fill: parent
        delegate: cardDelegate
	spacing: 10

	orientation: Qt.Horizontal
        layoutDirection: Qt.LeftToRight

	focus: true

	highlightMoveDuration: 400
	preferredHighlightBegin: width / 2 - centerCardWidth / 2
	preferredHighlightEnd: width / 2 + centerCardWidth / 2
	highlightRangeMode: ListView.StrictlyEnforceRange

	onCurrentIndexChanged: {
	    if(currentIndex > 0) 
		cardList.contentItem.children[currentIndex - 1].state = "collapsedLeft"
	    if(currentIndex < testModel.count - 1)
		cardList.contentItem.children[currentIndex + 1].state = "collapsedRight"


	    cardList.contentItem.children[currentIndex].state = ""
	}

	highlight: Component {
	    Rectangle {
		width: 100
		height: 100
		color: "green"

		Behavior on x {
		    NumberAnimation {
			easing.type: Easing.InOutQuad
			duration: 1000
		    }
		}
	    }
	}
    }
    ListModel {
        id: testModel
        ListElement {
            name: "Bill Smith"
            number: "555 3264"
        }
        ListElement {
            name: "John Brown"
            number: "555 8426"
        }
        ListElement {
            name: "Sam Wise"
            number: "555 0473"
        }
    }
}
