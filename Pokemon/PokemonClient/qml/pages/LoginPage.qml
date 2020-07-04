import QtQuick 2.14
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.14
import QtQuick.LocalStorage 2.14
import QtQuick.Shapes 1.14

import "../components/"

Page {
    id: loginPage_t
    
    property int  chosenModel:  0
    property int  animDuration: 350
    
    //--------------------DESCRIPTION FOR UI APPEARANCE-------------------//
    
    Rectangle {
        id: bgRectPart1
        color: "#1F4BA5"
        width: parent.width
        height: parent.height * 0.375
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        MText {
            id: loginHint
            visible: chosenModel !== 2
            anchors {
                top: parent.top
                topMargin: parent.height * 0.3
                horizontalCenter: parent.horizontalCenter
            }
            font.bold: true
            text: "Login"
        }
        MouseArea {
            enabled: chosenModel === 0
            anchors.fill: parent
            onClicked: {
                chosenModel = 1
                loginAnimation.start()
            }
        }
        property int fromPos: parent.height * 0.375
        property int destPos: parent.height * 1.25
        
        property int fromPosPart2: parent.height * 0.625
        property int destPosPart2: 0
        
        ParallelAnimation {
            id: loginAnimation
            PropertyAnimation {
                id:          part1Animation
                target:      bgRectPart1
                property:    "height"
                from:        bgRectPart1.fromPos
                to:          bgRectPart1.destPos
                duration:    animDuration
                easing.type: Easing.InQuad
            }
            PropertyAnimation {
                id:          part2Animation
                target:      bgRectPart2
                property:    "height"
                from:        bgRectPart1.fromPosPart2
                to:          bgRectPart1.destPosPart2
                duration:    animDuration
                easing.type: Easing.InQuad
            }
            onFinished: {
                part1Animation.from = bgRectPart1.fromPos
                part1Animation.to   = bgRectPart1.destPos
                
                part2Animation.from = bgRectPart1.fromPosPart2
                part2Animation.to   = bgRectPart1.destPosPart2
            }
        }
        Column {
            visible: chosenModel === 1
            opacity: visible? 1: 0
            Behavior on opacity {
                PropertyAnimation { duration: animDuration }
            }
            width: parent.width * 0.95
            anchors {
                top: loginHint.bottom
                topMargin: utils.dp(20)
                horizontalCenter: parent.horizontalCenter
                horizontalCenterOffset: -utils.dp(10)
            }
            spacing: utils.dp(8)
            MTextInput{
                id: userName
                width: parent.width
                height: utils.dp(30)
                placeholderText: "请输入用户名"
                validator: RegExpValidator {regExp: /^\w*$/}
                implicitHeight: height * 1.5
            }
            MTextInput{
                id: userPsw
                width: parent.width
                height: utils.dp(30)
                placeholderText: "请输入密码"
                // 限制输入类型的正则校验器
                validator: RegExpValidator {regExp: /^\w*$/}
                // 设定输入密码啊嗯
                echoMode: TextInput.Password
                implicitHeight: height * 1.5
            }
            MFlatBtn {
                width: utils.dp(200)
                height: utils.dp(30)
                text: "登录"
                textColor: utils.colorWetAsphalt
                pressColor: utils.colorSilver
                releaseColor: utils.colorClouds
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: utils.dp(10)
                
                onClicked: {
                    if(userName.text.length && userPsw.text.length) {
                        backend.sendLoginRequest(userName.text, userPsw.text)
                    }
                }
            }
        }
        
    }
    Rectangle {
        id: bgRectPart2
        color: "#222222"
        width: parent.width
        height: parent.height * 0.625
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
        MText {
            id: signUpHint
            visible: chosenModel !== 1
            anchors.centerIn: parent
            text: "Sign Up"
            font.bold: true
        }
        MouseArea {
            enabled: chosenModel === 0
            anchors.fill: parent
            onClicked: {
                signUpAnimation.start()
                chosenModel = 2
            }
        }
        property int destPos: parent.height * 1.25
        property int fromPos: parent.height * 0.625
        
        PropertyAnimation {
            id: signUpAnimation
            target: bgRectPart2
            property: "height"
            from:   bgRectPart2.fromPos
            to:     bgRectPart2.destPos
            duration: animDuration
            easing.type: Easing.InQuad
            
            onFinished: {
                signUpAnimation.from = bgRectPart2.fromPos
                signUpAnimation.to   = bgRectPart2.destPos
            }
        }
        
        Column {
            visible: chosenModel === 2
            opacity: visible? 1: 0
            Behavior on opacity {
                PropertyAnimation { duration: animDuration }
            }
            width: parent.width * 0.95
            anchors {
                top: signUpHint.bottom
                topMargin: utils.dp(20)
                horizontalCenter: parent.horizontalCenter
                horizontalCenterOffset: -utils.dp(10)
            }
            spacing: utils.dp(8)
            MTextInput {
                id: signUpName
                width: parent.width
                height: utils.dp(30)
                placeholderText: "请输入注册用户名"
                validator: RegExpValidator {regExp: /^\w*$/}
                implicitHeight: height * 1.5
            }
            MTextInput {
                id: signUpPsw
                width: parent.width
                height: utils.dp(30)
                placeholderText: "请输入密码"
                // 限制输入类型的正则校验器
                validator: RegExpValidator {regExp: /^\w*$/}
                // 设定输入密码啊嗯
                echoMode: TextInput.Password
                implicitHeight: height * 1.5
            }
            MFlatBtn {
                width: utils.dp(200)
                height: utils.dp(30)
                text: "注册"
                pressColor: utils.colorBelizeHole
                releaseColor: utils.colorPeterRiver
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: utils.dp(10)
                
                onClicked: {
                    if(signUpName.text.length && signUpPsw.text.length) {
                        backend.sendSignUpRequest(signUpName.text, signUpPsw.text)
                    }
                }
            }
        }
    }
    
    Shape {
        id: triangle
        width: parent.width
        height: parent.height * 0.25
        anchors {
            top: bgRectPart2.top
            horizontalCenter: parent.horizontalCenter
        }
        smooth: true
        containsMode: Shape.FillContains
        ShapePath {
            strokeWidth: 1
            strokeColor: "#1F4BA5"
            fillColor: "#1F4BA5"
            startX: 0; startY: 0
            PathLine { x: 0;              y: 0 }
            PathLine { x: triangle.width; y: 0 }
            PathLine { x: 0;              y: triangle.height }
        }
        
    }
    
    MText {
        visible: chosenModel !== 1
        opacity: visible? 1: 0
        Behavior on opacity {
            PropertyAnimation { duration: animDuration }
        }
        
        text: "By clicking \"Sign Up\" you agree to our
                 <Terms of Service>"
        color: "#B2B2B2"
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: utils.dp(30)
        }
        font.pixelSize: utils.dp(12)
    }
    
    MImgBtn {
        id: back
        visible: chosenModel !== 0
        opacity: visible? 1: 0
        btnImage: "btn_back"
        Behavior on opacity {
            PropertyAnimation { duration: 200 }
        }
        
        onClicked: {
            if(chosenModel === 1) {
                // -loginAnimation
                    // 复位上层                
                    part1Animation.from = bgRectPart1.destPos
                    part1Animation.to   = bgRectPart1.fromPos
                    // 复位下层    
                    part2Animation.from = bgRectPart1.destPosPart2
                    part2Animation.to   = bgRectPart1.fromPosPart2
                // -configuration finish
                loginAnimation.start()
            } else if (chosenModel === 2) {
                signUpAnimation.from = bgRectPart2.destPos
                signUpAnimation.to   = bgRectPart2.fromPos
                signUpAnimation.start()
            }
            chosenModel = 0
        }
    }
    
    
    // ------------------------HANDLE BACKEND SIGNALS--------------------//
//    Connections {
//        target: backend
//    }
}
