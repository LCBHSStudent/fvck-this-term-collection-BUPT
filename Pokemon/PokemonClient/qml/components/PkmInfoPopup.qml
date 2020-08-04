import QtQuick 2.14
import QtQuick.Controls 2.14

Popup {
    id: info
    x: (root.width - bgRect.width) / 2
    y: (root.height - bgRect.height) / 2
    width: 0; height: 0
    margins: 0; padding: 0
    modal: true; focus: true
    visible: opacity > 0
    opacity: 0
    
    property var pkmData: ({})
    
    Behavior on opacity {
        PropertyAnimation {
            duration: 150
        }
    }
    closePolicy: Popup.NoAutoClose
    
    Rectangle {
        id: bgRect
        clip: true
        color: "#CCFFFFFF"
        width: root.width * 0.8
        height: root.height * 0.78
        radius: utils.dp(10)
        
        AnimatedImage {
            id: pkmGif
            anchors {
                left: parent.left
                top:  parent.top
                margins: utils.dp(20)
            }

            source: info.opacity !== 1?
                "":
                "qrc:/res/image/character/" + pkmData["typeId"] + "_oppo.gif"
        }
        
        MText {
            id: dictNumText
            visible: pkmGif.progress === 1
            font.pixelSize: utils.dp(12)
            anchors.horizontalCenter: pkmGif.horizontalCenter
            anchors.top: pkmGif.bottom
            anchors.topMargin: utils.dp(5)
            color: "#000000"
            text: "图鉴编号: " + pkmData["typeId"]
        }
        
        Column {
            visible: pkmGif.progress === 1
            anchors {
                left: pkmGif.right
                leftMargin: utils.dp(20) 
                verticalCenter: pkmGif.verticalCenter
            }
            spacing: utils.dp(2)
            MText {
                color: "#000000"
                wrapMode: Text.WrapAnywhere
                font.pixelSize: utils.dp(15)
                text: "持有编号: " + pkmData["_id"]
            }
            MText {
                color: "#000000"
                wrapMode: Text.WrapAnywhere
                font.pixelSize: utils.dp(16)
                text: "名称: " + pkmData["name"]
            }
            MText {
                color: "#000000"
                wrapMode: Text.WrapAnywhere
                width: utils.dp(150)
                font.pixelSize: utils.dp(14)
                text: pkmData["desc"]? pkmData["desc"]: ""
            }
        }
        
        Rectangle {
            id: outLineRect
            anchors.top: dictNumText.bottom
            anchors.bottom: confirmBtn.top
            anchors.margins: utils.dp(20)
            anchors.horizontalCenter: parent.horizontalCenter
            border.width: utils.dp(2)
            border.color: "yellow"
            color: "transparent"
            width: parent.width * 0.9
            radius: utils.dp(2)
            Grid {
                anchors {
                    top: parent.top
                    left: parent.left
                    bottom: skillInfoLayout.top
                    leftMargin: utils.dp(2)
                    topMargin: utils.dp(2)
                    bottomMargin: 0
                }
                rows: 4
                rowSpacing: utils.dp(1)
                columns: 2
                columnSpacing: utils.dp(1)
                Rectangle {
                    x: utils.dp(2)
                    y: utils.dp(2)
                    id: attributeInfo
                    color: "#CFD5DC"
                    width: outLineRect.width * 0.49
                    height: parent.height * 0.241
                    MText {
                        font.pixelSize: utils.dp(15)
                        color: "#000000"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: -1
                        text: "属性"
                    }
                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: parent.height / 2
                        color: {
                            switch (pkmData["attr"]) {
                            case 0:
                                return utils.colorAlizarin
                            case 1:
                                return utils.colorPeterRiver
                            case 2:
                                return utils.colorEmerald
                            default:
                                return "#FFFFFF"
                            }
                        }
                        MText {
                            font.pixelSize: utils.dp(15)
                            anchors.centerIn: parent
                            color: "#FFFFFF"
                            text: {
                                switch (pkmData["attr"]) {
                                case 0:
                                    return "火属性"
                                case 1:
                                    return "水属性"
                                case 2:
                                    return "草属性"
                                default:
                                    return "#FFFFFF"
                                }
                            }
                        }
                    }
                }
                Rectangle {
                    id: typeInfo
                    color: "#CFD5DC"
                    width: outLineRect.width * 0.49
                    height: parent.height * 0.241
                    MText {
                        font.pixelSize: utils.dp(15)
                        color: "#000000"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: -1
                        text: "类型"
                    }
                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: parent.height / 2
                        color: "#CCFFFFFF"
                        MText {
                            font.pixelSize: utils.dp(15)
                            color: "#000000"
                            anchors.centerIn: parent
                            text: {
                                switch (pkmData["type"]) {
                                case 0:
                                    return "高攻击型"
                                case 1:
                                    return "高耐久型"
                                case 2:
                                    return "高防御型"
                                case 3:
                                    return "高速度型"
                                default:
                                    return ""
                                }
                            }
                        }
                    }
                }
                
                Repeater {
                    model: [
                        ["等级", pkmData["level"]],
                        ["经验值", pkmData["exp"]],
                        ["生命值", pkmData["hp"]],
                        ["攻击力", pkmData["atk"]],
                        ["防御力", pkmData["def"]],
                        ["速度", pkmData["spd"]]
                    ]
                    delegate: Rectangle {
                        color: "#CFD5DC"
                        width: outLineRect.width * 0.49
                        height: parent.height * 0.241
                        MText {
                            font.pixelSize: utils.dp(15)
                            color: "#000000"
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top
                            anchors.topMargin: -1
                            text: modelData[0]
                        }

                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width
                            height: parent.height / 2
                            color: "#CCFFFFFF"
                            MText {
                                font.pixelSize: utils.dp(15)
                                color: "#000000"
                                anchors.centerIn: parent
                                text: {
                                    if (typeof(modelData[1]) === "undefined")
                                        return ""
                                    else 
                                        return JSON.stringify(modelData[1])
                                }
                            }
                        }
                    }
                }
            }
            
            Grid {
                id: skillInfoLayout
                anchors.bottom: parent.bottom
                anchors.bottomMargin: utils.dp(2)
                anchors.horizontalCenter: parent.horizontalCenter
                rows: 2
                rowSpacing: utils.dp(1)
                columns: 2
                columnSpacing: utils.dp(1)
                Repeater {
                    model: [
                        [pkmData["skill_1"], pkmData["desc_s1"]],
                        [pkmData["skill_2"], pkmData["desc_s2"]],
                        [pkmData["skill_3"], pkmData["desc_s3"]],
                        [pkmData["skill_4"], pkmData["desc_s4"]],
                    ]
                    delegate: Rectangle {
                        width: outLineRect.width * 0.48
                        height: utils.dp(45)
                        radius: utils.dp(3)
                        color: "#2CC486"
                        
                        Behavior on color {
                            PropertyAnimation { duration: 100 }
                        }
                        
                        Text {
                            id: skillName
                            text: modelData[0]? modelData[0]: ""
                            anchors {
                                left: parent.left
                                top:  parent.top
                                margins: utils.dp(3)
                            }
                            color: "#FFFFFF"
                            font.pixelSize: utils.dp(10)
                            font.family: "微软雅黑"
                            font.bold: true
                        }
                        
                        Text {
                            id: skillDesc
                            text: modelData[1]? modelData[1]: ""
                            width: parent.width - utils.dp(4)
                            wrapMode: Text.WrapAnywhere
                            anchors {
                                left: skillName.left
                                top:  skillName.bottom
                                margins: utils.dp(1)
                            }
                            color: "#FFFFFF"
                            font.pixelSize: utils.dp(7)
                            font.family: "微软雅黑"
                            font.bold: true
                        }
                        
                        MouseArea {
                            anchors.fill: parent
                            onEntered: {
                                parent.color = "#34D0C6"
                            }
                            onExited: {
                                parent.color = "#2CC486"
                            }
                        }
                    }
                }
            }
        }
        
        
        
        MFlatBtn {
            id: confirmBtn
            text: "确认"
            width: utils.dp(200)
            pressColor: "#34D0C6"
            releaseColor: "#2CC486"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: utils.dp(5)
            
            onClicked: {
                info.opacity = 0
            }
        }
    }
    
    function showPokemonInfo(
        _id, typeId, name, level, type,
        exp, attr,   atk,  def,   hp, spd,
        skill_1, skill_2, skill_3, skill_4,
        desc, desc_s1, desc_s2, desc_s3, desc_s4
    ) {
        info.opacity = 1
        pkmData = {
            "_id":      _id,
            "typeId":   typeId,
            "name":     name,
            "level":    level,
            "type":     type,
            "exp":      exp,
            "attr":     attr,
            "atk":      atk,
            "def":      def,
            "hp":       hp,
            "spd":      spd,
            "skill_1":  skill_1,
            "skill_2":  skill_2,
            "skill_3":  skill_3,
            "skill_4":  skill_4,
            "desc":     desc,
            "desc_s1":  desc_s1,
            "desc_s2":  desc_s2,
            "desc_s3":  desc_s3,
            "desc_s4":  desc_s4,
        }
    }
}
