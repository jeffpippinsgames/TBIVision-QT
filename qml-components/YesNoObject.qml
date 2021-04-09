import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2

Item
{
    id: yesnodialogId
    focus: true
    anchors.centerIn: parent
    property string messagetext: "Do You Like Welding?"
    property color messagecolor: Qt.rgba(1,1,.95,1)
    property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"

    signal selectionMade(string selection)
    signal completed()
    signal destroyPage()
    signal aboutToDestroy()



    ControllerObject
    {
        id: controllerId
        focus: false

    }

    FontLoader
    {
        id: fontId
        source: yesnodialogId.fontsource
    }

    Text
    {
        id: messageId
        anchors.horizontalCenter: rectId.horizontalCenter
        anchors.bottom: rectId.bottom
        text: message
        color: messagecolor
        font.family: fontId.name
        font.pointSize: 25


    }

    Image
    {
        anchors.left: rectId.left
        anchors.top: rectId.top
        width:150
        height: 150
        id: greenbuttonimageId
        source: "qrc:/Icons/GreenButton2.png"
        opacity: 1.0
    }

    Text
    {
        id: yesId
        anchors.horizontalCenter: greenbuttonimageId.horizontalCenter
        y: greenbuttonimageId.y + greenbuttonimageId.height + 10
        text: "Yes"
        color: messagecolor
        font.family: fontId.name
        font.pointSize: 25
    }

    Image
    {
        anchors.right: rectId.right
        anchors.top: rectId.top
        width:150
        height: 150
        id: redbuttonimageId
        source: "qrc:/Icons/RedButton2.png"
        opacity: 1.0
    }

    Text
    {
        id: noId
        anchors.horizontalCenter: redbuttonimageId.horizontalCenter
        y: redbuttonimageId.y + redbuttonimageId.height + 10
        text: "No"
        color: messagecolor
        font.family: fontId.name
        font.pointSize: 25
    }

    Keys.onPressed:
    {
        console.log("YesNo.qml - Keys.OnPressed Fired.");
        switch(event.key)
        {
        case controllerId.greenbutton:
            yesnodialogId.selectionMade("Yes");
            break;
        case controllerId.redbutton:
            yesnodialogId.selectionMade("No");
            break;
        }
    }

}

