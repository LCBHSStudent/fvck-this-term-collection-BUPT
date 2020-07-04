import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    id: editLine;
    color: "transparent";
    property alias icon: editIco.source
    property alias text: editField.text
    property alias validator:       editField.validator
    property alias placeholderText: editField.placeholderText
    property alias echoMode:        editField.echoMode
    // 判断是否为密码输入框
    property bool isPsw:            placeholderText === "请输入密码"
    Image {
        id: editIco;
        source: ""
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: utils.dp(5)
        }
        height: parent.height
        width: height
        fillMode: Image.PreserveAspectFit
    }
    TextField {
        clip: false
        id: editField
        anchors {
            verticalCenter: parent.verticalCenter
            left: editIco.right
            leftMargin: utils.dp(5)
            right: showPsw.left
        }
        width: parent.width - editIco.width
        height: parent.height
        implicitWidth: width - showPsw.width
        font{
            pixelSize: (isPsw && text !== "" && !showPsw.showpsw)?
                        parent.height * 0.2:
                        parent.height * 0.6
            family: "微软雅黑"
            // 若是密码框且输入内容不为空
        }
        background: Rectangle{
            color: "transparent"
        }
        maximumLength: 24
        color: "#EEEEEE"
        placeholderTextColor: "#FFFFFF"
    }
    Rectangle{
        id:     underLine
        height: 1
        color:  "grey"
        opacity: 0.5
        anchors {
            top:            editField.bottom
            left:           editIco.right
            leftMargin:     utils.dp(5)
            right:          editLine.right
            rightMargin:    utils.dp(5)
        }
    }
    
    Image {
        id: showPsw;
        scale: 0.75;
        property bool showpsw: false;
        anchors {
            verticalCenter: parent.verticalCenter;
            right: parent.right;
            rightMargin: utils.dp(5);
        }
        source: isPsw? "qrc:/res/ui/ic_eye_off.png": "";
        MouseArea {
            anchors.fill: parent;
            onClicked: {
                if(!parent.showpsw){
                    parent.showpsw = !parent.showpsw;
                    parent.source = "qrc:/res/ui/ic_eye_on.png";
                    echoMode = TextInput.Normal;
                }
                else{
                    parent.showpsw = !parent.showpsw;
                    parent.source = "qrc:/res/ui/ic_eye_off.png";
                    echoMode = TextInput.Password;
                }
            }
        }
    }
}
