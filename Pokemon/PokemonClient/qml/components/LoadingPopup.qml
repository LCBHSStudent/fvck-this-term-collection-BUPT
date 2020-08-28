import QtQuick 2.14
import QtQuick.Controls 2.14

Popup {
    id: popup_t
    x: 0; y: 0
    width: 0; height: 0
    margins: 0; padding: 0
    modal: true; focus: true
    visible: opacity > 0
    opacity: 0
        
    Behavior on opacity {
        PropertyAnimation {
            duration: 150
        }
    }
    closePolicy: Popup.NoAutoClose
    
    Rectangle {
        clip: true
        y: (root.height - height) / 2
        width: root.width
        height: utils.dp(45)
        color: "#FFFFFF"
        Image {
            id: sync
            width: utils.dp(20)
            height: width
            anchors {
                right: content.left
                rightMargin: utils.dp(10)
                verticalCenter: parent.verticalCenter
            }
            
            source: "qrc:/res/ui/ic_sys_reset.png"
            PropertyAnimation {
                target: sync
                properties: "rotation"
                from: 0
                to: 360
                duration: 800
                loops: Animation.Infinite
                running: visible
            }
        }
        Text {
            id: content
            text: ""
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -2
            font.pixelSize: utils.dp(18)
            font.family: "微软雅黑"
        }
    }
    
    OneBtnToast {
        id: timeOutHint
        contentH: 300
        contentW: 350
    }
    
    Timer {
        id: timeOutTimer
        interval:   6000
        repeat:     false
        running:    false
        onTriggered: {
            hideLoading()
            timeOutHint.showPopup("请求超时", "知道了")
        }
    }
    
    
    function showLoading(contentText, timeout = true) {
        content.text = contentText
        popup_t.opacity = 1
        console.log(timeout)
        if (timeout) {
            timeOutTimer.start()
        }
    }
    function hideLoading() {
        popup_t.opacity = 0
        timeOutTimer.stop()
    }
}
