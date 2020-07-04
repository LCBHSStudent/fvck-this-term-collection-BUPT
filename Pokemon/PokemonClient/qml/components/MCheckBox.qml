import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle{
    id: check;
    property bool checked: true;
    height: forgetPsw.height;
    width: checkImg.width + checkTxt.width;
    color: "transparent";
    Image {
        id: checkImg;
        source: parent.checked? 
                    "../../assets/mdpi/radio_on.png": 
                    "../../assets/mdpi/radio_off.png";
        height: checkTxt.height;
        fillMode: Image.PreserveAspectFit;
        MouseArea {
            anchors.fill: parent;
            onClicked: {
                check.checked = !check.checked;
            }
        }
        Rectangle {
            anchors.centerIn: parent;
            width: 0;
            height: width;
            radius: width / 2;
            color: check.checked? tintColor: "grey";
            PropertyAnimation {
                id: checkAni;
                target: parent;
                property: "width";
                
            }
        }
    }
    Text {
        id: checkTxt;
        anchors.left: checkImg.right;
        anchors.leftMargin: dp(1);
        text: "记住密码";
        color: tintColor;
        font{
            pixelSize: dp(4);
            bold: true;
        }
    }
}
