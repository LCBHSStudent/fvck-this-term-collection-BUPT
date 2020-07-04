import QtQuick 2.14

QtObject {

    property real __pixelDensity: 4.5 // pixels/mm

    function cm(number) {
        return number * __pixelDensity * 10
    }

    /*!
       Converts millimeters into pixels. Used primarily by \l units::dp, but there might be other
       uses for it as well.
    */
    function mm(number) {
        return number * __pixelDensity
    }

    function inch(number) {
        return number * __pixelDensity * 10 * 2.54
    }

    // 相对以视窗的宽度，视窗宽度是100vm
    function vw(i, width) {
        return number * (width / 100)
    }

    // 相对以视窗的高度，视窗高度是100vh
    function vh(number, height) {
        return number * (height / 100)
    }

    function vmin(number, width, height) {
        return number * (Math.min(width, height) / 100)
    }

    function vmax(number, width, height){
        return number * (Math.max(width, height) / 100)
    }

    function dp(number) {
        var px = Math.round(number * (__pixelDensity * 25.4 / 160));

        if(Qt.platform.os === "windows" || Qt.platform.os === "mac")
            return px * 2;
        else
            return px;
    }
    
    /* provide a list of flat ui color. */
    // 绿色
    readonly property color colorTurquoise      : "#1abc9c"
    readonly property color colorGreenSea       : "#16a085"
    // 翠绿色
    readonly property color colorEmerald        : "#2ecc71"
    readonly property color colorNephritis      : "#27ae60"
    // 蓝色
    readonly property color colorPeterRiver     : "#3498db"
    readonly property color colorBelizeHole     : "#2980b9"
    // 紫色
    readonly property color colorAmethyst       : "#9b59b6"
    readonly property color colorWisteria       : "#8e44ad"
    // 沥青色
    readonly property color colorWetAsphalt     : "#34495e"
    readonly property color colorMidnightBlue   : "#2c3e50"
    // 橙色
    readonly property color colorSunFlower      : "#f1c40f"
    readonly property color colorOrange         : "#f39c12"
    // 胡萝卜色
    readonly property color colorCarrot         : "#e67e22"
    readonly property color colorPumpkin        : "#d35400"
    // 红色
    readonly property color colorAlizarin       : "#e74c3c"
    readonly property color colorPomegranate    : "#c0392b"
    // 白云色
    readonly property color colorClouds         : "#ecf0f1"
    readonly property color colorSilver         : "#bdc3c7"  // disable color
    // 水泥色
    readonly property color colorConcrete       : "#95a5a6"
    readonly property color colorAsbestos       : "#7f8c8d"
    
    function objectIsNull(object){
        return (!object && typeof(object)!="undefined" && object !== 0);
    }
    
    readonly property int close:          -1
    readonly property int showMinimized:   1
    readonly property int showNormal:      2
    readonly property int showMaxmized:    3
    readonly property int showFullScreen:  4
    
}

