import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Universal 2.14

import nsp.DS.Backend 1.14

Page {
    id: datVisPage
    background: Rectangle {
        color: "#0F0F0F"
    }
    
    TextField {
        id: hint
        anchors{
            left: parent.left
            top:  parent.top
            leftMargin: parent.width * 0.05
            topMargin:  utils.dp(20)
        }
        width:  contentWidth + font.pixelSize * 2
        height: font.pixelSize + utils.dp(10)
        text: " Supported City List"
        color: utils.colorClouds
        enabled: false
        font.bold: true
        background: Rectangle {
            color: "#3E65FF"
            opacity: 0.9
        }
    }
    Rectangle {
        id: cityBorder
        anchors.horizontalCenter: hint.horizontalCenter
        anchors.top: hint.bottom
        anchors.topMargin: utils.dp(20)
        color: "transparent"
        border.color: utils.colorWetAsphalt
        border.width: utils.dp(1)
        width: utils.dp(200)
        height: utils.dp(300)
        radius: utils.dp(4)
        
        ListView {
            id: cityView
            clip: true
            anchors.centerIn:   parent
            width:              parent.width
            height:             parent.height * 0.98
            model:              backend.cityModel
            delegate: Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                color: utils.colorWetAsphalt
                height: utils.dp(38)
                width: parent.width * 0.9
                radius: utils.dp(2)
                
                Behavior on color {
                    ColorAnimation {
                        duration: 200
                    }
                }
                
                MouseArea {
                    anchors.fill: parent
                    onPressedChanged: 
                        pressed?
                            parent.color = utils.colorMidnightBlue:
                            parent.color = utils.colorWetAsphalt
                }
                
                Text {
                    id: ctName
                    text: CityName
                    color: utils.colorClouds
                    font {
                        bold: true
                        pixelSize: utils.dp(18)
                    }
                    anchors {
                        verticalCenter: ctID.verticalCenter
                        left: ctID.right
                        leftMargin: utils.dp(7)
                    }
                }
                Text {
                    id: ctID
                    text: "No." + CityID
                    color: utils.colorBelizeHole
                    font {
                        bold: true
                        pixelSize: utils.dp(18)
                    }
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: utils.dp(5)
                    }
                }
                Text {
                    id: ctFactor
                    text: CityDangerFactor.toFixed(1)
                    color: {
                        if(text === "0.9")
                            return utils.colorAlizarin
                        else if(text === "0.5")
                            return utils.colorCarrot
                        else
                            return utils.colorEmerald
                    }

                    font {
                        bold: true
                        pixelSize: utils.dp(15)
                    }
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                        rightMargin: utils.dp(5)
                    }
                }
            }
        }
    }
    
    ComboBox {
        width: utils.dp(150)
        height: utils.dp(30)
        anchors.right: scheduleBorder.right
        anchors.verticalCenter: hint2.verticalCenter
        textRole: "text"
        valueRole: "value"
        model: [
            { value: -1,                text: qsTr("All") },
            { value: Backend.Airplane,  text: qsTr("Airplane") },
            { value: Backend.Train,     text: qsTr("Train") },
            { value: Backend.Car,       text: qsTr("Car") }
        ]
        onCurrentValueChanged: {
            scheduleView.filter = currentValue
            scheduleView.positionViewAtBeginning()
        }
    }
    
    TextField {
        id: hint2
        anchors {
            horizontalCenter: scheduleBorder.horizontalCenter
            verticalCenter: hint.verticalCenter
        }
        width:  contentWidth + font.pixelSize * 2
        height: font.pixelSize + utils.dp(10)
        text: "  Traveling Schedule"
        color: utils.colorClouds
        enabled: false
        font.bold: true
        background: Rectangle {
            color: "#3E65FF"
            opacity: 0.9
        }
    }
    
    Rectangle {
        id: scheduleBorder
        anchors {
            right: parent.right
            rightMargin: utils.dp(30)
            left: cityBorder.right
            leftMargin: utils.dp(50)
            bottom: cityBorder.bottom
        }
        color: "transparent"
        border.color: utils.colorWetAsphalt
        border.width: utils.dp(1)
        height: utils.dp(300)
        radius: utils.dp(4)
        
        ListView {
            id: scheduleView
            clip: true
            anchors.centerIn:   parent
            width:              parent.width
            height:             parent.height * 0.98
            model:              backend.scheduleModel
            
            property int        filter: -1;
            property string     sFilter: ""
            property string     aFilter: ""
            
            delegate: Rectangle {
                visible: {
                    var transType = 
                        scheduleView.filter === -1 ||
                        scheduleView.filter === TransportationType
                    var s = scheduleView.sFilter.length > 0
                    var a = scheduleView.aFilter.length > 0
                    if(!s && a) {
                        return  transType &&
                                scheduleView.aFilter === ToStation
                    } else if (s && !a) {
                        return  transType &&
                                scheduleView.sFilter === FromStation
                    } else if (s && a) {
                        return  transType &&
                                scheduleView.sFilter === FromStation &&
                                scheduleView.aFilter === ToStation
                    } else {
                        return transType
                    }
                }

                anchors.horizontalCenter: parent.horizontalCenter
                color: utils.colorWetAsphalt
                height: visible? utils.dp(50): 0
                width: parent.width * 0.96
                radius: utils.dp(2)
                
                Behavior on color {
                    ColorAnimation {
                        duration: 200
                    }
                }
                
                MouseArea {
                    anchors.fill: parent
                    onPressedChanged: 
                        pressed?
                            parent.color = utils.colorMidnightBlue:
                            parent.color = utils.colorWetAsphalt
                }
                Text {
                    id: tacInfo
                    text: {
                        var info = ""
                        switch(TransportationType) {
                        case Backend.Airplane:
                            info += "A"
                            break
                        case Backend.Train:
                            info += "T"
                            break 
                        case Backend.Car:
                            break
                        default:
                            info += "C"
                            break
                        }
                        info += TACNumber
                        return info
                    }
                        
                    font {
                        pixelSize: utils.dp(20)
                        bold: true
                    }
                    color: utils.colorClouds
                    anchors.left: tacArrow.left
                    anchors.leftMargin: utils.dp(15)
                    anchors.bottom: tacArrow.top
                    anchors.bottomMargin: utils.dp(1)
                }
                Text {
                    id: tacInfoEX
                    text: {
                        var info = ""
                        var arrValue = ArriveH * 60 + ArriveM
                        var staValue = StartH * 60 + StartM
                        
                        var passTime = arrValue - staValue
                        var sub = 1440 - staValue
                        
                        if(passTime > sub) {
                            info += " 【" +
                                    (1 + Math.floor((passTime - sub) / 1440)) +
                                    "日后到达】"
                        } else {
                            info += " 【当日到达】"
                        }
                    }
                    font {
                        pixelSize: utils.dp(20)
                        bold: true
                    }
                    color: utils.colorClouds
                    anchors.right: tacArrow.right
                    anchors.rightMargin: utils.dp(15)
                    anchors.bottom: tacArrow.top
                    anchors.bottomMargin: utils.dp(1)
                }
                
                Text {
                    id: tacArrow
                    text: qsTr("———————————>")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: fromInfo.verticalCenter
                    font.bold: true
                    font.pixelSize: utils.dp(20)
                    color: "#F0A30A"
                }
                
                Text {
                    id: fromInfo
                    text: FromStation
                    font {
                        pixelSize: utils.dp(20)
                        bold: true
                    }
                    anchors {
                        horizontalCenter: startInfo.horizontalCenter
                        top: parent.top
                        topMargin: utils.dp(25)
                    }
                    color: "#A4C400"
                }
                Text {
                    id: toInfo
                    text: ToStation
                    font {
                        pixelSize: utils.dp(20)
                        bold: true
                    }
                    anchors {
                        horizontalCenter: arrInfo.horizontalCenter
                        top: parent.top
                        topMargin: utils.dp(25)
                    }
                    color: "#A4C400"
                }
                Text {
                    id: startInfo
                    text: scheduleBorder.formatTimeValue(StartH) +
                          ":" +
                          (StartM < 10? "0"+StartM: StartM)
                    font {
                        pixelSize: utils.dp(15)
                        bold: true
                    }
                    anchors {
                        bottom: fromInfo.top
                        bottomMargin: utils.dp(5)
                        left: parent.left
                        leftMargin: utils.dp(64.5)
                    }
                    color: "#1BA1E2"
                }
                Text {
                    id: arrInfo
                    text: scheduleBorder.formatTimeValue(ArriveH) +
                          ":" +
                          (ArriveM < 10? "0"+ArriveM: ArriveM)
                    font {
                        pixelSize: utils.dp(15)
                        bold: true
                    }
                    anchors {
                        bottom: toInfo.top
                        bottomMargin: utils.dp(5)
                        right: parent.right
                        rightMargin: utils.dp(64.5)
                    }
                    color: "#1BA1E2"
                    
                }
            }
        }
        function formatTimeValue(value) {
            var temp = value % 24
            return temp < 10? "0" + temp: temp
        }
    }
    TextField {
        width: utils.dp(180)
        anchors.left: scheduleBorder.left
        anchors.top: scheduleBorder.bottom
        anchors.margins: utils.dp(5)
        placeholderText: "Select city to set up"
        placeholderTextColor: utils.colorClouds
        background: Rectangle {
            color: "#647687"
        }
        Rectangle {
            visible: parent.activeFocus
            anchors.fill: parent
            color: "transparent"
            border.color: "#00ABA9"
            border.width: utils.dp(1)
        }
        font.bold: true
        color: "#FFFFFF"
        onEditingFinished: {
            scheduleView.sFilter = text
            scheduleView.positionViewAtBeginning()
        }
    }
    TextField {
        width: utils.dp(180)
        anchors.right: scheduleBorder.right
        anchors.top: scheduleBorder.bottom
        anchors.margins: utils.dp(5)
        placeholderText: "Select city to arrive"
        placeholderTextColor: utils.colorClouds
        background: Rectangle {
            color: "#647687"
        }
        Rectangle {
            visible: parent.activeFocus
            anchors.fill: parent
            color: "transparent"
            border.color: "#00ABA9"
            border.width: utils.dp(1)
        }
        font.bold: true
        color: "#FFFFFF"
        onEditingFinished: {
            scheduleView.aFilter = text
            scheduleView.positionViewAtBeginning()
        }
    }
}

/*  This code block is used for positioning components  */
/*
    Drag.active: true
    MouseArea {
        anchors.fill: parent
        drag.target: parent
        onReleased: {
            console.log(
                parent.x / utils.dp(1),
                parent.y / utils.dp(1)
            )
        }
    }             
*/                
