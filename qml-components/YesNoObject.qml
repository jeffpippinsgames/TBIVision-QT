import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2

FocusScope
{
    id: yesnodialogId
    focus: rectId.visible
    anchors.centerIn: parent
    property string message: ""
    property color messagecolor: Qt.rgba(1,1,.95,1)

    signal aboutToOpen()
    signal aboutToClose()
    signal selectionMade(string selection)

    function open()
    {
        yesnodialogId.aboutToOpen();
        rectId.visible=true;
    }

    function close()
    {
        yesnodialogId.aboutToClose();
        rectId.visible=false;
    }

    Rectangle
    {
        id: rectId
        focus: rectId.visible
        visible: false
        width: 500
        height: 300
        anchors.centerIn: parent
        color: "transparent"

        ControllerObject
        {
            id: controllerId
        }

        FontLoader
        {
            id: fontId
            source: "qrc:/Fonts/Typo Draft Demo.otf"
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
}
