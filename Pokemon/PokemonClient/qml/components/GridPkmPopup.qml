import QtQuick 2.14
import QtQuick.Controls 2.14

import QtQuick 2.14
import QtQuick.Controls 2.14

Popup {
    id: toast_t
    width: 0; height: 0
    margins: 0; padding: 0
    modal: true; focus: true
    visible: opacity > 0
    opacity: 0
    
    signal confirmed()
    
    Behavior on opacity {
        PropertyAnimation {
            duration: 150
        }
    }
    closePolicy: Popup.NoAutoClose
    
    property int cellColumnCnt: 5
    
    property alias contentW: toastRect.width
    property alias contentH: toastRect.height
    property alias btnText:  text.text
    
    property ListModel pkmDataModel
    
    Rectangle {
        x: (root.width - contentW) / 2
        y: (root.height - contentH) / 2
        
        id: toastRect
        color: "#AAFAFAFA"
        radius: utils.dp(5)
        
        GridView {
            id: pkmView
            clip: true
            model: pkmDataModel
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: utils.dp(10)
                bottom: btn.top
                bottomMargin: utils.dp(10)
            }
        
            width: parent.width * 0.95
            cellWidth: width / cellColumnCnt
            cellHeight: cellWidth
            
            property int spacing: utils.dp(3)
            
            delegate: Rectangle {
                radius: utils.dp(3.5)
                width: pkmView.cellWidth - pkmView.spacing
                height: pkmView.cellHeight - pkmView.spacing
                color: "#AAFFFFFF"
                Behavior on color {
                    PropertyAnimation { duration: 100 }
                }
                
                Image {
                    id: pkmIco
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: -utils.dp(5)
                    source: "qrc:/res/image/character/" + typeId + ".png"
                }
                Text {
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: utils.dp(2)
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "微软雅黑"
                    font.pixelSize: utils.dp(9)
                    text: "No." + _id
                    color: "#AA1E90FF"
                }
                MouseArea {
                    anchors.fill: parent
                    onPressedChanged: {
                        if (pressed) {
                            parent.color = utils.colorClouds
                        } else {
                            parent.color = "#AAFFFFFF"
                        }
                    }
                    onClicked: {
                        pkmView.currentIndex = index
                        m_pkmInfoPopup.showPokemonInfo(
                            _id, typeId, name, level, type,
                            exp, attr,   atk,  def,   hp, spd,
                            skill_1, skill_2, skill_3, skill_4,
                            desc, desc_s1, desc_s2, desc_s3, desc_s4
                        )
                    }
                }
                
                Image {
                    id: mark
                    anchors {
                        top: parent.top
                        right: parent.right
                        margins: utils.dp(2)
                    }
                    width: parent.width * 0.35
                    height: width
                    source: "qrc:/res/ui/ic_target.png"
                    visible: index == pkmView.currentIndex
                }
            }
        }
        
        Rectangle {
            id: btn
            anchors {
                bottom: parent.bottom
                bottomMargin: utils.dp(10)
                horizontalCenter: parent.horizontalCenter
            }
            height: text.contentHeight * 1.4
            width:  parent.width * 0.6
            radius: utils.dp(8)
            color: "#2CC486"
            border.width: 2
            border.color: "#34D0C6"
            
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
                text: "选择完成"
                font.pixelSize: utils.dp(16)                
                anchors.centerIn: parent
                color: "#EEEEEE"                
            }
            MouseArea {
                id: btnArea
                anchors.fill: parent
                onClicked: {
                    toast_t.confirmed()
                }
                onPressedChanged: {
                    btn.scale = pressed? 1.1: 1
                }
            }
        }
    }
    
    function showPopup() {
        opacity = 1
    }
    
    function hidePopup() {
        opacity = 0
    }
    
    function getSelectedPkmId() {
        return pkmDataModel.get(pkmView.currentIndex)._id
    }
    
    PkmInfoPopup {
        id: m_pkmInfoPopup
    }
}
