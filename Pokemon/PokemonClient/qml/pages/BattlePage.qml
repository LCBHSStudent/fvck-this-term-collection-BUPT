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
    
    property var battleMode: -1
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
    property bool enableOperate: true
    
    
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
                    enabled: enableOperate
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
                        loadingPopup.showLoading("等待对方操作...")
                        enableOperate = false
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
            stack.pop()
        }
        onCanceled: {
            console.debug("canceled escaping")
        }
    }
    
    OneBtnToast {
        id: battleResultPopup
        
        contentW: parent.width * 0.8
        contentH: contentW * 0.8
        
        onClicked: {
            if (battleMode == 0 || taUserName == "_server") {
                stack.pop()
            }
        }
    }
    
    GridPkmPopup {
        id: gridPkmPopup
        contentH: parent.height * 0.25
        contentW: parent.width * 0.7
        cellColumnCnt: 3
        
        onConfirmed: {
            var pkmId = getSelectedPkmId()
            backend.sendTransferPokemonRequest(taUserName, pkmId)
            hidePopup()
            loadingPopup.showLoading("正在获取选择的精灵...")
        }
    }
    
    
    function getPokemonInfo() {
        backend.sendBattlePokemonInfoRequest(taUserName, myPkmId, taPkmId)
    }
    
    Connections {
        target: backend
        onSigGetBattleTurnInfo: {
            console.debug("get battle turn info")
            loadingPopup.hideLoading()
            enableOperate = true
            var log = ""
            console.log(info.type, isUserA, info.type ^ isUserA)
            if (info.type ^ isUserA) {
                log += "你使用了" + info.skillName + ", 你的生命值变化了" + 
                        (-info.selfDeltaHP) 
                        + ", 对手的生命值变化了" + (-info.destDeltaHP)
            } else {
                log += "对手使用了" + info.skillName + ", 你的生命值变化了" +
                        (-info.selfDeltaHP)  
                        + ", 对手的生命值变化了" + (-info.destDeltaHP)
            }
            if (info.selfBuffID) {
                log += "\n你获得了buff[ID: " + info.selfBuffID + "], 持续时间" 
                        + info.selfBuffLast + "回合"
            }
            if (info.destBuffID) {
                log += "\n你获得了buff[ID: " + info.destBuffID + "], 持续时间" 
                        + info.destBuffLast + "回合"
            }
            field.append(log)
            
            myCurHP -= info.selfDeltaHP
            taCurHP -= info.destDeltaHP
        }
        onSigGetBattleFinishInfo: {
            console.debug("get battle finish info")
            loadingPopup.hideLoading()
            var log = ""
            if (mode == 0) {
                if (result === 0) {
                    log += "恭喜您\n战斗获胜了"
                } else {
                    log += "很遗憾\n决斗失败了"
                }
            } else {
                log += "对方已断开连接"
            }
            battleResultPopup.showPopup(log, "知道了")
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
            if (mode === 2) {
                gridPkmPopup.pkmDataModel.clear()
                for (var i = 0; i < pkmList.length; i++) {
                    gridPkmPopup.pkmDataModel.append({
                        "_id":      pkmList[i].id,
                        "typeId":   pkmList[i].typeId,
                        "name":     pkmList[i].name,
                        "level":    pkmList[i].level,
                        "type":     pkmList[i].type,
                        "exp":      pkmList[i].exp,
                        "attr":     pkmList[i].attr,
                        "atk":      pkmList[i].atk,
                        "def":      pkmList[i].def,
                        "hp":       pkmList[i].hp,
                        "spd":      pkmList[i].spd,
                        "skill_1":  pkmList[i].skill_1,
                        "skill_2":  pkmList[i].skill_2,
                        "skill_3":  pkmList[i].skill_3,
                        "skill_4":  pkmList[i].skill_4,
                        "desc":     pkmList[i].desc,
                        "desc_s1":  pkmList[i].desc_s1,
                        "desc_s2":  pkmList[i].desc_s2,
                        "desc_s3":  pkmList[i].desc_s3,
                        "desc_s4":  pkmList[i].desc_s4,
                    })
                }
                gridPkmPopup.showPopup()
            }
        }
        onSigTransferPokemonFinished: {
            loadingPopup.hideLoading()
            if (result === 0) {
                stack.pop()
            }
            else {
                console.debug("获取选择的宝可梦失败!")
            }
        }
    }
}
