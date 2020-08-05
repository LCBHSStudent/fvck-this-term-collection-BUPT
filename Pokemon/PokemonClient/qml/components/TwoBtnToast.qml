import QtQuick 2.14
import QtQuick.Controls 2.14

Popup {
    id: toast_t
    x: (root.width - contentW) / 2
    y: (root.height - contentH) / 2
    width: 0; height: 0
    margins: 0; padding: 0
    modal: true; focus: true
    visible: opacity > 0
    opacity: 0
    
    signal clicked()
    
    Behavior on opacity {
        PropertyAnimation {
            duration: 150
        }
    }
    closePolicy: Popup.NoAutoClose
    
    property alias contentW: toastRect.width
    property alias contentH: toastRect.height
    
    signal confirmed()
    signal canceled()
    
    Rectangle {
        id: toastRect
        color: utils.colorClouds
        radius: utils.dp(5)
        
        MText {
            id: titleText
            font.pixelSize: utils.dp(22)
            color: utils.colorAsbestos
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: utils.dp(5)
        }
        
        MText {
            id: contentText
            horizontalAlignment: Text.AlignHCenter
            color: utils.colorAsbestos
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: titleText.bottom
            anchors.topMargin: utils.dp(6)
        }
        
        
        Rectangle {
            height: 1
            width: parent.width - 10
            color: "#DDDDDD"
            anchors {
                bottom: parent.bottom
                bottomMargin: parent.height * 0.25
                horizontalCenter: parent.horizontalCenter
            }
        }
        Rectangle {
            height: parent.height * 0.23
            width: 1
            color: "#DDDDDD"
            anchors {
                bottom: parent.bottom
                bottomMargin: parent.height * 0.01
                horizontalCenter: parent.horizontalCenter
            }
        }
        
        Row {
            spacing: utils.dp(20)
            anchors {
                bottom: parent.bottom
                bottomMargin: parent.height * 0.05
                horizontalCenter: parent.horizontalCenter
            }
            Repeater {
                model: 2
                delegate: Rectangle {
                    id: btn
                    height: text.contentHeight * 1.4
                    width:  toastRect.width * 0.35
                    radius: utils.dp(8)
                    color: {
                        if (index === 1)
                            return "#2CC486"
                        else
                            return utils.colorPomegranate
                    }
                    border.width: 2
                    border.color: {
                        if (index === 1)
                            return "#34D0C6"
                        else
                            return utils.colorAlizarin
                    }
                    
                    Behavior on scale {
                        PropertyAnimation {duration: 50}
                    }
                    Rectangle {
                        id: mask
                        color: "#FFFFFF"
                        anchors.fill: parent
                        opacity: btnArea.pressed? 0.3: 0
                        radius: utils.dp(8)
                        Behavior on opacity {
                            PropertyAnimation {duration: 50}
                        }
                    }
                    
                    MText {
                        id: text
                        text: index === 0? "取消": "确定"
                        font.pixelSize: utils.dp(16)                
                        anchors.centerIn: parent
                        color: "#EEEEEE"                
                    }
                    MouseArea {
                        id: btnArea
                        anchors.fill: parent
                        onClicked: {
                            if (index === 0) {
                                
                            }
                            else {
                                
                            }

                            toast_t.opacity = 0
                        }
                        onPressedChanged: {
                            btn.scale = pressed? 1.1: 1
                        }
                    }
                }
            }
        }
    }
    
    function showPopup(title, content) {
        titleText.text      = title
        contentText.text    = content
        toast_t.opacity     = 1
    }
}
