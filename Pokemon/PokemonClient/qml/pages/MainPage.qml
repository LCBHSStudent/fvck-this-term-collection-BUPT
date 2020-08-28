import QtQuick 2.14
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.14
import QtQuick.Shapes 1.14
import QtMultimedia 5.14

import "../components/"

Page {
    id: mainPage_t
    
    readonly property color pageColor: "#AAFAFAFA"
    property string destUserName: ""
    property int battleMode: 0
    
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
        id: bgm
        source: "qrc:/res/audio/title_1.mp3"
        loops: 999

        // Component.onCompleted: play()
    }
    
    Connections {
        target: stack
        onDepthChanged: {
            if (depth == 1) {
                bgm.play()
            } else {
                bgm.pause()
            }
        }
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
                active: true
//                active: SwipeView.isCurrentItem ||
//                        SwipeView.isNextItem ||
//                        SwipeView.isPreviousItem
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
            color: "#00BFFF"
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
    
    TwoBtnToast {
        id: invitePopup
        contentW: parent.width * 0.8
        contentH: contentW * 0.8
        
        property string fromUser: ""
        readonly property int accept: 0
        readonly property int refuse: 1
        
        onConfirmed: {
            gridPkmPopup.selectServerPkm = false
            gridPkmPopup.isInvited = true
            gridPkmPopup.showPopup()
        }
        onCanceled: {
            gridPkmPopup.isInvited = false
            backend.sendBattleInviteResponse(refuse, battleMode, 0, fromUser)
        }
    }
    
    ListModel {
        id: pkmDataModel_t
    }
    ListModel {
        id: pkmDataModel_server
    }
    ListModel {
        id: pkmDataModel_other
    }
    
    function appendPokemonData(pkmDataModel, pkmList) {
        pkmDataModel.clear()
        for (var i = 0; i < pkmList.length; i++) {
            pkmDataModel.append({
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
    }
    
    GridPkmPopup {
        id: gridPkmPopup
        contentW: parent.width * 0.8
        contentH: contentW
        
        pkmDataModel: selectServerPkm? pkmDataModel_server: pkmDataModel_t
        
        property bool   selectServerPkm: false
        property bool   isInvited: false
        property int    serverPkmId: 0
        
        onConfirmed: {
            if (!selectServerPkm) {
                if (isInvited) {
                    backend.sendBattleInviteResponse(
                        invitePopup.accept,
                        battleMode,
                        getSelectedPkmId(),
                        invitePopup.fromUser
                    )
                    isInvited = false
                } else {
                    backend.sendBattleStartRequest(
                        battleMode, getSelectedPkmId(), destUserName, serverPkmId
                    )
                }
                hidePopup()
                loadingPopup.showLoading("等待建立对战...", false)
            } else {
                serverPkmId = getSelectedPkmId()
                console.debug("serverPkmId: ", serverPkmId)
                selectServerPkm = false
                showPopup()
            }
        }
    }
    
    GridPkmPopup {
        id: othersPkmPopup
        contentW: parent.width * 0.8
        contentH: contentW
        
        pkmDataModel: pkmDataModel_other
        
        btnText: "确定"
        
        onConfirmed: {
            hidePopup()
        }
    }
    
    PkmInfoPopup {
        id: pkmInfoPopup
    }
    
    Connections {
        target: backend
        onSigGetBattleInviteRequest: {
            console.debug("onSigGetBattleInviteRequest")
            var stat = "\n邀请者: " + fromUser + "\n\n对战模式: "
            if (battleMode === 0) {
                stat += "升级战"
            } else if (battleMode === 1) {
                stat += "决斗战"
            }
            invitePopup.fromUser = fromUser
            mainPage_t.battleMode  = battleMode
            invitePopup.showPopup("收到对战请求", stat)
        }
        
        onSigGetBattleStartResponse: {
            console.debug("onSigGetBattleStartResponse")
            gridPkmPopup.hidePopup()
            loadingPopup.hideLoading()
            
            switch (status) {
            case 0:
                stack.push("BattlePage.qml")
                stack.currentItem.isUserA = isUserA
                stack.currentItem.myPkmId = urPkmId
                stack.currentItem.taPkmId = taPkmId
                if (isUserA) {
                    stack.currentItem.taUserName = destUserName
                } else {
                    stack.currentItem.taUserName = invitePopup.fromUser
                }
                
                stack.currentItem.battleMode = battleMode
                stack.currentItem.getPokemonInfo()
                break
            case 1:
                popup.showPopup("对方拒绝了您的邀请", "知道了")
                break
            case 2:
                popup.showPopup("对方已经在战斗中", "知道了")
                break
            case 3:
                popup.showPopup("目标用户不存在或不在线", "知道了")
                break
            default:
                break
            }
        }
        
        onSigGetPokemonDataList: {
            if (mode === 3) {
                appendPokemonData(pkmDataModel_server, pkmList)
            }
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////---START-----BATTLE---//////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    Component {
        id: battle
        Rectangle {
            anchors.fill: parent
            radius: utils.dp(5)
            color: pageColor
            
            Column {
                spacing: utils.dp(10)
                anchors.centerIn: parent
                TextField {
                    id: destNameInput
                    width: inviteBtn.width
                    height: utils.dp(30)
                    font.pixelSize: utils.dp(18)
                    color: "#2CC486"
                    font.family: "微软雅黑"
                    background: Rectangle {
                        color: "#00FFFFFF"
                        border.width: utils.dp(1)
                        border.color: "#2CC486"
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                MFlatBtn {
                    id: inviteBtn
                    text: "邀请对战"
                    pressColor: "#34D0C6"
                    releaseColor: "#2CC486"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        destUserName = destNameInput.text
                        battleMode = modeCheckGroup.battleMode
                        
                        pkmDataModel_t.clear()
                        backend.sendSelfPokemonInfoRequest()
                        gridPkmPopup.showPopup()
                        loadingPopup.showLoading("等待获取您的宝可梦信息...")
                        
//                        backend.sendBattleStartRequest(
//                            modeCheckGroup.battleMode, destNameInput.text)
                    }
                }
                MFlatBtn {
                    id: testBattleBtn
                    text: "模拟对战"
                    pressColor: "#34D0C6"
                    releaseColor: "#2CC486"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        destUserName = "_server"
                        battleMode = modeCheckGroup.battleMode
                        
                        pkmDataModel_t.clear()
                        backend.sendSelfPokemonInfoRequest()
                        gridPkmPopup.selectServerPkm = true
                        gridPkmPopup.showPopup()
                        loadingPopup.showLoading("等待获取您的宝可梦信息...")
                        
//                        backend.sendBattleStartRequest(
//                            modeCheckGroup.battleMode, "_server")
                    }
                }
                Row {
                    id: modeCheckGroup
                    anchors.horizontalCenter: parent.horizontalCenter
                    property int battleMode: 0
                    CheckBox {
                        text: "升级赛"
                        font.pixelSize: utils.dp(12)
                        font.family: "微软雅黑"
                        checked: parent.battleMode === 0
                        onClicked: {
                            parent.battleMode = 0
                        }
                    }
                    CheckBox {
                        text: "决斗赛"
                        font.pixelSize: utils.dp(13)
                        font.family: "微软雅黑"
                        checked: parent.battleMode === 1
                        onClicked: {
                            parent.battleMode = 1
                        }
                    }
                }
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////--QUERY---PKMINFO--///////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    Component {
        id: pkmInfo
        Rectangle {
            anchors.fill: parent
            radius: utils.dp(5)
            color: pageColor
            
            MFlatBtn {
                id: refreshBtn
                text: "刷新宝可梦列表"
                pressColor: "#34D0C6"
                releaseColor: "#2CC486"
                width: parent.width * 0.8
                anchors.bottom: parent.bottom
                anchors.bottomMargin: utils.dp(4)
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    backend.sendSelfPokemonInfoRequest()
                    loadingPopup.showLoading("正在获取宝可梦信息...")
                }
            }
            
            GridView {
                id: pkmView
                clip: true
                model: pkmDataModel_t
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: utils.dp(10)
                }

                width: parent.width * 0.95
                height: parent.height * 0.8
                cellWidth: width / 5
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
                            pkmInfoPopup.showPokemonInfo(
                                _id, typeId, name, level, type,
                                exp, attr,   atk,  def,   hp, spd,
                                skill_1, skill_2, skill_3, skill_4,
                                desc, desc_s1, desc_s2, desc_s3, desc_s4
                            )
                        }
                    }
                }
            }
            
            
            
            Connections {
                target: backend
                onSigGetPokemonDataList: {
                    console.debug("onSigGetPokemonDataList")
                    if (mode === 1) {
                        appendPokemonData(pkmDataModel_t, pkmList)
                        loadingPopup.hideLoading()
                    }
                }
            }
            
            Component.onCompleted: {
                backend.sendSelfPokemonInfoRequest()
            }
        }
    }
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////QUERY--ONLINE--USER--INFO/////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
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
                
                spacing: utils.dp(1)
                
                // readonly property int user_idle: 0
                // readonly property int user_battling: 1
                
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
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width * 0.8
                    height: utils.dp(30)
                    radius: utils.dp(5)
                    color: "#AAFAFAD2"
                    Text {
                        font.pixelSize: utils.dp(18)
                        font.family: "微软雅黑"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.right: statusIco.left
                        anchors.leftMargin: utils.dp(5)
                        anchors.rightMargin: utils.dp(5)
                        
                        text: name
                        color: "#CC1E90FF"
                        elide: Text.ElideRight
                    }
                    
                    Rectangle {
                        id: statusIco
                        width: utils.dp(8)
                        height: width
                        radius: width / 2
                        color: {
                            if (status === 0) {
                                return "#00FF00"
                            } else {
                                return "#FF1493"
                            }
                        }
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: utils.dp(5)
                    }
                }
            }
            
            Connections {
                target: backend
                onSigGetOnlineUserList: {
                    console.debug("get online user list response")
                    userModel.clear()
                    
                    for (var i = 0; i < nameList.length; i++) {
                        userModel.append({
                            "name": nameList[i],
                            "status": statusList[i]
                        })
                    }
                }
            }
            
            MFlatBtn {
                id: refreshBtn
                text: "刷新在线用户列表"
                pressColor: "#34D0C6"
                releaseColor: "#2CC486"
                width: parent.width * 0.8
                anchors.bottom: parent.bottom
                anchors.bottomMargin: utils.dp(4)
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    backend.sendOnlineUserListRequest()
                }
            }
            Component.onCompleted: {
                userModel.clear()
                backend.sendOnlineUserListRequest()
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////--QUERY--USER--INFO--/////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    Component {
        id: queryUser
        Rectangle {
            anchors.fill: parent
            radius: utils.dp(5)
            color: pageColor
            
            StackView {
                id: userInfoStack
                anchors.fill: parent
                initialItem: selectMode
                
                pushEnter: Transition {
                    PropertyAnimation {
                        property: "opacity"
                        from: 0
                        to:1
                        duration: 200
                    }
                }
                pushExit: Transition {
                    PropertyAnimation {
                        property: "opacity"
                        from: 1
                        to:0
                        duration: 200
                    }
                }
                popEnter: Transition {
                    PropertyAnimation {
                        property: "opacity"
                        from: 0
                        to:1
                        duration: 200
                    }
                }
                popExit: Transition {
                    PropertyAnimation {
                        property: "opacity"
                        from: 1
                        to:0
                        duration: 200
                    }
                }
            }
            
            Component {
                id: selectMode
                Item {
                    Column {
                        spacing: utils.dp(8)
                        anchors.centerIn: parent
                        TextField {
                            id: othersNameInput
                            width: otherInfoBtn.width
                            height: utils.dp(30)
                            font.pixelSize: utils.dp(18)
                            color: "#2CC486"
                            font.family: "微软雅黑"
                            background: Rectangle {
                                color: "#00FFFFFF"
                                border.width: utils.dp(1)
                                border.color: "#2CC486"
                            }
                        }
                        
                        MFlatBtn {
                            id: otherInfoBtn
                            text: "查询此用户"
                            pressColor: "#34D0C6"
                            releaseColor: "#2CC486"
                            onClicked: {
                                enabled = false
                                backend.sendUserInfoRequest(othersNameInput.text)
                                loadingPopup.showLoading("正在查询用户信息...")
                            }
                        }
                        MFlatBtn {
                            id: myInfoBtn
                            width: otherInfoBtn.width
                            text: "我的信息"
                            pressColor: "#34D0C6"
                            releaseColor: "#2CC486"
                            onClicked: {
                                enabled = false
                                backend.sendSelfUserInfoRequest()
                                loadingPopup.showLoading("正在查询用户信息...")
                            }
                        }
                    }
                    
                    Connections {
                        target: backend
                        onSigGetUserInfo: {
                            myInfoBtn.enabled = true
                            otherInfoBtn.enabled = true
                            loadingPopup.hideLoading()
                            if (status == 0) {
                                console.debug("get user info!")
                                userInfoStack.push(infoMode)
                                userInfoStack.currentItem.userInfo = {
                                    "userName":     userInfo.userName,
                                    "timeOfDuel":   userInfo.timeOfDuel,
                                    "timeOfWins":   userInfo.timeOfWins,
                                    "winRate":      userInfo.winRate,
                                    "amountBadge":  userInfo.amountBadge,
                                    "levelBadge":   userInfo.levelBadge,
                                    "status":       userInfo.status
                                }
                                console.debug(userInfoStack.currentItem.userInfo)
                            } else {
                                popup.showPopup("用户不存在", "知道了")
                            }
                        }
                    }
                }
            }
            Component {
                id: infoMode
                Item {
                    property var userInfo: ({})
                    MText {
                        id: userNameText
                        anchors {
                            left: parent.left
                            top: back.bottom
                            topMargin: utils.dp(5)
                            leftMargin: utils.dp(20)
                        }

                        color: "#000000"
                        text: {
                            if (typeof(parent.userInfo["userName"]) === "undefined")
                                return "null"
                            else 
                                return parent.userInfo["userName"]
                        }
                    }
                    MText {
                        id: winDetail
                        anchors {
                            left: userNameText.left
                            top: userNameText.bottom
                            topMargin: utils.dp(5)
                        }

                        color: "#000000"
                        text: {
                            if (typeof(parent.userInfo["timeOfDuel"]) ===
                                    "undefined")
                                return "null"
                            else
                                return  "胜场/总场次: " +
                                        parent.userInfo["timeOfWins"] + "/" +
                                        parent.userInfo["timeOfDuel"]
                        }
                    }
                    
                    MText {
                        id: winRate
                        anchors {
                            left: winDetail.left
                            top: winDetail.bottom
                            topMargin: utils.dp(5)
                        }

                        color: "#000000"
                        text: {
                            if (typeof(parent.userInfo["winRate"]) === "undefined")
                                return "null"
                            else
                                return  "胜率: " + 
                                        parent.userInfo["winRate"] * 100 + "%"
                        }
                    }
                    MText {
                        id: amountBadgeText
                        anchors {
                            left: winRate.left
                            top: winRate.bottom
                            topMargin: utils.dp(5)
                        }
                        color: "#000000"
                        text: "精灵数量勋章: "
                    }
                    
                    Image {
                        anchors.verticalCenter: amountBadgeText.verticalCenter
                        anchors.left: amountBadgeText.right
                        anchors.leftMargin: utils.dp(5)
                        source: {
                            switch (parent.userInfo["amountBadge"]) {
                            case 0:
                                return "qrc:/res/ui/ic_badge_bronze.png"
                            case 1:
                                return "qrc:/res/ui/ic_badge_silver.png"
                            case 2:
                                return "qrc:/res/ui/ic_badge_golden.png"
                            default:
                                return ""
                            }
                        }
                    }
                    
                    MText {
                        id: levelBadgeText
                        anchors {
                            left: amountBadgeText.left
                            top: amountBadgeText.bottom
                            topMargin: utils.dp(5)
                        }
                        color: "#000000"
                        text: "高级精灵勋章: "
                    }
                    Image {
                        anchors.verticalCenter: levelBadgeText.verticalCenter
                        anchors.left: levelBadgeText.right
                        anchors.leftMargin: utils.dp(5)
                        source: {
                            switch (parent.userInfo["levelBadge"]) {
                            case 0:
                                return "qrc:/res/ui/ic_badge_bronze.png"
                            case 1:
                                return "qrc:/res/ui/ic_badge_silver.png"
                            case 2:
                                return "qrc:/res/ui/ic_badge_golden.png"
                            default:
                                return ""
                            }
                        }
                    }   
                    MImgBtn {
                        id: back
                        visible: true
                        btnImage: "btn_back_green"
                        triggerFactor: 4
                        onClicked: {
                            userInfoStack.pop()
                        }
                    }
                    MFlatBtn {
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottomMargin: utils.dp(10)
                        width: parent.width * 0.85
                        text: "看看Ta的P(okemon)"
                        pressColor: "#34D0C6"
                        releaseColor: "#2CC486"
                        onClicked: {
                            backend.sendUserAllPkmDataRequest(userNameText.text)
                            loadingPopup.showLoading("正在请求宝可梦信息...")
                        }
                    }
                    Connections {
                        target: backend
                        onSigGetPokemonDataList: {
                            if (mode === 4) {
                                loadingPopup.hideLoading()
                                appendPokemonData(pkmDataModel_other, pkmList)
                                othersPkmPopup.showPopup()
                            }
                        }
                    }
                }
            }
        }
    }
    
    Component.onCompleted: {
        loadingPopup.hideLoading()
        backend.sendServerPokemonInfoRequest()
    }
    
}
