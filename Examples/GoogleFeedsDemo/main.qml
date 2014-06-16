import QtQuick 2.2
import QtQuick.Window 2.1

Window {
    objectName: "RootWindow"
    visible: true
    width: 400
    height: 800

    property var queryObject
    property var currentFeed

    Rectangle {
        id: searchBar
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
        color: 'darkgrey'
        height: 40
        radius: 20
        TextInput {
            id: inputField
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: parent.height
            text: ""
            color: 'white'
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height * 0.6
            onTextChanged: queryObject.query = text
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            font.italic: true
            font.pixelSize: parent.height * 0.6
            color: 'lightgrey'
            text: "Enter query here..."
            visible: inputField.text == ""
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchBar.bottom
        anchors.margins: 20

        ListView {
            id: resultsList
            model: queryObject.results
            anchors.fill: parent
            delegate: Rectangle {
                property var queryItem: modelData
                width: parent.width
                height: 60
                color: 'lightgrey'
                radius: 5
                Text {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 5
                    font.pixelSize: parent.height * 0.3
                    text: modelData.title
                    elide: Text.ElideRight
                }
                Text {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 5
                    font.pixelSize: parent.height * 0.15
                    text: modelData.link
                    elide: Text.ElideRight
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: resultsList.currentIndex = index
                }
            }
            //TODO: when count changes, index 0 is automatically selected, which triggers load
            //      need a way to prevent this..
            onCountChanged: {
                currentIndex = -1;
            }
            spacing: 5

            onCurrentIndexChanged: {
                if (currentIndex !== -1)
                    currentFeed = resultsList.currentItem.queryItem.feed();
            }
        }
    }

    Rectangle {
        id: feedView
        anchors.fill: parent
        visible: resultsList.currentIndex != -1

        Rectangle {
            id: close
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 20
            height: 40
            radius: 20
            width: 60
            color: 'lightgrey'
            Text { anchors.centerIn: parent; text: 'Close' }
            MouseArea { anchors.fill: parent; onClicked: resultsList.currentIndex = -1; }
        }

        Text {
            id: title
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: close.left
            anchors.margins: 20
            text: currentFeed.title
            font.pixelSize: 40
            wrapMode: Text.WordWrap
        }

        Text {
            id: description
            anchors.top: title.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            height: paintedHeight
            wrapMode: Text.WordWrap
            font.pixelSize: 20
            color: 'darkgrey'
            font.italic: true
            text: currentFeed.description
        }

        Text {
            id: siteLink
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            font.pixelSize: 12
            color: 'blue'
            font.underline: true
            text: currentFeed.feedUrl
            elide: Text.ElideRight
        }
    }
}
