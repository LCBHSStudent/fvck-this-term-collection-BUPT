import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

import nsp.DS.Backend 1.14

import "./Components/"
import "./Page/"

ApplicationWindow {
    id:      root
    visible: true
    flags:   Qt.FramelessWindowHint | Qt.Window
    title:   "ConvTravelEmulator"
    width:   (Qt.platform.os === "windows" || Qt.platform.os === "mac")?
                 1280:
                 Screen.desktopAvailableWidth
    height:  (Qt.platform.os === "windows" || Qt.platform.os === "mac")?
                 720:
                 Screen.desktopAvailableHeight
    
    signal sigPauseAnimation()
    signal sigContinueAnimation()
    
    UIBase {
        id: utils
    }
    
    Backend {
        id: backend
        onSigLoadDataFinished: {
            console.log("load data finished");
        }
        onSigCloseRootWindow: {
            root.close()
        }
    }
    
//    onFocusObjectChanged: {
//        console.log("Fcous Object Changed!", object)
//    }
    
    header: ToolBar {
        id: topBar
        contentHeight: utils.dp(10)
        
        MouseArea {
            anchors.fill: parent
            property real preX: 0.0
            property real preY: 0.0
            onPressed: {
                preX = mouseX
                preY = mouseY
            }

            onPositionChanged: {
                root.x += mouseX - preX
                root.y += mouseY - preY
            }
        }
        
        ToolButton {
            id: toolButton
            text: stack.depth > 1 ? "\u25C0" : "\u2630"
            font {
                bold: true
                pixelSize: utils.dp(18)
            }
            onClicked: {
                if (stack.depth > 1) {
                    stack.pop()
                } else {
                    drawer.open()
                }
            }
            anchors.verticalCenter: parent.verticalCenter
        }
        
        Text {
            text: "ConvTravelEmulator"
            anchors.centerIn: parent
            font {
                bold: true
                pixelSize: utils.dp(18)
            }
            color: utils.colorClouds
        }
        
        MouseArea {
            id: minisizeBtn
            anchors.right:          closeBtn.left
            anchors.rightMargin:    utils.dp(10)
            anchors.verticalCenter: parent.verticalCenter
            hoverEnabled:   true
            height:         utils.dp(15)
            width:          height
            
            onEntered: minisizeIcon.source = "qrc:/res/icons/MinimizeHover.png"
            onExited:  minisizeIcon.source = "qrc:/res/icons/Minimize.png"
            
            onClicked: {
                root.showState = utils.showMinimized
            }
            Image {
                id: minisizeIcon
                source: "qrc:/res/icons/Minimize.png"
                anchors.centerIn: parent
            }
        }
        MouseArea {
            id: closeBtn
            anchors.right:          parent.right
            anchors.rightMargin:    utils.dp(10)
            anchors.verticalCenter: parent.verticalCenter
            hoverEnabled:   true
            height:         utils.dp(15)
            width:          height
            
            onEntered: closeIcon.source = "qrc:/res/icons/CloseHover.png"
            onExited:  closeIcon.source = "qrc:/res/icons/Close.png"
            
            onClicked: {
                root.showState = utils.close
            }
            Image {
                id: closeIcon
                source: "qrc:/res/icons/Close.png"
                anchors.centerIn: parent
            }
        }   
        
    }
    
    Drawer {
        id: drawer
        width: root.width * 0.66
        height: root.height
        edge: Qt.LeftEdge
        contentItem: MainDrawerPage {
            id: mainDrawerPage
        }
        dragMargin: utils.dp(20)
        enabled: stack.depth === 1
        onOpened: {
            // console.log("open")
            backend.invokePauseMainThrd()
            root.sigPauseAnimation()
        }
        onClosed: {
            // console.log("close")
            backend.invokeContinueMainThrd()
            root.sigContinueAnimation()
        }
    }
    
    StackView {
        id: stack
        initialItem: OverviewPage{ id: overviewPage }
        anchors.fill: parent
    }
    
    property int showState: 0;
    onShowStateChanged: {
        if(showState === utils.showNormal)
            root.showNormal()
        if(showState == -1){
            try {
                root.close()
                // root.destroy(10);
            }
            catch(err){
                console.debug(root, err)
            }
        }
        if(showState === utils.showMinimized)
            root.showMinimized()
    }
    
    onWindowStateChanged: {
        switch(windowState) {
        case 0x00000000:
            showState = utils.showNormal
            break
        case 0x00000001:
            showState = utils.showMinimized
            break
        default:
            break
        }
    }
    
    onClosing: {
        backend.setRunning(false)
    }
}
