import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import "qrc:/qml-components"
import tbi.vision.components 1.0

Page
{
    id: mainpageID
    width: 300
    height: 300
    x:0
    y:0

    Gary
    {
        id: garyId
        name: "teensy"
    }


    //
    background: Rectangle
    {
        anchors.fill: parent
        color: "black"
    }

    ControllerObject
    {
        focus: true
        id:controllerId

        onBlackButtonPressed:
        {
            stackview.replace(mainmenupage);
        }

        onGreenButtonPressed:
        {
            keypadId.open();
        }

        onRedButtonPressed:
        {

        }

        onUpButtonPressed:
        {
            garyId.jogUp = true;
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

        onUpButtonReleased:
        {
            garyId.jogStop = true;

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
        source: "qrc:/Fonts/Typo Draft Demo.otf"
    }

    //Background Image
    Image
    {
        id: backgroundimageId
        source: "qrc:/Icons/dark_steel_texture.jpg"
        anchors.fill: parent
        opacity: .7
    }

    //Bottom Button Operation Key
    ControllerKeyObject
    {
        greenbuttonmessage: "Auto"
        redbuttonmessage: "Stop"
        blackbuttonmessage: "Main Menu"
    }

    KeypadObject
    {
        id: keypadId
        displayedmessage: "Enter a Value"
        displayedvalue: "23.22"
        onAboutToOpen:
        {
            controllerId.focus=false;
        }
        onAboutToClose:
        {
            controllerId.focus=true;
        }
        onReturnedvalueChanged:
        {
            console.log("Keypad Returned: " + returnedvalue);
        }
    }

}
