import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"

Item
{
    //Object Properties-------------------------
    id: mainmenupageId
    width: parent.width
    height: parent.height
    anchors.fill:  parent

    visible: true
    focus: true

    //Custom Properties-------------------------
    property string selection: "NONE"

    //Signals-----------------------------------
    signal closePage()
    signal applicationClosing()


    //Functions---------------------------------
    function grabFocus()
    {
        //mainmenupageId.focus = true;
        controllerId.focus = true;
    }

    function quitProgram()
    {
        mainmenupageId.applicationClosing();
        Qt.quit();
    }

    //QML Objects-------------------------------


    //Background Image
    Image
    {
        focus: false
        id: backgroundimageId
        source: "qrc:/Icons/dark_steel_texture.jpg"
        anchors.fill: parent
        opacity: 1
    }

    //Input Handler
    ControllerObject
    {
        focus: false
        id: controllerId

        onGreenButtonPressed:
        {
            mainmenupageId.selection = menulistmodelId.get(menupathviewId.currentIndex).name;

            switch(mainmenupageId.selection)
            {
            case "Quit":
                controlkeyId.visible=false;
                quitdialogId.open();
                //Qt.quit();
                break;
            }
        }

        onRedButtonPressed:
        {
            mainmenupageId.closePage()
        }

        onBlackButtonPressed:
        {
            mainmenupageId.closePage()
        }

        onUpButtonPressed:
        {

        }

        onDownButtonPressed:
        {

        }

        onLeftButtonPressed:
        {
            menupathviewId.decrementCurrentIndex();
        }

        onRightButtonPressed:
        {
            menupathviewId.incrementCurrentIndex();
        }
    }

    //Font for UI
    FontLoader
    {
        id: fontId
        source: "qrc:/Fonts/Typo Draft Demo.otf"
    }

    //Main Menu Text
    Text
    {
        id: titletextId
        focus: false
        font.family: fontId.name
        text: "Main Menu"
        font.pointSize: 40
        width: titletextId.implicitWidth
        height: titletextId.implicitHeight
        x: parent.width/2 - width/2
        y: parent.height/2 - height/2 - 100
        color: Qt.rgba(1,1,.95,1)
    }

    //The Pathview
    PathView
    {
        id: menupathviewId
        focus: false
        anchors.fill: parent
        model: ListModel
        {
            id: menulistmodelId

            ListElement
            {
                iconfile: "qrc:/Icons/save.png"
                name: "Save Profile"
            }
            ListElement
            {
                iconfile: "qrc:/Icons/gear.png"
                name: "System Settings"
            }
            ListElement
            {
                iconfile: "qrc:/Icons/laser3.png"
                name: "Laser Power"
            }
            ListElement
            {
                iconfile: "qrc:/Icons/quit.png"
                name: "Quit"
            }
        }
        delegate: Component
        {
            id: pathviewdelegateId
            Column
            {
                opacity: PathView.opacity
                z: PathView.z
                scale: PathView.scale
                spacing: 5
                Image
                {
                    id: delegateimageId
                    anchors.horizontalCenter: delegatetextId.horizontalCenter
                    source: iconfile; width: 64; height: 64; smooth: true
                    fillMode: Image.PreserveAspectFit
                }
                Text
                {
                    id: delegatetextId
                    font.family: fontId.name
                    text: name; font.pixelSize: 35
                    color: Qt.rgba(1,1,.95,1)
                }
            }

        }
        property int xpathradius: 650
        property int ypathradius: 150
        path: Path
        {
            id: pathviewpathId
            startX: (titletextId.x + (titletextId.implicitWidth/2))
            startY: (titletextId.y + (titletextId.implicitHeight/2)) + menupathviewId.ypathradius + 50
            PathAttribute { name: "opacity"; value: 1.0 }
            PathAttribute { name: "scale"; value: 1.0 }
            PathAttribute { name: "z"; value: 0 }

            //Left
            PathArc{x: (titletextId.x + (titletextId.implicitWidth/2)) - menupathviewId.xpathradius; y: (titletextId.y + (titletextId.implicitHeight/2)); radiusX: menupathviewId.xpathradius; radiusY: menupathviewId.ypathradius}
            PathAttribute { name: "opacity"; value: .3 }
            PathAttribute { name: "scale"; value: .8}
            PathAttribute { name: "z"; value: -1 }

            //Top
            PathArc{x: (titletextId.x + (titletextId.implicitWidth/2)); y: (titletextId.y + (titletextId.implicitHeight/2)) - menupathviewId.ypathradius; radiusX: menupathviewId.xpathradius; radiusY: menupathviewId.ypathradius}
            PathAttribute { name: "opacity"; value: .2 }
            PathAttribute { name: "scale"; value: .4 }
            PathAttribute { name: "z"; value: -2 }

            //Right
            PathArc{x: (titletextId.x + (titletextId.implicitWidth/2)) + menupathviewId.xpathradius; y: (titletextId.y + (titletextId.implicitHeight/2)); radiusX: menupathviewId.xpathradius; radiusY: menupathviewId.ypathradius}
            PathAttribute { name: "opacity"; value: .3}
            PathAttribute { name: "scale"; value: .8 }
            PathAttribute { name: "z"; value: -1 }

            //Bottom
            PathArc{x: pathviewpathId.startX; y: pathviewpathId.startY; radiusX: menupathviewId.xpathradius; radiusY: menupathviewId.ypathradius}
            PathAttribute { name: "opacity"; value: 1.0 }
            PathAttribute { name: "scale"; value: 1.0 }
            PathAttribute { name: "z"; value: 0 }

        }
    }

    ControllerKeyObject
    {
        id: controlkeyId
        greenbuttonmessage: "Select"
        redbuttonmessage: "Back"
        enableblackbutton: false
    }

    YesNoObject
    {
        id: quitdialogId
        message: "Are You Sure You Want to Exit?"

        onSelectionMade:
        {
            switch(selection)
            {
            case "Yes":
                quitProgram();
                break;
            case "No":
                quitdialogId.close();
                break;
            case "Cancel":
                quitdialogId.close();
                break;
            }
        }

        onAboutToOpen:
        {
            controllerId.focus=false;
            menupathviewId.opacity=.1;
            titletextId.opacity=.1;
        }

        onAboutToClose:
        {
            controllerId.focus=true;
            menupathviewId.opacity=1.0;
            titletextId.opacity=1.0;
        }
    }


}

