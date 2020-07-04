import QtQuick 2.14

// Warning: plase using .png format as button resource file

Image {
    id: btn
    
    property string btnImage: ""
    property string baseUrl: "qrc:/res/ui/"
    property real   triggerFactor: 1.5
    signal clicked()
    
    source: baseUrl + btnImage + ".png"
    anchors {
        left: parent.left
        leftMargin: utils.dp(10)
        top: parent.top
        topMargin: utils.dp(10)
    }
    MouseArea {
        width: parent.width * triggerFactor
        height: parent.height * triggerFactor
        anchors.centerIn: parent
        onPressedChanged: {
            if(pressed) {
                btn.source = baseUrl + btnImage + "_press.png"
            } else {
                btn.source = baseUrl + btnImage + ".png"
            }
        }
        onClicked: {
            btn.clicked()
        }
    }
}
