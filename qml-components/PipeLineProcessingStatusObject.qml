import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"
import tbi.vision.components 1.0

Item {
    id: rootitemId
    anchors.fill: parent

    //property string fontsource2: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource2: "qrc:/Fonts/EurostileBold.ttf"
    property string fontsource2: "qrc:/Fonts/Measurements.ttf"
    //property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string fontsource: "qrc:/Fonts/PermanentMarker-Regular.ttf"
    //property string fontsource2: "qrc:/Fonts/Calculator.ttf"

    property bool showstatusstring: true
    property int statusstring_x: 5
    property int statusstring_y: 5
    property int status_x: 5
    property int status_y: 5
    property int rect_width: 450
    property int rect_height: 75

    property int pipelinestatus: ProcessingPipeLineStatus.status
    property string pipelinestatusstring: ProcessingPipeLineStatus.statusString

    property bool microcontrollerconnected: MicroControllerStatusPacket.serialportconnected

    readonly property string okiconresource: "qrc:/Icons/okcheck.png"
    readonly property string notokiconresource: "qrc:/Icons/cross.png"

    readonly property color mode1color: Qt.rgba(.8, .8, .0, .7) //yellow
    readonly property color mode2color: Qt.rgba(.8, .0, .0, .7) //red
    readonly property color mode3color: Qt.rgba(.0, .0, .8, .7) //blue
    readonly property color mode4color: Qt.rgba(.2, .8, .2, .7) //green - chartreuse
    readonly property color mode5color: Qt.rgba(.3, .3, .8, .7) //cyan

    function setStatus()
    {
        switch(rootitemId.pipelinestatus)
        {
        case TBIWeld_ProcessingPipeLineReturnType.TBI_PIPELINE_OK:
            statusimageId.source = rootitemId.okiconresource;
            glowId.color = rootitemId.mode4color;
            statustextId.text = "Tracking Point"
            statustextId.color = "chartreuse";
            statusrectId.border.color = rootitemId.mode4color;
            statusstringtextId.color = "chartreuse"
            break;
        default:
            statusimageId.source = rootitemId.notokiconresource;
            glowId.color = rootitemId.mode2color;
            statustextId.text = "Tracking Point"
            statustextId.color = "red";
            statusrectId.border.color = rootitemId.mode2color;
            statusstringtextId.color = "red";
            break;
        }
    }

    Component.onCompleted:
    {
        setStatus();
    }

    onMicrocontrollerconnectedChanged:
    {
        if(!rootitemId.microcontrollerconnected)
        {
            statusimageId.source = rootitemId.notokiconresource;
            glowId.color = rootitemId.mode2color;
            statustextId.text = "Com Error"
            statustextId.color = "red";
            statusrectId.border.color = rootitemId.mode2color;
            statusstringtextId.color = "red";
        }
        else
        {
            rootitemId.setStatus();
        }
    }

    onPipelinestatusChanged:
    {
        setStatus();
    }

    onPipelinestatusstringChanged:
    {
        setStatus();
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

    //Status Rect Indicator
    Rectangle
    {
        id: statusrectId
        width: rootitemId.rect_width
        height: rootitemId.rect_height
        color: "transparent"
        radius: 10
        border.width: 3
        border.color: Qt.rgba(.8, .8, .0, .7)
        x: rootitemId.status_x
        y: rootitemId.status_y

        Image
        {
            id: statusimageId
            width: 55;
            height: 55;
            x: 5
            y: (statusrectId.height-statusimageId.height)/2
            visible: true
            opacity: .1
        }

        Glow
        {
            id: glowId
            anchors.fill: statusimageId
            radius: 8
            samples: 25
            color: "red"
            source: statusimageId
            opacity: .5
        }

        Text
        {
            id: statustextId
            visible: true
            focus: false
            font.family: font2Id.name
            text: "Tracking Point"
            font.pointSize: 25
            width: statustextId.implicitWidth
            height: statustextId.implicitHeight
            x: statusimageId.x+statusimageId.width+10 //(statusrectId.width - statustextId.width)/2
            y: (statusrectId.height - statustextId.height)/2
            opacity: .7


        }
    }

    //Status String Indlicator
    Text
    {
        id: statusstringtextId
        visible: rootitemId.showstatusstring
        focus: false
        font.family: fontId.name
        text: "Pipeline Status: " + rootitemId.pipelinestatusstring
        font.pointSize: 30
        x: rootitemId.statusstring_x
        y: rootitemId.statusstring_y
        width: statusstringtextId.implicitWidth
        height: statusstringtextId.implicitHeight
        opacity: .7
    }

}
