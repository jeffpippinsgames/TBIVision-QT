import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"

Item
{
    id: yesnodialogId
    focus: true
    anchors.fill: parent
    property string messagetext: "Do You Like Welding?"
    property color messagecolor: Qt.rgba(1,1,.95,1)
    property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string backgroundimage: "qrc:/Icons/dark_steel_texture.jpg"
    property string pagename: "YesNo Dialog"
    property int buttoniconsize: 150
    property int buttoniconspacing: 100

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
            yesnodialogId.selectionMade("Yes");
            destroyPage();

        }

        onRedButtonPressed:
        {
            yesnodialogId.selectionMade("No");
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
            pathviewId.decrementCurrentIndex();
        }

        onRightButtonPressed:
        {
            pathviewId.incrementCurrentIndex();
        }
    }

    FontLoader
    {
        id: fontId
        source: yesnodialogId.fontsource
    }

    //Background Image
    Image
    {
        focus: false
        id: backgroundimageId
        source: yesnodialogId.backgroundimage
        anchors.fill: parent
        opacity: 1
    }

    //Message
    Text
    {
        id: messageId
        anchors.horizontalCenter: parent.horizontalCenter
        y: posrectId.y - messageId.implicitHeight
        text: yesnodialogId.messagetext
        color: yesnodialogId.messagecolor
        font.family: fontId.name
        font.pointSize: 25
    }

    //Green Button Image
    Image
    {
        id: greenbuttonimageId
        anchors.left: posrectId.left
        anchors.verticalCenter: posrectId.verticalCenter
        width: yesnodialogId.buttoniconsize
        height: yesnodialogId.buttoniconsize
        source: "qrc:/Icons/GreenButton2.png"
        opacity: 1.0
    }

    //Green Button Text
    Text
    {
        id: yesId
        anchors.horizontalCenter: greenbuttonimageId.horizontalCenter
        y: greenbuttonimageId.y + greenbuttonimageId.height + 10
        text: "Yes"
        color: yesnodialogId.messagecolor
        font.family: fontId.name
        font.pointSize: 25
    }

    //Red Button
    Image
    {
        id: redbuttonimageId
        anchors.right: posrectId.right
        anchors.verticalCenter: posrectId.verticalCenter
        width: yesnodialogId.buttoniconsize
        height: yesnodialogId.buttoniconsize
        source: "qrc:/Icons/RedButton2.png"
        opacity: 1.0
    }

    //Red Button Text
    Text
    {
        id: noId
        anchors.horizontalCenter: redbuttonimageId.horizontalCenter
        y: redbuttonimageId.y + redbuttonimageId.height + 10
        text: "No"
        color: yesnodialogId.messagecolor
        font.family: fontId.name
        font.pointSize: 25
    }

}

