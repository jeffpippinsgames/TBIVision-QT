import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import "qrc:/qml-components"
import "qrc:/qml-pages"
import tbi.vision.components 1.0

Item
{
    //Object Properties-------------------------
    id: mainpageID
    width: parent.width
    height: parent.height
    visible: true
    focus: true
    x:0
    y:0

    //Custom Properties-------------------------
    property var page: null


    //Misc Functions-------------------------
    function grabFocus()
    {
        controllerId.focus = true;
    }

    //Dialog and Menu Functions------------------
    function connectPageSignals()
    {
        if(page !== null)
        {
            switch(page.name)
            {
            case "MainMenu Page":
                page.destroyPage.connect(destroyPage);
                break;
            case "YesNo Dialog":
                page.destroyPage.connect(destroyPage);
                break;
            }
        }
    }

    function createPage(page_qml)
    {
        if(page == null)
        {
            var component = Qt.createComponent(page_qml);
            page = component.createObject(mainpageID);
            if(page !== null)
            {
                page.grabFocus();
                connectPageSignals();
                console.log(page_qml + " created.")
            }
            else
            {
                console.log(page_qml + " created.");
            }

        }
    }

    function destroyPage()
    {
        if(page !== null)
        {
            page.destroy();
            page = null;
            controllerId.focus = true;
            mainpageID.opacity = 1;
        }
    }

    function garyAboutToClose()
    {
        //Kill All Bindings For The Destruction of Gary
        xposId.text = "";
    }

    //Slots
    Component.onCompleted:
    {
        Gary.aboutToDestroy.connect(garyAboutToClose);
    }

    //Signals-----------------------------------


    //OML Components----------------------------
    ControllerObject
    {

        id:controllerId
        focus: true

        onBlackButtonPressed:
        {
            createPage("/qml-pages/MainMenuPage.qml");
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
             Gary.sendJogUp();
        }

        onDownButtonPressed:
        {
            Gary.sendJogDown();
        }

        onLeftButtonPressed:
        {
            Gary.sendJogLeft();
        }

        onRightButtonPressed:
        {
            Gary.sendJogRight();
        }

        onUpButtonReleased:
        {
             Gary.sendStopMovement();
        }

        onDownButtonReleased:
        {
            Gary.sendStopMovement();
        }

        onLeftButtonReleased:
        {
            Gary.sendStopMovement();
        }

        onRightButtonReleased:
        {
            Gary.sendStopMovement();
        }
    }

    Rectangle
    {
        anchors.fill: parent
        color: "black"
    }

    Text
    {
        id: xposId
        focus: false
        font.family: fontId.name
        text: Gary.xPosition
        font.pointSize: 40
        width: titletextId.implicitWidth
        height: titletextId.implicitHeight
        x: 30
        y: 10
        color: Qt.rgba(1,1,.95,1)
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
            keypadId.focus=true;
        }
        onAboutToClose:
        {
            keypadId.focus=false;
            controllerId.focus=true;
        }
        onReturnedvalueChanged:
        {
            console.log("Keypad Returned: " + returnedvalue);
        }
    }

}
