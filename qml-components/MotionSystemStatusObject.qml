import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"
import tbi.vision.components 1.0

Item
{
    id: rootitemId
    anchors.fill: parent

    property bool microcontrollerconnected: SerialPortController.isconnected

    property int rect_x: 0
    property int rect_y: 0
    property int rect_width: 450
    property int rect_height: 75

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

    function setStatus()
    {
       if(!microcontrollerconnected)
       {
           statustextId.color = "red";
           rootrectId.border.color = rootitemId.mode2color;
           glowId.color = "red";
       }
       else
       {
           statustextId.color = "chartreuse";
           rootrectId.border.color = rootitemId.mode4color;
           glowId.color = "chartreuse";
       }
    }

    onMicrocontrollerconnectedChanged:
    {
        rootitemId.setStatus();
    }

    Component.onCompleted:
    {
        setStatus();
    }


    Rectangle
    {
        id: rootrectId
        width: rootitemId.rect_width
        height: rootitemId.rect_height
        color: "transparent"
        radius: 10
        border.width: 3
        border.color: Qt.rgba(.8, .8, .0, .7)
        x: rootitemId.rect_x
        y: rootitemId.rect_y

        Image
        {
            id: imageId
            width: 55;
            height: 55;
            x: 5
            y: (rootrectId.height - imageId.height)/2
            visible: true
            source: "qrc:/Icons/microcontroller(white).png"
            opacity: .1
        }

        Glow
        {
            id: glowId
            anchors.fill: imageId
            radius: 8
            samples: 25
            color: "yellow"
            source: imageId
            opacity: .5
        }

        //Camera Name Text
        Text
        {
            id: statustextId
            visible: true
            focus: false
            font.family: font2Id.name
            text: "Motion System Status"
            font.pointSize: 20
            width: statustextId.implicitWidth
            height: statustextId.implicitHeight
            x: imageId.x+imageId.width+10
            y: (rootrectId.height - statustextId.height)/2
            color: rootitemId.textcolor
            opacity: .7

        }

    }

}
