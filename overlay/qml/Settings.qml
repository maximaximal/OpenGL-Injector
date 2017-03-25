import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: root

    onActiveFocusChanged: {
    }

    ColumnLayout {
	id: rootLayout

	HeaderText {
	    text: "Settings!"
	}

	Rectangle {
	    width: 1000
	    height: 500
	    color: "yellow"
	}
    }
}
