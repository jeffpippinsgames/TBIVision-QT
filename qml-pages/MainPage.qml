import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import "qrc:/qml-components"
import tbi.vision.components 1.0

Page
{
    id: mainpageID
    width: parent.width
    height: parent.height
    x:0
    y:0


    Gary
    {
        id: garyId
    }

    background: Rectangle
    {
        anchors.fill: parent
        color: "black"
    }

   /* FocusScope
    {

        Keys.OnPressed:
        {
            /*
            switch(event.key)
            {
            case greenbutton:
                //keypadId.open();
                break;
            case redbutton:
                break;
            case blackbutton:
                //stackview.replace(mainmenupage);
                break;
            case upbutton:
                //garyId.sendJogUp();
                break;
            case downbutton:
                break;
            case leftbutton:
                break;
            case rightbutton:
                break;
            }

        }

        Keys.onReleased:
        {
            switch(event.key)
            {
            case greenbutton:
                break;
            case redbutton:
                break;
            case blackbutton:
                break;
            case upbutton:
                garyId.sendStopMovement();
                break;
            case downbutton:
                garyId.sendStopMovement();
                break;
            case leftbutton:
                garyId.sendStopMovement();
                break;
            case rightbutton:
                garyId.sendStopMovement();
                break;
            }
        }

    }
*/
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
             garyId.sendJogUp();
        }

        onDownButtonPressed:
        {
            garyId.sendJogDown();
        }

        onLeftButtonPressed:
        {
            garyId.sendJogLeft();
        }

        onRightButtonPressed:
        {
            garyId.sendJogRight();
        }

        onUpButtonReleased:
        {
             garyId.sendStopMovement();

        }

        onDownButtonReleased:
        {
            garyId.sendStopMovement();
        }

        onLeftButtonReleased:
        {
            garyId.sendStopMovement();
        }

        onRightButtonReleased:
        {
            garyId.sendStopMovement();
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
