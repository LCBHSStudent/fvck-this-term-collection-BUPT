import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

import "./components/"
import "./pages/"

ApplicationWindow {
    id:      root
    visible: true
    flags:   Qt.Window // | Qt.FramelessWindowHint
    title:   "Pokemon"
    color:   "black"
    width:   (Qt.platform.os === "windows" || Qt.platform.os === "mac")?
                 468:
                 Screen.desktopAvailableWidth
    height:  (Qt.platform.os === "windows" || Qt.platform.os === "mac")?
                 832:
                 Screen.desktopAvailableHeight
    
    function w(val) {return val/100.0*root.width}
    
    property alias loadingPopup: loadingPopup_t
    
    UIBase {id: utils}
    
    StackView {
        id: stack
        initialItem: LoginPage {id: loginPage}   // 测试更改中
        anchors.fill: parent
        
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
    
    LoadingPopup {
        id: loadingPopup_t
        z:  999
    }
    
    function getRandomInt(max) {
      return Math.floor(Math.random() * Math.floor(max));
    }
}
