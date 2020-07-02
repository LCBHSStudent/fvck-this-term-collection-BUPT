import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Particles 2.14

Page {
    id: ovPage
    
    property var citys: [
        [-114, -514, "下北泽"],    // aborted
        
        [834, 234, "北京"],
        [625, 427, "成都"],
        [750, 624, "北海"],
        [718, 350, "西安"],
        [619, 312, "兰州"],
        [673, 452, "重庆"],
        [810, 481, "长沙"],
        [829, 426, "武汉"],
        [850, 249, "天津"],
        [379, 156, "乌鲁木齐"],
        [580, 302, "西宁"],
        [363, 446, "拉萨"],
    ]
    signal showText()
    
    AnimatedImage {
        id: opAnima
        speed:      3
        source:     "qrc:/res/images/op.gif"
        fillMode:   Image.PreserveAspectCrop
        height:     parent.height - topBar.height
        width:      parent.width
        anchors.centerIn: parent
        anchors.verticalCenterOffset: utils.dp(40)
        onCurrentFrameChanged: {
            if(currentFrame === frameCount-1) {
                paused       = true
                ovBG.visible = true
                //destroy()
            }
        }
        
        PropertyAnimation {
            target: opAnima
            running: true
            properties: "scale"
            to: 1.15
            duration: 800
            easing.type: Easing.InQuad
        }
    }
    
    Image {
        id: ovBG
        visible: false
        opacity: visible? 1: 0
        anchors.centerIn: parent
        anchors.verticalCenterOffset: utils.dp(40)
        source: "qrc:/res/images/ovBG.png"
        
        Behavior on opacity {
            PropertyAnimation {
                property:    "opacity"
                duration:    800
                easing.type: Easing.OutQuad
            }
        }
        PropertyAnimation {
            target: ovBG
            running: true
            properties: "scale"
            to: 1.3
            loops: 1
            duration: 800
            easing.type: Easing.OutQuad
        }
    }
    
    Text {
        text: backend.sysTime
        font.family: "微软雅黑"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: utils.dp(20)
        font.pixelSize: utils.dp(20)
        color: utils.colorClouds
    }
    
    ParticleSystem {
        id: particles
        anchors.fill: parent
        
        ImageParticle {
            source: "qrc:/res/images/blueStar.png"
            alpha: 0
            colorVariation: 0.1
            color: utils.colorWetAsphalt
        }
        
        Emitter {
            id: emt
            emitRate: 500
            lifeSpan: 1000
            enabled: false
            velocity: AngleDirection {
                magnitude: 64; angleVariation: 360
            }
            size: 24
            sizeVariation: 8
        }
        
        Timer {
            interval: 1500
            running: true
            repeat: true
            onTriggered: {
                for(var i = 0; i < citys.length; i++) {
                    emt.x = citys[i][0]
                    emt.y = citys[i][1]
                    emt.burst(81)
                }
            }
        }
    }
    
    Repeater {
        model: citys
        delegate: Text {
            visible: false
            color: utils.colorClouds
            font.pixelSize: utils.dp(10)
            font.bold: true
            
            text: modelData[2]
            y: modelData[1] - font.pixelSize/2
            x: modelData[0] - font.pixelSize*text.length/2
            
            Connections {
                target: ovPage
                onShowText: {
                    visible = true
                }
            }
        }
    }
    
    Timer {
        repeat: false
        interval: 1000
        Component.onCompleted: start()
        onTriggered: {
            opAnima.source = ""
            showText()
        }
    }
    
//----------------------- reserved for customer animation ----------------------//
    ParticleSystem {
        id: customerSys
        ImageParticle {
            source: "qrc:/res/images/blueStar.png"
            color: utils.colorPeterRiver
            colorVariation: 0.5
            alpha: 0
        }
        
        Attractor {
            width: 1; height: 1
            system: customerSys
            pointX: citys[10][0]
            pointY: citys[10][1]
            strength: 100000
        }
        
        Emitter {
            id: customerEmt
            emitRate: 16
            width:  1
            height: 1
            
            enabled: false
            
            property real destX
            property real destY
            property int  duration
            
            velocity: PointDirection {
                x: customerEmt.destX
                y: customerEmt.destY
                xVariation: 0.1
                yVariation: 0.1
            }
            
            size: utils.dp(5)
            
//            sizeVariation: 1
            
//            PropertyAnimation {
//                id:     pathAnimation
//                target: customerEmt
//                properties: "lifeSpan"
//                from: 0
//                to: duration - 100

//                duration: customerEmt.duration
//                running: false
//                loops: 1
                
//                onFinished: {
//                    customerEmt.lifeSpan = 0
//                    customerEmt.enabled = false
//                }
//            }
            
            function setEmitInfo(fromId, destId, duration) {
                customerEmt.x = citys[fromId][0];                
                customerEmt.y = citys[fromId][1];
                
                var tempX = citys[destId][0] - customerEmt.x;
                var tempY = citys[destId][1] - customerEmt.y;
                
                customerEmt.destX = tempX / duration * 1000
                                 // tempX / Math.sqrt(tempX*tempX + tempY*tempY)
                customerEmt.destY = tempY / duration * 1000
                                 // tempY / Math.sqrt(tempX*tempX + tempY*tempY)
                
                customerEmt.duration = duration
                
                console.log(x, y, destX, destY, duration)
                
                customerEmt.enabled = true
                customerEmt.lifeSpan = duration
                pathTimer.duration = duration
                pathTimer.start()
                // pathAnimation.start()
            }
        }
    }
    
    Timer {
        id: pathTimer
        running: false
        repeat:  true
        
        property int  duration:  0
        property int  factor:    0
        property int  recover:   0
        property bool paused: false
        interval: 100
        
        function pause() {
            if(pathTimer.running && !paused) {
                paused = true
                recover = duration - (interval * factor)
                factor = 0
                stop()
            }
        }
        function resume() {
            if(paused) {
                duration = recover
                start()
                paused = false
            }
        }
        
        onTriggered: {
            if(factor < duration/100) {
                factor++
            } else {
                customerSys.reset()
                customerEmt.lifeSpan = 0
                customerEmt.enabled = false
                pathTimer.stop()
                factor = 0
            }
        }
    }
    
//    PropertyAnimation {
//        id: resetSysAction
//        target: customerSys
//        property: "opacity"
//        from: 1
//        to: 0
//        duration: 50
//        onFinished: {
//            customerSys.reset()
//            customerSys.opacity = 1
//            //recoverSysOpacity.start()
//        }
//    }
//    PropertyAnimation {
//        id: recoverSysOpacity
//        target: customerSys
//        property: "opacity"
//        from: 0
//        to: 1
//        duration: 400
//    }
    
    Connections {
        target: backend
        onSigCustomerPosChanged: {
            customerEmt.setEmitInfo(fromId, destId, duration)
        }
    }
    Connections {
        target: root
        onSigPauseAnimation: {
            console.log("pause")
            pathTimer.pause()
            if(!customerSys.paused) {
                customerSys.pause()
            }
        }
        onSigContinueAnimation: {
            console.log("resume")
            pathTimer.resume()
            if(customerSys.paused) {
                customerSys.resume()
            }
        }
    }
    
//    Button {
//        anchors.centerIn: parent
//        width: 100
//        height: 100
//        onClicked: customerEmt.setEmitInfo(Math.floor(Math.random()*12), Math.floor(Math.random()*12), 10000)
//    }
    
}
