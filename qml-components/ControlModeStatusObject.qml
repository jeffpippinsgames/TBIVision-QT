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
    height: 150
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

    readonly property color mode1color: Qt.rgba(.8, .8, .0, .7) //yellow
    readonly property color mode2color: Qt.rgba(.8, .0, .0, .7) //red
    readonly property color mode3color: Qt.rgba(.0, .0, .8, .7) //blue
    readonly property color mode4color: Qt.rgba(.2, .8, .2, .7) //green - chartreuse
    readonly property color mode5color: Qt.rgba(.3, .3, .8, .7) //cyan


    Component.onCompleted:
    {
        setState();

        if(!rootitemId.microcontrollerconnected)
        {
            controlimageId.source = rootitemId.manualiconresource;
            glowId.color = "red";
            controlmodetextId.color = "red";
            controlmodetextId.text = "Com Error";
            rootitemId.border.color = rootitemId.mode2color;
            tittletextId.color = "red";
        }
    }

    onMicrocontrollerconnectedChanged:
    {
        if(!rootitemId.microcontrollerconnected)
        {
            controlimageId.source = rootitemId.manualiconresource;
            glowId.color = "red";
            controlmodetextId.color = "red";
            controlmodetextId.text = "Com Error";
            rootitemId.border.color = rootitemId.mode2color;
            tittletextId.color = "red";
        }
        else
        {
            setState();
        }
    }

    onMicrocontrollermodeChanged:
    {
        rootitemId.setState();
    }

    function setState()
    {
        switch(rootitemId.microcontrollermode)
        {
        case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE: //yellow
            controlimageId.source = rootitemId.manualiconresource;
            glowId.color = "yellow";
            controlmodetextId.color = "yellow";
            controlmodetextId.text = "Manual Mode";
            rootitemId.border.color = rootitemId.mode1color;
            tittletextId.color = "yellow";
            break;
        case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE: //lime
            controlimageId.source = rootitemId.autoiconresource;
            glowId.color = "chartreuse";
            controlmodetextId.color = "chartreuse";
            controlmodetextId.text = "Full Auto";
            rootitemId.border.color = rootitemId.mode4color;
            tittletextId.color = "chartreuse";
            break;
        case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY: //cyan
            controlimageId.source = rootitemId.heighticonresource;
            glowId.color = "cyan";
            controlmodetextId.color = "cyan"
            controlmodetextId.text = "Auto Height";
            rootitemId.border.color = rootitemId.mode5color;
            tittletextId.color = "cyan";
            break;
        case GaryControlMode.TBI_CONTROL_MODE_MOTORCALIBRATION: //aquamarine
            controlimageId.source = rootitemId.calibrationiconresource;
            glowId.color = "blue";
            controlmodetextId.color = "blue";
            controlmodetextId.text = "Calibration";
            rootitemId.border.color = rootitemId.mode3color;
            tittletextId.color = "blue";
            break;
        }
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

    Text
    {
      id: tittletextId
      visible: false
      focus: false
      font.family: font2Id.name
      text: "Control Mode"
      font.pointSize: 30
      width: tittletextId.implicitWidth
      height: tittletextId.implicitHeight
      x: (rootitemId.width - tittletextId.width) - 5
      y: 5
      opacity: .7

    }

    Text
    {
        id: controlmodetextId
        visible: true
        focus: false
        font.family: font2Id.name
        text: ""
        font.pointSize: 30
        width: controlmodetextId.implicitWidth
        height: controlmodetextId.implicitHeight
        x: controlimageId.x + controlimageId.width + 5//controlimageId.x + controlimageId.width + 5
        y: (rootitemId.height - controlmodetextId.height)/2
        color: rootitemId.textcolor
        opacity: .7
    }

    Image
    {
        id: controlimageId
        width: 125;
        height: 125;
        x: 0
        y: (rootitemId.height - controlimageId.height)/2
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
