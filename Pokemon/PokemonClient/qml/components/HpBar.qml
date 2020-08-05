import QtQuick 2.14
import QtQuick.Controls 2.14

Image {
    id: hpBar_t
    clip: true
    source: "qrc:/res/ui/hp_bar_bg.png"
    fillMode: Image.PreserveAspectFit
    property var curHP: 114
    property var maxHP: 114
    
    Item {
        clip: true
        y: 1; opacity: 0.75
        height: parent.height * 0.92
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.02
        width: parent.width * curHP / maxHP * 0.9685
        
        Rectangle {
            width: hpBar_t.width * 0.9685
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0
                    color: utils.colorAlizarin
                }
                GradientStop {
                    position: 1
                    color: "#54FF9F"
                }
            }
            radius: utils.dp(4)
            
            
            Behavior on width {
                PropertyAnimation {
                    duration: 400
                }
            }
            
            
        }
    }
    
    MText {
        text: "HP: " + curHP + " / " + maxHP
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -parent.height * 0.02
    }
    
    Image {
        id: hpBar_cover
        anchors.fill: parent
        source: "qrc:/res/ui/hp_bar_cover.png"
        fillMode: Image.PreserveAspectFit
    }
}
