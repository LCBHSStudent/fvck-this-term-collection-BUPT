import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Universal 2.14
import nsp.DS.Backend 1.14

Page {
    id: mDrawerPage
    anchors.fill: parent
    background: Rectangle {
        anchors.fill: parent
        color: "transparent"
    }
    
    Button {
        id: visualPageBtn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: utils.dp(30)
        anchors.bottomMargin: utils.dp(20)
        text: "Travel System Details"
        onClicked: {
            drawer.close()
            stack.push("qrc:/qml/Page/DataVisualizationPage.qml")
        }
    }
    
    Row {
        id: cityRow
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: utils.dp(30)
        spacing: utils.dp(30)
        ComboBox {
            id: fromCityBox
            width: utils.dp(150)
            height: utils.dp(30)
            
            
            textRole:   "CityName"
            valueRole:  "CityName"
            model: backend.cityModel
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.top
                anchors.bottomMargin: utils.dp(5)
                font.pixelSize: utils.dp(15)
                font.bold: true
                color: utils.colorClouds
                text: "FromCity"
            }
        }
        ComboBox {
            id: destCityBox
            width: utils.dp(150)
            height: utils.dp(30)
            
            textRole:   "CityName"
            valueRole:  "CityName"
            model: backend.cityModel
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.top
                anchors.bottomMargin: utils.dp(5)
                font.pixelSize: utils.dp(15)
                font.bold: true
                color: utils.colorClouds
                text: "DestCity"
            }
        }
    }
    
    Column {
        id: travelTypeBtn
        anchors.left: cityRow.left
        anchors.top: cityRow.bottom
        anchors.topMargin: utils.dp(5)
        
        property int travelType: Backend.SafetyRefered
        
        RadioButton { 
            text: qsTr("Safety Refered")
            checked: travelTypeBtn.travelType === Backend.SafetyRefered
            onClicked: {
                travelTypeBtn.travelType = Backend.SafetyRefered
            }
        }
        RadioButton {
            text: qsTr("Time Refered")
            checked: travelTypeBtn.travelType  === Backend.TimeRefered
            onClicked: {
                travelTypeBtn.travelType = Backend.TimeRefered
            }
        }
    }
    
    Text {
        anchors.left: timeInputRow.left
        anchors.horizontalCenter: timeInputRow.horizontalCenter
        anchors.bottom: timeInputRow.top
        anchors.bottomMargin: utils.dp(5)
        text: qsTr("Start Time")
        color: utils.colorClouds
        font.pixelSize: utils.dp(15)
        font.bold: true
    }
    
    Row {
        id: timeInputRow
        anchors.left: travelTypeBtn.right
        anchors.leftMargin: utils.dp(53)
        anchors.bottom: travelTypeBtn.bottom
        spacing: utils.dp(4)
        TextField {
            id: startH
            width: utils.dp(30)
            placeholderText: "HH"
            placeholderTextColor: utils.colorClouds
            validator: RegExpValidator {regExp: /^[0-9]*$/}
            maximumLength: 2
            
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
        }
        Text {
            text: qsTr(":")
            color: utils.colorClouds
            font.pixelSize: utils.dp(15)
            font.bold: true
            anchors.verticalCenter: parent.verticalCenter
        }
        TextField {
            id: startM
            width: utils.dp(30)
            placeholderText: "MM"
            placeholderTextColor: utils.colorClouds
            validator: RegExpValidator {regExp: /^[0-9]*$/}
            maximumLength: 2
            
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
        }
    }     
    
    
    Button {
        id: goBtn
        anchors.bottom: timeInputRow.bottom
        anchors.left: timeInputRow.right
        anchors.topMargin: utils.dp(10)
        anchors.leftMargin: utils.dp(10)
        text: "   GO!   "
        background: Rectangle {
            id: btnBg
            color: "#1BA1E2"
            radius: utils.dp(1)
            Behavior on color {
                ColorAnimation {
                    duration: 200
                }
            }
        }
        onPressedChanged: {
            btnBg.color = pressed? "#3E65FF": "#1BA1E2"
        }
        onClicked: {
            backend.slotStartQuery(
                fromCityBox.currentValue,
                destCityBox.currentValue,
                startH.text,
                startM.text,
                travelTypeBtn.travelType
            )
        }
    }
    
    TextArea {
        id: field
        anchors {
            top: goBtn.bottom
            bottom: visualPageBtn.top
            left: parent.left
            right: parent.right
            margins: utils.dp(20)
        }
        enabled: false
        
        color: utils.colorClouds
        
        Connections {
            target: backend
            onSigNewMessage: {
                field.append(msg)
            }
        }
    }
}       
