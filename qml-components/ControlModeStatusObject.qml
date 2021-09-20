import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"
import tbi.vision.components 1.0

Rectangle
{
    id: rootitemId
    width: 450
    height: 100
    color: "transparent"
    radius: 10
    border.width: 3
    border.color: Qt.rgba(.8, .8, .0, .7)

    //property string fontsource2: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource2: "qrc:/Fonts/EurostileBold.ttf"
    property string fontsource2: "qrc:/Fonts/Measurements.ttf"
    property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    //property string fontsource: "qrc:/Fonts/PermanentMarker-Regular.ttf"
    //property string fontsource2: "qrc:/Fonts/Calculator.ttf"

    property int microcontrollermode: MicroControllerStatusPacket.controlMode
    property color textcolor: "green"
    property bool microcontrollerconnected: SerialPortController.isconnected

    readonly property string manualiconresource: "qrc:/Icons/manualmode.png"
    readonly property string autoiconresource: "qrc:/Icons/automaticmode.png"
    readonly property string heighticonresource: "qrc:/Icons/heightonlymode.png"
    readonly property string calibrateiconresource: "qrc:/Icons/calibrationmode.png"

    Component.onCompleted:
    {
        setState();
    }

    onMicrocontrollerconnectedChanged:
    {
        if(!rootitemId.microcontrollerconnected)
        {
            controlimageId.source = rootitemId.manualiconresource;
            glowId.color = "red";
            titletextId.color = "red";
            titletextId.text = "Connection Error: Manual Only";
        }
        else
        {
            setState();
        }
    }

    function setState()
    {
        switch(rootitemId.microcontrollermode)
        {
        case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE: //lime
            controlimageId.source = rootitemId.manualiconresource;
            glowId.color = "lime";
            titletextId.color = "lime";
            titletextId.text = "Control Mode: Manual";
            break;
        case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE: //cyan
            controlimageId.source = rootitemId.autoiconresource;
            glowId.color = "cyan";
            titletextId.color = "cyan";
            titletextId.text = "Control Mode: Full Auto";
            break;
        case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY: //yellow
            controlimageId.source = rootitemId.heighticonresource;
            glowId.color = "yellow";
            titletextId.color = "yellow"
            titletextId.text = "Control Mode: Auto Height";
            break;
        case GaryControlMode.TBI_CONTROL_MODE_MOTORCALIBRATION: //aquamarine
            controlimageId.source = rootitemId.calibrationiconresource;
            glowId.color = "aquamarine";
            titletextId.color = "aquamarine";
            titletextId.text = "Control Mode: Calibration";
            break;
        }
    }

    Text
    {
        id: titletextId
        visible: true
        focus: false
        font.family: font2Id.name
        text: "Control Mode: "
        font.pointSize: 20
        width: titletextId.implicitWidth
        height: titletextId.implicitHeight
        x: controlimageId.x + controlimageId.width + 5
        y: (rootitemId.height-titletextId.height)/2
        color: rootitemId.textcolor
        opacity: .7
    }

    Image
    {
        id: controlimageId
        width: 75;
        height: 75;
        x: 5
        y: (rootitemId.height-controlimageId.height)/2
        visible: true
        opacity: .1
    }

    Glow
    {
        id: glowId
        anchors.fill: controlimageId
        radius: 8
        samples: 25
        color: "yellow"
        source: controlimageId
        opacity: .5
    }




}
