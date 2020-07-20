import QtQuick 2.14

Rectangle {
    id: flatBtn
    
    radius: utils.dp(5)
    color:  releaseColor
    height: btnText.font.pixelSize * 1.4
    width: btnText.width * 1.7
    
    property var    releaseColor
    property var    pressColor
    property alias  text: btnText.text
    property alias  textColor: btnText.color
    signal clicked()
    
    Behavior on color {
        PropertyAnimation { duration: 100 }
    }
    
    MText {
        id: btnText
        anchors.centerIn: parent
    }
    
    MouseArea {
        anchors.fill: parent
        onPressedChanged: {
            if(pressed) {
                color = pressColor
            } else {
                color = releaseColor
            }
        }
        onClicked: {
            flatBtn.clicked()
        }
    }
}
