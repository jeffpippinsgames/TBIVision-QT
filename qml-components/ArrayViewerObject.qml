import QtQuick 2.0
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import "qrc:/qml-components"
import "qrc:/qml-pages"
import tbi.vision.components 1.0

Rectangle {
    id: rootcomponentId
    width: 750
    height: 120
    color: "transparent"
    border.color: nonhighlightcolor
    border.width: highlightbordersize

    property string controlname: "control_name"

    property string title_text: "Array Viewer"

    //property string fontsource: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource: "qrc:/Fonts/EurostileBold.ttf"
    //property string fontsource: "qrc:/Fonts/Measurements.ttf"
    property string fontsource: "qrc:/Fonts/PermanentMarker-Regular.ttf"

    readonly property string nothighlightedstate: "NotHighlightedState"
    readonly property string highlightedstate: "HighlightedState"
    readonly property string focusedstate: "FocusedState"

    readonly property color focuscolor: Qt.rgba(0,1,0,.2)
    readonly property color nonfocuscolor: "transparent"
    readonly property color highlightcolor: Qt.rgba(0,1,0,.6)
    readonly property color nonhighlightcolor: Qt.rgba(1,1,1,.1)
    readonly property int highlightbordersize: 5

    property bool controlbuttonautorepeat: false
    property bool controlstickautorepeat: false


    property int selected_index
    property real selected_value

    signal endFocus()

    states:
    [
        State
        {
            name: nothighlightedstate
            PropertyChanges{ target: rootcomponentId; border.color: nonhighlightcolor}
            PropertyChanges{ target: rootcomponentId; color: nonfocuscolor}
        },
        State
        {
            name: highlightedstate
            PropertyChanges{ target: rootcomponentId; border.color: highlightcolor}
            PropertyChanges{ target: rootcomponentId; color: nonfocuscolor}
        },
        State
        {
            name: focusedstate
            PropertyChanges{ target: rootcomponentId; border.color: highlightcolor}
            PropertyChanges{ target: rootcomponentId; color: focuscolor}
        }

    ]

    function addListElement(_value, _index)
    {
        modelId.append({"value": _value, "index": _index});
    }

    function grabFocus()
    {
        controllerId.focus = true;
        rootcomponentId.state = focusedstate;
    }

    function listModelSize()
    {
        return modelId.count;
    }

    function updateElementValue(_value, _index)
    {
        modelId.setProperty(_index, "value", _value);
    }

    ControllerObject
    {

        id:controllerId
        focus: false

        useAutoRepeatonButtons: rootcomponentId.controlbuttonautorepeat
        useAutoRepeatonSticks: rootcomponentId.controlstickautorepeat

        onBlackButtonPressed:
        {
            rootcomponentId.state = highlightedstate;
            rootcomponentId.endFocus();
        }

        onGreenButtonPressed:
        {
            rootcomponentId.state = highlightedstate;
            rootcomponentId.endFocus();
        }

        onRedButtonPressed:
        {
            rootcomponentId.state = highlightedstate;
            rootcomponentId.endFocus();
        }

        onUpButtonPressed:
        {

        }

        onDownButtonPressed:
        {

        }

        onLeftButtonPressed:
        {
            listviewId.decrementCurrentIndex();
        }

        onRightButtonPressed:
        {
            listviewId.incrementCurrentIndex();
        }

        onUpButtonReleased:
        {

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

    //FontLoader
    FontLoader
    {
        id: fontId
        source: rootcomponentId.fontsource
    }

    Text
    {
        id: titletextId
        width: titletextId.implicitWidth
        height: titletextId.implicitHeight
        anchors.horizontalCenter: rootcomponentId.horizontalCenter
        y: rootcomponentId.height - rootcomponentId.highlightbordersize - titletextId.height - 5
        font.family: fontId.name
        text: title_text
        font.pixelSize: 20
        color: rootpageId.textcolor
    }

    //ListView Model
    ListModel
    {
        id: modelId
        //Elements are value and index
    }

    //Menu Delegate
    Component
    {
        id: delegateId
        Column
        {
            spacing: 10
            Rectangle
            {
                id: delegaterectId
                width: 120
                height: 30
                color: "transparent"
                border.color: "yellow"
                border.width: 2
                Text
                {
                    id: delegatecolumnId
                    anchors.centerIn: delegaterectId
                    font.family: fontId.name
                    text: "Index " + index
                    font.pixelSize: 20
                    color: rootpageId.textcolor
                }
            }

            Text
            {
                id: delegatevalueId
                anchors.horizontalCenter: delegaterectId.horizontalCenter
                height: delegatevalueId.implicitHeight
                width: delegatevalueId.implicitHeight
                font.family: fontId.name
                text: value.toFixed(1)
                font.pixelSize: 20
                color: rootpageId.textcolor
            }
        }
    }

    //ListView
    ListView
    {
        id: listviewId
        anchors.centerIn: rootcomponentId
        width: rootcomponentId.width - 10
        height: rootcomponentId.height - 10
        model: modelId
        delegate: delegateId
        orientation: Qt.Horizontal
        layoutDirection: Qt.LeftToRight
        highlight: Rectangle{id: elementselecthighlightrectId; color: "red"; radius: 5; opacity: .4}
        clip: true
        spacing: 10

    }
}
