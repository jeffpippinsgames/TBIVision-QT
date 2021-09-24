import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"
import tbi.vision.components 1.0


Rectangle {
    id: rootitemId
    width: 220
    height: 220
    color: "transparent"
    radius: 10
    border.width: 3
    border.color: Qt.rgba(.8, .8, .0, .7)



    property string axisname: "X"
    property int position: 0
    property int microcontrollermotionstatus: 0
    property color textcolor: "yellow"
    property bool microcontrollerconnected: SerialPortController.isconnected

    //property string fontsource2: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource2: "qrc:/Fonts/EurostileBold.ttf"
    property string fontsource2: "qrc:/Fonts/Measurements.ttf"
    property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    //property string fontsource: "qrc:/Fonts/PermanentMarker-Regular.ttf"
    //property string fontsource2: "qrc:/Fonts/Calculator.ttf"

    readonly property color mode1color: Qt.rgba(.8, .8, .0, .7) //yellow
    readonly property color mode2color: Qt.rgba(.8, .0, .0, .7) //red
    readonly property color mode3color: Qt.rgba(.0, .0, .8, .7) //blue
    readonly property color mode4color: Qt.rgba(.2, .8, .2, .7) //green - chartreuse
    readonly property color mode5color: Qt.rgba(.3, .3, .8, .7) //cyan


    function setStatus()
    {
        switch(rootitemId.microcontrollermotionstatus)
        {
            case GaryMotionStatus.TBI_MOTION_STATUS_IDLE:
                statustextId.text = "Idle";
                statustextId.color = "yellow";
                axisnametextId.color = "yellow";
                glowId.color = "yellow";
                positiontextId.color = "yellow"
                rootitemId.border.color = rootitemId.mode1color;
                break;
            case GaryMotionStatus.TBI_MOTION_STATUS_GENERAL_ERROR:
                statustextId.text = "Error";
                statustextId.color = "red";
                axisnametextId.color = "red";
                glowId.color = "red";
                positiontextId.color = "red";
                rootitemId.border.color = rootitemId.mode2color;
                break;
            case GaryMotionStatus.TBI_MOTION_STATUS_HOMING:
                statustextId.text = "Homing";
                statustextId.color = "blue";
                axisnametextId.color = "blue";
                glowId.color = "blue";
                positiontextId.color = "blue";
                rootitemId.border.color = rootitemId.mode3color;
                break;
            case GaryMotionStatus.TBI_MOTION_STATUS_LIMIT_TRIPPED:
                statustextId.text = "Limit Tripped";
                statustextId.color = "red";
                axisnametextId.color = "red";
                glowId.color = "red";
                positiontextId.color = "red";
                rootitemId.border.color = rootitemId.mode2color;
                break;
            case GaryMotionStatus.TBI_MOTION_STATUS_JOGGING:
                statustextId.text = "Jogging";
                statustextId.color = "chartreuse";
                axisnametextId.color = "chartreuse";
                glowId.color = "chartreuse";
                positiontextId.color = "chartreuse";
                rootitemId.border.color = rootitemId.mode4color;
                break;
            case GaryMotionStatus.TBI_MOTION_STATUS_MOVING:
                statustextId.text = "Moving";
                statustextId.color = "cyan";
                axisnametextId.color = "cyan";
                glowId.color = "cyan";
                positiontextId.color = "cyan";
                rootitemId.border.color = rootitemId.mode5color;
                break;
        }
    }

    Component.onCompleted:
    {
        rootitemId.setStatus();
        if(!rootitemId.microcontrollerconnected)
        {
            positiontextId.visible = false;
            statustextId.text = "Com Error";
            statustextId.color = "red";
            axisnametextId.color = "red";
            glowId.color = "red";
            positiontextId.color = "red";
            rootitemId.border.color = rootitemId.mode2color;
        }
    }

    onMicrocontrollerconnectedChanged:
    {
        if(!rootitemId.microcontrollerconnected)
        {
            positiontextId.visible = false;
            statustextId.text = "Com Error";
            statustextId.color = "red";
            axisnametextId.color = "red";
            glowId.color = "red";
            positiontextId.color = "red";
            rootitemId.border.color = rootitemId.mode2color;
        }
        else
        {
            //statustextId.font.pointSize = 20;
            positiontextId.visible = true;
            setStatus();
        }
    }

    onMicrocontrollermotionstatusChanged:
    {
        rootitemId.setStatus();

    }

    //Font for UI
    FontLoader
    {
        id: fontId
        source: rootitemId.fontsource
    }

    //Font for UI
    FontLoader
    {
        id: font2Id
        source: rootitemId.fontsource2
    }

    Image
    {
        id: motorimageId
        width: 150;
        height: 150;
        x: (rootitemId.width - motorimageId.width)/2
        y: axisnametextId.y + axisnametextId.height - 15
        visible: true
        source: "qrc:/Icons/Motor2.png"
        opacity: .1
    }


    Glow
    {
        id: glowId
        anchors.fill: motorimageId
        radius: 8
        samples: 25
        color: "yellow"
        source: motorimageId
        opacity: .5
    }

    SequentialAnimation
    {
                id: glowsequentialanimationId
                running: false
                loops: Animation.Infinite
    }
    //Axis Text
    Text
    {
        id: axisnametextId
        visible: true
        focus: false
        font.family: font2Id.name
        text: rootitemId.axisname
        font.pointSize: 20
        width: axisnametextId.implicitWidth
        height: axisnametextId.implicitHeight
        x: (rootitemId.width - axisnametextId.width)/2
        y: 5
        color: rootitemId.textcolor
        opacity: .7
    }

    //status Text
    Text
    {
        id: statustextId
        visible: true
        focus: false
        font.family: font2Id.name
        text: "Idle"
        font.pointSize: 20
        width: statustextId.implicitWidth
        height: statustextId.implicitHeight
        x: (rootitemId.width - statustextId.width)/2
        y: rootitemId.height - statustextId.height-5
        color: rootitemId.textcolor
        opacity: .7
    }

    //Position Text
    Text
    {
        id: positiontextId
        visible: true
        focus: false
        font.family: font2Id.name
        text: "Pos:" + rootitemId.position
        font.pointSize: 20
        width: statustextId.implicitWidth
        height: statustextId.implicitHeight
        x: 5
        y: motorimageId.y + motorimageId.height - 10
        color: rootitemId.textcolor
        opacity: .7
    }
}
