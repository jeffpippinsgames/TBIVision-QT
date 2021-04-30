import QtQuick 2.0
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import "qrc:/qml-components"
import "qrc:/qml-pages"
import tbi.vision.components 1.0

Rectangle
{
    id: rootobjectId
    width: 600
    height: 85
    color: "transparent"
    border.color: nonhighlightcolor
    border.width: highlightbordersize

    property string controlname: "control_name"

    readonly property string nothighlightedstate: "NotHighlightedState"
    readonly property string highlightedstate: "HighlightedState"
    readonly property string focusedstate: "FocusedState"

    readonly property color focuscolor: Qt.rgba(0,1,0,.2)
    readonly property color nonfocuscolor: "transparent"
    readonly property color highlightcolor: Qt.rgba(0,1,0,.6)
    readonly property color nonhighlightcolor: Qt.rgba(1,1,1,.1)
    readonly property color valuebarcolor: Qt.rgba(1,0,0,.2)
    readonly property int highlightbordersize: 5

    property string messagetext: "Message Text"
    property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property color textcolor: Qt.rgba(1,1,.95,1)
    property int fontsize: 20
    property real valueto: 100
    property real valuefrom: 0
    property real value: 0
    property real stepsize: 1
    property real majorstepsize: 10

    property bool controlstickautorepeat: false
    property bool controlbuttonautorepeat: false

    signal endFocus()

    function getValue()
    {
        return rootobjectId.value;
    }

    function grabFocus()
    {
        controllerId.focus = true;
        rootobjectId.state = focusedstate;
    }

    function highlight()
    {
        rootobjectId.state = highlightedstate;
    }

    function endHighlight()
    {
        rootobjectId.state = nothighlightedstate;
    }

    states:
    [
        State
        {
            name: nothighlightedstate
            PropertyChanges{ target: rootobjectId; border.color: nonhighlightcolor}
            PropertyChanges{ target: focusrectId; color: nonfocuscolor}
        },
        State
        {
            name: highlightedstate
            PropertyChanges{ target: rootobjectId; border.color: highlightcolor}
            PropertyChanges{ target: focusrectId; color: nonfocuscolor}
        },
        State
        {
            name: focusedstate
            PropertyChanges{ target: rootobjectId; border.color: highlightcolor}
            PropertyChanges{ target: focusrectId; color: focuscolor}
        }

    ]

    Component.onCompleted:
    {
        rootobjectId.state = nothighlightedstate;
    }

    ControllerObject
    {

        id:controllerId
        focus: false

        useAutoRepeatonButtons: rootobjectId.controlbuttonautorepeat
        useAutoRepeatonSticks: rootobjectId.controlstickautorepeat

        onBlackButtonPressed:
        {
            rootobjectId.endFocus();
        }

        onGreenButtonPressed:
        {
            rootobjectId.endFocus();
        }

        onRedButtonPressed:
        {
            rootobjectId.endFocus();
        }

        onUpButtonPressed:
        {
            if((rootobjectId.value+rootobjectId.majorstepsize) <= rootobjectId.valueto)
            {
                rootobjectId.value = rootobjectId.value + rootobjectId.majorstepsize;
            }

        }

        onDownButtonPressed:
        {
            if((rootobjectId.value-rootobjectId.majorstepsize) >= rootobjectId.valuefrom)
            {
                rootobjectId.value = rootobjectId.value - rootobjectId.majorstepsize;
            }
        }

        onLeftButtonPressed:
        {
            if((rootobjectId.value-rootobjectId.stepsize) >= rootobjectId.valuefrom)
            {
                rootobjectId.value = rootobjectId.value - rootobjectId.stepsize;
            }
        }

        onRightButtonPressed:
        {
            if((rootobjectId.value+rootobjectId.stepsize) <= rootobjectId.valueto)
            {
                rootobjectId.value = rootobjectId.value + rootobjectId.stepsize;
            }
        }

        onUpButtonReleased:
        {
            if((rootobjectId.value-rootobjectId.stepsize) >= rootobjectId.valuefrom)
            {
                rootobjectId.value = rootobjectId.value - rootobjectId.stepsize;
            }
        }

        onDownButtonReleased:
        {

        }

        onLeftButtonReleased:
        {

        }

        onRightButtonReleased:
        {

        }
    }

    FontLoader
    {
        id: fontId
        source: rootobjectId.fontsource
    }

    Rectangle
    {
        id: valuebarrectId
        x: rootobjectId.highlightbordersize
        y: rootobjectId.highlightbordersize
        width: (rootobjectId.value/rootobjectId.valueto) * rootobjectId.width
        height: rootobjectId.height - 2*rootobjectId.highlightbordersize
        color: rootobjectId.valuebarcolor
        visible: true
    }

    Rectangle
    {
        id:focusrectId
        x:valuebarrectId.x+valuebarrectId.width
        y:valuebarrectId.y
        width: rootobjectId.width - valuebarrectId.width - rootobjectId.highlightbordersize
        height: valuebarrectId.height
    }

    Text
    {
        id: textId
        font.family: fontId.name
        font.pointSize: rootobjectId.fontsize
        anchors.centerIn: rootobjectId
        //anchors.horizontalCenter: rootobjectId.horizontalCenter
        //y: rootobjectId.height-textId.height-rootobjectId.highlightbordersize
        height: textId.implicitHeight
        width: textId.implicitWidth
        text: rootobjectId.messagetext
        color: rootobjectId.textcolor

    }

    Text
    {
        id:fromtextId
        width: fromtextId.implicitWidth
        height: fromtextId.implicitHeight
        font.family: fontId.name
        font.pixelSize: rootobjectId.fontsize * .9
        x:rootobjectId.highlightbordersize;
        y:rootobjectId.height-fromtextId.height-rootobjectId.highlightbordersize
        text:rootobjectId.valuefrom
        color: rootobjectId.textcolor
    }

    Text
    {
        id: totextId
        width: totextId.implicitWidth
        height: totextId.implicitHeight
        font.family: fontId.name
        font.pixelSize: rootobjectId.fontsize * .9
        x: rootobjectId.width - totextId.width - rootobjectId.highlightbordersize
        y:rootobjectId.height-totextId.height-rootobjectId.highlightbordersize
        text: rootobjectId.valueto
        color: rootobjectId.textcolor
    }
}
