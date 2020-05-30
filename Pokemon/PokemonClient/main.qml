import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    
    Button {
        anchors.centerIn: parent
        width: 50
        height: width
        onClicked: {
            backend.sendSignUpRequest("114514191981011451419198101145141919810", "364364364364")
        }
    }
}
