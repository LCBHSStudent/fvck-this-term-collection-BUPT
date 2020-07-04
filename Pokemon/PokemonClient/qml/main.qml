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
    
    function dpH(val) {return val/100.0*root.height}
    function dpW(val) {return val/100.0*root.width}
    
    UIBase {id: utils}    
    
    StackView {
        id: stack
        initialItem: LoginPage {id: loginPage}
        anchors.fill: parent
    }
}
