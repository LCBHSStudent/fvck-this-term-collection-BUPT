import QtQuick 2.14
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.14
import QtQuick.Shapes 1.14
import QtMultimedia 5.14

import "../components/"

Page {
    id: mainPage_t
    
    readonly property color pageColor: "#AAFAFAFA"
    
    // ------------BACKGROUND -------------- //
    Image {
        source: "qrc:/res/image/bg2.png"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }
    Rectangle {
        id: bgMask
        anchors.fill: parent
        color: "#FFFFFF"
        opacity: 0
        SequentialAnimation {
            running: true
            loops: Animation.Infinite
            PropertyAnimation {
                target: bgMask
                property: "opacity"
                from: 0; to: 0.2
                duration: 1000
            }
            PropertyAnimation {
                target: bgMask
                property: "opacity"
                from: 0.2; to: 0
                duration: 1000
            }
        }
    }
    Audio {
        source: "qrc:/res/audio/title_1.mp3"
        loops: Audio.Infinite
        // Component.onCompleted: play()
    }
    
    
    // -----------------USER INTERFACE-------------- //
    Image {
        id: decoration
        width: moduleView.width
        anchors {
            top: userIcon.bottom
            bottom: moduleView.top
            horizontalCenter: parent.horizontalCenter
        }
        opacity: 0.9
        source: "qrc:/res/ui/decoration.png"
        fillMode: Image.PreserveAspectFit
    }
    // -----------------MENU VIEW BEGIN-----------------//
    SwipeView {
        id: moduleView
        spacing: utils.dp(5)
        width: parent.width * 0.8
        height: parent.height * 0.5
        anchors.centerIn: parent
        anchors.verticalCenterOffset: utils.dp(30)
        currentIndex: 0
        
        Repeater {
            model: 4
            Loader {
                active: SwipeView.isCurrentItem ||
                        SwipeView.isNextItem ||
                        SwipeView.isPreviousItem
                sourceComponent: {
                    switch(index) {
                    case 0:
                        return battle
                    case 1:
                        return pkmInfo
                    case 2:
                        return onlineInfo
                    case 3:
                        return queryUser
                    default:
                        return null
                    }
                }
            }
        }
    }
    // -----------------MENU VIEW END-----------------//
    
    // ------------------NAVIGATION BAR BEGIN------------------ //
    Rectangle {
        id: navigationBar
        color: "#FAFAFA"
        anchors {
            bottom: parent.bottom
            bottomMargin: -(height/2)
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width
        height: utils.dp(80)
        radius: utils.dp(10)
        Rectangle {
            id: navigationHint
            width: utils.dp(40)
            height: utils.dp(4)
            color: "red"
            property var posData: [navigationBtnLayout.x, 0, 0, 0]
            x: posData[moduleView.currentIndex]
            y: parent.height / 2 - height
            
            Behavior on x {
                PropertyAnimation {
                    duration: 200
                    easing.type: Easing.OutQuart
                }
            }
            
            Component.onCompleted: {
                var startX = navigationBtnLayout.x
                for(var i = 0; i < 4; i++) {
                    posData[i] = startX + (utils.dp(60) * i)
                }
            }
        }
    }
    
    Row {
        id: navigationBtnLayout
        anchors {
            bottom: navigationBar.top
            bottomMargin: -navigationBar.height * 0.25
            horizontalCenter: parent.horizontalCenter
        }
        spacing: utils.dp(20)
        property int currentIndex: 0
        
        Repeater {
            model: [
                ["lightning", "开始战斗"],
                ["ball", "宝可梦信息"], 
                ["book", "在线用户"],
                ["stick", "角色信息"],
            ]
            delegate: Image {
                width: utils.dp(40)
                height: width
                source: "qrc:/res/ui/nav_" + modelData[0] + ".png"
                MouseArea {
                    anchors.fill: parent
                    onPressedChanged: {
                        mask.opacity = pressed? 0.4: 0
                    }
                    onClicked: {
                        moduleView.currentIndex = index
                    }
                }
                Rectangle {
                    id: mask
                    anchors.fill: parent
                    opacity: 0
                    color: "black"
                    radius: utils.dp(4)
                    Behavior on opacity {
                        PropertyAnimation {
                            duration: 100
                        }
                    }
                }
                Text {
                    anchors {
                        top: parent.bottom
                        topMargin: utils.dp(2)
                        horizontalCenter: parent.horizontalCenter
                    }
                    font.bold: true
                    font.family: "微软雅黑"
                    font.pixelSize: utils.dp(10)
                    text: modelData[1]
                }
            }
        }
    }
    // ------------------NAVIGATION BAR END------------------ //
    
    // ------------------USER ICON BEGIN--------------------- //
    Rectangle {
        id: userIcon
        anchors {
            top: parent.top
            left: parent.left
            margins: utils.dp(20)
        }
        width: utils.dp(40)
        height: width
        radius: width / 2
        color: utils.colorClouds
        Text {
            color: utils.colorWetAsphalt
            anchors.centerIn: parent
            font.pixelSize: utils.dp(30)
            text: backend.userName.substring(0, 1)
        }
    }
    
    OneBtnToast {
        id: popup
        contentW: parent.width * 0.8
        contentH: contentW * 0.7
    }
    
    
    Component {
        id: battle
        Rectangle {
            anchors.fill: parent
            radius: utils.dp(5)
            color: pageColor
            
            Column {
                spacing: utils.dp(10)
                anchors.centerIn: parent
                
                MFlatBtn {
                    id: inviteBtn
                    text: "发出邀请"
                    pressColor: "#34D0C6"
                    releaseColor: "#2CC486"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }
    Component {
        id: pkmInfo
        Rectangle {
            anchors.fill: parent
            radius: utils.dp(5)
            color: pageColor
        }
    }
    Component {
        id: onlineInfo
        Rectangle {
            anchors.fill: parent
            radius: utils.dp(5)
            color: pageColor
            
            ListView {
                id: userView
                clip:  true
                width: parent.width
                model: ListModel {
                    id: userModel
                }
                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                    bottom: refreshBtn.top
                    margins: utils.dp(4)
                }
                
                delegate: Rectangle {
                    
                }
            }
            
            Connections {
                target: backend
                onSigGetOnlineUserList: {
                    console.log("get online user list response")
                }
            }
            
            MFlatBtn {
                id: refreshBtn
                text: "刷新列表"
                pressColor: "#34D0C6"
                releaseColor: "#2CC486"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: utils.dp(4)
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    backend.sendOnlineUserListRequest()
                }
            }
            Component.onCompleted: backend.sendOnlineUserListRequest()
        }
    }
    Component {
        id: queryUser
        Rectangle {
            anchors.fill: parent
            radius: utils.dp(5)
            color: pageColor
        }
    }
}
