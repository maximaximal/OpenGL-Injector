import QtQuick 2.0

Item {
    id: root

    property int centerCardWidth: 400
    property int centerCardHeight: root.height * 0.95
    property int collapsedCardWidth: 90

    width: cardList.count * (collapsedCardWidth - 1) + centerCardWidth
    
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
        anchors.fill: parent

        model: testModel

        delegate: cardDelegate
	spacing: 10

	orientation: Qt.Horizontal
        layoutDirection: Qt.LeftToRight
	focus: true
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
