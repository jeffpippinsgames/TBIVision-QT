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




    //property string fontsource: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource: "qrc:/Fonts/EurostileBold.ttf"
    //property string fontsource: "qrc:/Fonts/Measurements.ttf"
    //property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string fontsource: "qrc:/Fonts/PermanentMarker-Regular.ttf"

    //Custom Properties-------------------------
    property var page: null

    //Misc Functions-------------------------
    function grabFocus()
    {
        controllerId.focus = true;
    }

    //Dialog and Menu Functions------------------

    function createPage(page_qml)
    {
        if(page == null)
        {
            var component = Qt.createComponent(page_qml);
            page = component.createObject(mainpageID);
            if(page !== null)
            {
                page.destroyPage.connect(destroyPage);
                page.grabFocus();
            }
            else
            {
                console.log("QML: " + page_qml + " Could Not Be Created.");
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
            triggerTobyNextFrame();
        }
    }

    function garyAboutToDestory()
    {
        //Kill All Bindings For The Destruction of Gary
        xposId.text = "";
    }

    function maryAboutToDestroy()
    {
        timeinlooptextId.visible = false;
        timeinlooptextId.text = "";
    }


    function triggerTobyNextFrame()
    {
        if(page == null) Toby.triggerCamera();
    }

    //Slots
    Component.onCompleted:
    {
        //Connect the Application Singletons to the QML Objects.
        Max.processingComplete.connect(mainpageID.triggerTobyNextFrame);
        Gary.aboutToDestroy.connect(mainpageID.garyAboutToDestory);
        Mary.aboutToDestroy.connect(mainpageID.maryAboutToDestroy);
        Toby.startCamera();
        Mary.loadMaryFromFile();
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

    //Font for UI
    FontLoader
    {
        id: fontId
        source: fontsource
    }

    //Background Image
    Image
    {
        id: backgroundimageId
        source: "qrc:/Icons/dark_steel_texture.jpg"
        anchors.fill: parent
        opacity: .7
    }

    //QImage Viewer Component - Main Display Screen
    QmlTBIDisplay
    {
        id: mainscreenId
        visible: true
        x:0
        y:0
        scaleToWidth: 720
        anchors.fill: parent
        opacity: 1

        Component.onCompleted:
        {
            //Max.newOperationMatProcessed.connect(mainscreenId.recieveCVMat);
        }
    }

    //Time in Loop Text
    Text
    {
        id: timeinlooptextId
        visible: Mary.showdebuginfo
        focus: false
        font.family: fontId.name
        text: Max.timeinloop
        font.pointSize: 15
        width: timeinlooptextId.implicitWidth
        height: timeinlooptextId.implicitHeight
        x: 30
        y: 10
        color: Qt.rgba(1,1,.95,1)
    }

    //Gary Notification Icon
    IconInfoStatusObject
    {
        id:garystatusId
        imagesource: "qrc:/Icons/microcontroller(white).png"
        width:150
        height:150
        x:mainpageID.width-garystatusId.width-50
        y:50

        function garyChangedOperationStatus(status)
        {
            switch(status)
            {
            case GaryOperationStatus.TBI_OPERATION_OK:
                garystatusId.glowcolor = Qt.rgba(0,1,0,1);
                garystatusId.messagetext = "Motion Controller O.K.";
                break;
             case GaryOperationStatus.TBI_OPERATION_ERROR:
                 garystatusId.glowcolor = Qt.rgba(1,0,0,1);
                 garystatusId.messagetext = "Error in Motion Controller";
            }
        }

        Component.onCompleted:
        {
            Gary.operationStatusChanged.connect(garystatusId.garyChangedOperationStatus);
        }
    }

    //Control Key Display
    ControllerKeyObject
    {
        id: controlkeyId
        greenbuttonmessage: "Selection"
        redbuttonmessage: "Leave Selection"
        blackbuttonmessage: "Main Menu"
    }

    //Testing Keypad Object
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
