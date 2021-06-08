import QtQuick 2.0
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import "qrc:/qml-components"
import "qrc:/qml-pages"
import tbi.vision.components 1.0

Rectangle
{
    id: rootdialogId
    anchors.fill: parent
    color: "transparent"
    visible: false

    //property string fontsource: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource: "qrc:/Fonts/EurostileBold.ttf"
    //property string fontsource: "qrc:/Fonts/Measurements.ttf"
    //property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string fontsource: "qrc:/Fonts/PermanentMarker-Regular.ttf"
    property color textcolor: Qt.rgba(1,1,.95,1)
    property string filename: "null"

    //Misc Functions-------------------------------------------------

    //grabFocus function
    function grabFocus()
    {
        controllerId.focus = true;
    }

    function nullFileName()
    {
        rootdialogId.filename = "null";
    }

    onVisibleChanged:
    {
        Toby.updateStillImageFileList();
    }

    //OML Components------------------------------------------------

   //Controller Object
    ControllerObject
    {

        id:controllerId
        focus: false

        onBlackButtonPressed:
        {
            rootdialogId.visible = false;
            rootdialogId.filename = "null";
        }

        onGreenButtonPressed:
        {
            rootdialogId.filename   = Toby.stillImageListViewModel[filelistviewId.currentIndex];
            if(rootdialogId.filename != "null")
            {
            Toby.openStillImagetoProcess(rootdialogId.filename);
            }
            rootdialogId.visible = false;

        }

        onRedButtonPressed:
        {
            rootdialogId.visible = false;
            rootdialogId.filename = "null";
        }

        onUpButtonPressed:
        {
            filelistviewId.decrementCurrentIndex();
        }

        onDownButtonPressed:
        {
            filelistviewId.incrementCurrentIndex();
        }

        onLeftButtonPressed:
        {

        }

        onRightButtonPressed:
        {

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

    //Font for UI
    FontLoader
    {
        id: fontId
        source: rootdialogId.fontsource
    }

    //Background Fill
    Rectangle
    {
        id: backgroundrectId
        anchors.fill: rootdialogId
        color:"black"
        opacity: .95
    }

    //Dialog Title
    Text
    {
        id: titletextId
        visible: true
        focus: false
        font.family: fontId.name
        text: "Still Image File Selection"
        font.pointSize: 25
        width: titletextId.implicitWidth
        height: titletextId.implicitHeight
        x: 30
        y: 10
        color: rootdialogId.textcolor
    }

    //Preview Image
    Image
    {
        id: previewimageId
        width: 720
        height: 540
        x: 10
        y: rootdialogId.height/2 - previewimageId.height/2
    }

    //ListView
    ListView
    {
       id: filelistviewId
       x:previewimageId.x+previewimageId.width + 20
       y: 20
       width: rootdialogId.width-filelistviewId.x-20
       height: rootdialogId.height - 2* filelistviewId.y
       model: Toby.stillImageListViewModel
       spacing: 10
       highlight: Rectangle{id: filehighlightrectId; color: "green"; radius: 5; opacity: .2}

       delegate: Rectangle
       {
           height: 25
           width: filelistviewId.width-10
           color: "transparent"
           Text
           {
               id: delegatetextId
               text: modelData
               font.family: fontId.name
               font.pointSize: 15
               anchors.centerIn: parent
               color: rootdialogId.textcolor
           }
       }


       onCurrentIndexChanged:
       {
           previewimageId.source = "file:///" + Toby.currentApplicationPath + "/stills/" + Toby.stillImageListViewModel[filelistviewId.currentIndex];
       }
    }


}
