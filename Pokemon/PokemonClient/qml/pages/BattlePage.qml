import QtQuick 2.14
import QtMultimedia 5.14
import QtQuick.Controls 2.14

import "../components/"

Page {
    id: battlePage_t
    background: Image {
        source: "qrc:/res/image/battle_background.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        fillMode: Image.PreserveAspectCrop
        width: parent.width
        height: parent.height
    }
    
    property var isUserA: 1
    property var taUserName: ""
    property var taCurHP: 1
    property var taMaxHP: 1
    property var myCurHP: 1
    property var myMaxHP: 1
    property var myPkmId: 1
    property var taPkmId: 1
    property var myPkmType: 0
    property var taPkmType: 0
    property var skill_1: ""
    property var skill_2: ""
    property var skill_3: ""
    property var skill_4: ""
    property var skills: [skill_1, skill_2, skill_3, skill_4]
    
    
    Audio {
        loops: 999
        source: "qrc:/res/audio/battle_" + (root.getRandomInt(4)+1) +  ".mp3"
        autoPlay: true
    }
    
    
    
    // ------------------ my pokemon components ------------------ //
    AnimatedImage {
        id: myPkmGif
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: utils.dp(90)
        }
        scale: 1.3
        source: myPkmType? 
            "qrc:/res/image/character/" + myPkmType + ".gif": ""
    }
    Image {
        id: myPkmRef
        anchors.right: myPkmGif.left
        anchors.bottom: myPkmGif.top
        source: "qrc:/res/ui/reference.png"
    }
    HpBar {
        id: myHpBar
        curHP: battlePage_t.myCurHP
        maxHP: battlePage_t.myMaxHP
        anchors.horizontalCenter: myPkmRef.horizontalCenter
        anchors.bottom: myPkmRef.top
        width: parent.width * 0.55
    }
    // ------------------ enemy pokemon components ------------------ //
    AnimatedImage {
        id: taPkmGif
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: utils.dp(270)
        }
        scale: 1.2
        source: taPkmType?
            "qrc:/res/image/character/" + taPkmType + "_oppo.gif": ""
    }
    Image {
        id: taPkmRef
        anchors.right: taPkmGif.left
        anchors.bottom: taPkmGif.top
        source: "qrc:/res/ui/reference.png"
    }
    HpBar {
        id: taHpBar
        curHP: battlePage_t.taCurHP
        maxHP: battlePage_t.taMaxHP
        anchors.horizontalCenter: taPkmRef.horizontalCenter
        anchors.bottom: taPkmRef.top
        width: parent.width * 0.55
    }
    
    Column {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: utils.dp(120)
        spacing: utils.dp(10)
        Repeater {
            model: battlePage_t.skills
            delegate: Image {
                width: battlePage_t.width * 0.4
                fillMode: Image.PreserveAspectFit
                source: "qrc:/res/ui/skill_slot_" + (index + 1) + ".png"
                rotation: 20
                MouseArea {
                    anchors.fill: parent
                    onPressedChanged: {
                        if (pressed) {
                            parent.source =
                                "qrc:/res/ui/skill_slot_pressed.png"
                        } else {
                            parent.source =
                                "qrc:/res/ui/skill_slot_" + (index + 1) + ".png"
                        }
                    }
                    onClicked: {
                        backend.sendBattleSkillIndex(isUserA, index)
                    }
                }
                Item {
                    clip: true
                    height: parent.height
                    width: parent.width * 0.6
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: parent.width * 0.16
                    
                    Text {
                        text: modelData
                        anchors.centerIn: parent
                        font.pixelSize: utils.dp(13)
                        font.family: "微软雅黑"
                        font.bold: true
                        color: "#B0000000"
                    }
                }
            }
        }
    }
    
    MFlatBtn {
        text: "ESCAPE"
        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: utils.dp(20)
        }
        pressColor: utils.colorPomegranate
        releaseColor: utils.colorAlizarin
        onClicked: {
            popup.showPopup("逃离战斗", "\n\n确定要这样做吗")
        }
    }
    
    // ------------------ battle infomation displayer ------------- //
    MText {
        color: "#77000000"
        text: "Battle Log"
        anchors.centerIn: messageView
    }
    
    ScrollView {
        id: messageView
        clip: true
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            leftMargin: utils.dp(5)
            rightMargin: utils.dp(5)
            bottom: taHpBar.top
            bottomMargin: utils.dp(20)
        }
        
        TextArea {
            id: field
            enabled: false
            wrapMode: Text.WrapAnywhere
            color: "#DD00BFFF"
            font.pixelSize: utils.dp(12)
            background: Rectangle {
                height: messageView.height
                radius: utils.dp(10)
                color: "#AAFFFFFF"
            }
            font.family: "微软雅黑"
            font.bold: true
        }
    }
    
    TwoBtnToast {
        id: popup
        contentW: parent.width * 0.8
        contentH: contentW * 0.8
        
        onConfirmed: {
            backend.sendBattleGiveupInfo()
        }
        onCanceled: {
            console.debug("canceled escaping")
        }
    }
    
    function getPokemonInfo() {
        backend.sendBattlePokemonInfoRequest(taUserName, myPkmId, taPkmId)
    }
    
    Connections {
        target: backend
        onSigGetBattleTurnInfo: {
            console.debug("get battle turn info")
        }
        onSigGetBattleFinishInfo: {
            console.debug("get battle finish info")
        }
        onSigGetPokemonDataList: {
            if (mode === 0) {
                console.debug("get battle start pkm info")
                if (userName === taUserName) {
                    taPkmType   = pkmList[0].typeId
                    taMaxHP     = pkmList[0].hp
                    taCurHP     = taMaxHP
                    taPkmId     = pkmList[0].id
                } else {
                    myPkmType   = pkmList[0].typeId
                    skill_1     = pkmList[0].skill_1
                    skill_2     = pkmList[0].skill_2
                    skill_3     = pkmList[0].skill_3
                    skill_4     = pkmList[0].skill_4
                    
                    myMaxHP     = pkmList[0].hp
                    myCurHP     = myMaxHP
                    myPkmId     = pkmList[0].id
                }
            }
        }
    }
}
