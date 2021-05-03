/*
    ElipticalRotateMenu.qml

    A QML Control That Builds a Round Rotating Selection Menu.
    The ListModel Must Be of Type ElipticalRotateMenu-Model

*/
import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"

Item
{
    //Object Properties-------------------------
    id: rootcomponentId
    anchors.fill:parent
    focus: true

    //Custom Properties-------------------------
    property int xpathradius: 650
    property int ypathradius: 150
    property int menucenterx: rootcomponentId.width/2 - titletextId.width/2
    property int menucentery: rootcomponentId.height/2 - titletextId.height/2
    property string menutitle: "Menu"
    property string backgroundimage: "qrc:/Icons/dark_steel_texture.jpg"
    property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property int menuiconsizex: 64
    property int menuiconsizey: 64
    property int menuicontextsize: 35
    property int menutitletextsize: 40
    property color menucolor: Qt.rgba(1,1,.95,1)
    property int icontextspacing: 5
    property real menuopacity: 1
    property real backgroundopacity: 1
    readonly property string closemenustring: "closemenu"

    //Signals----------------------------------
    signal selectionMade(string selection)
    signal completed()
    signal menuItemAdded()
    signal destroyPage()
    signal aboutToDestroy()

    //Functions--------------------------------
    function grabFocus()
    {
        controllerId.focus = true;
    }

    function setBackGroundImage(_source)
    {
        backgroundimage = _source;
    }

    function addMenuItem(_iconfile, _name)
    {
        modelId.append({"iconfile": _iconfile, "name": _name});
        menuItemAdded();
    }

    Component.onCompleted:
    {
        completed();
    }

    //QML Components---------------------------
    ControllerObject
    {
        focus: false
        id: controllerId

        onGreenButtonPressed:
        {
            selectionMade(modelId.get(pathviewId.currentIndex).name);
        }

        onRedButtonPressed:
        {
            selectionMade(rootcomponentId.closemenustring);
        }

        onBlackButtonPressed:
        {

            selectionMade(rootcomponentId.closemenustring);
        }

        onUpButtonPressed:
        {

        }

        onDownButtonPressed:
        {

        }

        onLeftButtonPressed:
        {
            pathviewId.decrementCurrentIndex();
        }

        onRightButtonPressed:
        {
            pathviewId.incrementCurrentIndex();
        }
    }

    //Font for UI
    FontLoader
    {
        id: fontId
        source: rootcomponentId.fontsource
    }

    //Background Image
    Image
    {
        focus: false
        id: backgroundimageId
        source: rootcomponentId.backgroundimage
        anchors.fill: parent
        opacity: rootcomponentId.backgroundopacity
    }

    //Main Menu Text
    Text
    {
        id: titletextId
        focus: false
        font.family: fontId.name
        text: menutitle
        font.pointSize: rootcomponentId.menutitletextsize
        width: titletextId.implicitWidth
        height: titletextId.implicitHeight
        x: rootcomponentId.menucenterx
        y: rootcomponentId.menucentery
        color: rootcomponentId.menucolor
    }

    //Menu ListModel
    ListModel
    {
        id: modelId
    }

    //Menu Delegate
    Component
    {
        id: delegateId
        Column
        {
            opacity: PathView.opacity
            z: PathView.z
            scale: PathView.scale
            spacing: rootcomponentId.icontextspacing
            Image
            {
                id: delegateimageId
                anchors.horizontalCenter: delegatetextId.horizontalCenter
                source: iconfile; width: rootcomponentId.menuiconsizex; height: rootcomponentId.menuiconsizey; smooth: true
                fillMode: Image.PreserveAspectFit
            }
            Text
            {
                id: delegatetextId
                font.family: fontId.name
                text: name; font.pixelSize: rootcomponentId.menuicontextsize
                color: rootcomponentId.menucolor
            }
        }

    }

    //Menu Path
    Path
    {
        id: pathId
        startX: (titletextId.x + (titletextId.implicitWidth/2))
        startY: (titletextId.y + (titletextId.implicitHeight/2)) + rootcomponentId.ypathradius + 50
        PathAttribute { name: "opacity"; value: 1.0 }
        PathAttribute { name: "scale"; value: 1.0 }
        PathAttribute { name: "z"; value: 0 }

        //Left
        PathArc{x: (titletextId.x + (titletextId.implicitWidth/2)) - rootcomponentId.xpathradius; y: (titletextId.y + (titletextId.implicitHeight/2)); radiusX: rootcomponentId.xpathradius; radiusY: rootcomponentId.ypathradius}
        PathAttribute { name: "opacity"; value: .3 }
        PathAttribute { name: "scale"; value: .8}
        PathAttribute { name: "z"; value: -1 }

        //Top
        PathArc{x: (titletextId.x + (titletextId.implicitWidth/2)); y: (titletextId.y + (titletextId.implicitHeight/2)) - rootcomponentId.ypathradius; radiusX: rootcomponentId.xpathradius; radiusY: rootcomponentId.ypathradius}
        PathAttribute { name: "opacity"; value: .2 }
        PathAttribute { name: "scale"; value: .4 }
        PathAttribute { name: "z"; value: -2 }

        //Right
        PathArc{x: (titletextId.x + (titletextId.implicitWidth/2)) + rootcomponentId.xpathradius; y: (titletextId.y + (titletextId.implicitHeight/2)); radiusX: rootcomponentId.xpathradius; radiusY: rootcomponentId.ypathradius}
        PathAttribute { name: "opacity"; value: .3}
        PathAttribute { name: "scale"; value: .8 }
        PathAttribute { name: "z"; value: -1 }

        //Bottom
        PathArc{x: pathId.startX; y: pathId.startY; radiusX: rootcomponentId.xpathradius; radiusY: rootcomponentId.ypathradius}
        PathAttribute { name: "opacity"; value: 1.0 }
        PathAttribute { name: "scale"; value: 1.0 }
        PathAttribute { name: "z"; value: 0 }
    }

    //PathView
    PathView
    {
        id: pathviewId
        focus: false
        anchors.fill: parent
        model: modelId
        delegate: delegateId
        path: pathId
        opacity: rootcomponentId.menuopacity

    }

    //Graphical Key
    ControllerKeyObject
    {
        id: controlkeyId
        greenbuttonmessage: "Select"
        redbuttonmessage: "Back"
        enableblackbutton: false
    }

}
