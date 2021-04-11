import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"

Item
{
    id: twobuttonoptiondialogId
    focus: true
    anchors.fill: parent
    property string messagetext: "Are You Sure?"
    property color messagecolor: Qt.rgba(1,1,.95,1)
    property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string backgroundimage: "qrc:/Icons/dark_steel_texture.jpg"
    property string pagename: "TwoButtonOption Dialog"
    property int buttoniconsize: 150
    property int buttoniconspacing: 100
    property string icon1src: value
    property string icon2src: value
    property string option1: "Yes"
    property string option2: "No"

    signal selectionMade(string selection)
    signal completed()
    signal destroyPage()
    signal aboutToDestroy()

    function grabFocus()
    {
        controllerId.focus = true;
    }

    Rectangle
    {
       id: posrectId
       anchors.centerIn: parent
       width: buttoniconsize*2 + buttoniconspacing
       height: buttoniconsize + buttoniconspacing/4
       color: "transparent"
    }

    ControllerObject
    {
        id: controllerId
        focus: false

        onGreenButtonPressed:
        {
            yesnodialogId.selectionMade(twobuttonoptiondialogIdq.option1);
            destroyPage();

        }

        onRedButtonPressed:
        {
            yesnodialogId.selectionMade(twobuttonoptiondialogId.option2);
            destroyPage();
        }

        onBlackButtonPressed:
        {

        }

        onUpButtonPressed:
        {

        }

        onDownButtonPressed:
        {

        }

        onLeftButtonPressed:
        {

        }

        onRightButtonPressed:
        {

        }
    }

    FontLoader
    {
        id: fontId
        source: twobuttonoptiondialogId.fontsource
    }

    //Background Image
    Image
    {
        focus: false
        id: backgroundimageId
        source: twobuttonoptiondialogId.backgroundimage
        anchors.fill: parent
        opacity: 1
    }

    //Message
    Text
    {
        id: messageId
        anchors.horizontalCenter: parent.horizontalCenter
        y: posrectId.y - messageId.implicitHeight
        text: twobuttonoptiondialogId.messagetext
        color: twobuttonoptiondialogId.messagecolor
        font.family: fontId.name
        font.pointSize: 25
    }

    //Option1 Icon
    Image
    {
        id: option1imageId
        anchors.left: posrectId.left
        anchors.verticalCenter: posrectId.verticalCenter
        width: twobuttonoptiondialogId.buttoniconsize
        height: twobuttonoptiondialogId.buttoniconsize
        source: "qrc:/Icons/GreenButton2.png"
        opacity: 1.0
    }

    //Option1 Text
    Text
    {
        id: option1textId
        anchors.horizontalCenter: greenbuttonimageId.horizontalCenter
        y: greenbuttonimageId.y + greenbuttonimageId.height + 10
        text: twobuttonoptiondialogId.option1
        color: twobuttonoptiondialogId.messagecolor
        font.family: fontId.name
        font.pointSize: 25
    }

    //Option2 Icon
    Image
    {
        id: option2imageId
        anchors.right: posrectId.right
        anchors.verticalCenter: posrectId.verticalCenter
        width: twobuttonoptiondialogId.buttoniconsize
        height: twobuttonoptiondialogId.buttoniconsize
        source: "qrc:/Icons/RedButton2.png"
        opacity: 1.0
    }

    //Option2 Text
    Text
    {
        id: option2textId
        anchors.horizontalCenter: redbuttonimageId.horizontalCenter
        y: redbuttonimageId.y + redbuttonimageId.height + 10
        text: twobuttonoptiondialogId.option2
        color: twobuttonoptiondialogId.messagecolor
        font.family: fontId.name
        font.pointSize: 25
    }

}

