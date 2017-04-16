import QtQuick 2.0

Flickable {
    id: root
    pixelAligned: true
    clip: true
    
    contentWidth: contentItem.childrenRect.width;
    contentHeight: contentItem.childrenRect.height
    height: bodyText.height
    
    contentX: 0
    contentY: 0
    
    property var text: ""
    property font font: bodyText.font
    property color color: bodyText.color

    Text {
	id: bodyText
	font: root.font
	color: root.color
	text: root.text

	anchors.left: parent.left
    }

    NumberAnimation on contentX {
	id: scrollAnim
	loops: Animation.Infinite
	from: 0
	to: root.contentWidth
	duration: (root.contentWidth / 1000) * 15000 // 1000 pixels in 15 seconds.
	running: {
	    if(contentWidth > width) {
		return true
	    } else {
		return false
	    }
	}
    }
}
