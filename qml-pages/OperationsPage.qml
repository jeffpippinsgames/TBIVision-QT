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

    readonly property string pagename: "Operations Page"

    //property string fontsource: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource: "qrc:/Fonts/EurostileBold.ttf"
    //property string fontsource: "qrc:/Fonts/Measurements.ttf"
    //property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string fontsource: "qrc:/Fonts/PermanentMarker-Regular.ttf"

    //Misc Functions-------------------------
    function grabFocus()
    {
        controllerId.focus = true;
    }

    //Dialog and Menu Functions------------------
    function cleanupForDestruction()
    {
        Toby.stopCamera();
        //Kill All Bindings For The Destruction
        timeinlooptextId.text = "";
        //Disconnect All Signals
        Max.newOperationMatProcessed.disconnect(operationviewId.recieveCVMat);
        Max.processingComplete.disconnect(mainpageID.triggerTobyNextFrame);
    }

    function triggerTobyNextFrame()
    {
        Toby.triggerCamera();
    }

    //Slots
    Component.onCompleted:
    {
        Max.processingComplete.connect(mainpageID.triggerTobyNextFrame);
        Toby.startCamera();
    }

    //Signals
    signal destroyPage(string _transition_page)

    //Required For Every Page
    PagesDescriptionObject
    {
        id:pagesId
    }

    //OML Components
    ControllerObject
    {

        id:controllerId
        focus: true

        onBlackButtonPressed:
        {
            cleanupForDestruction();
            destroyPage(pagesId.mainmenupage);
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

    //Background Color
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

    //Camera FPS Text
    Text
    {
        id: camerafpstextId
        visible: Mary.showdebuginfo
        focus: false
        font.family: fontId.name
        text: Toby.camera_fps
        font.pointSize: 15
        width: camerafpstextId.implicitWidth
        height: camerafpstextId.implicitHeight
        x: 300
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
        enableredbutton: false
        greenbuttonmessage: "Activate"
        blackbuttonmessage: "Menu"
        buttonspacing: 150
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

    //Background Rectangle For QmlTBIDisplay
    Rectangle
    {
        id: viewbackgroundrectId
        width:1034
        height:770
        x:50
        y: 100
        color: "black"
        border.color: Qt.rgba(1,1,1,.1)
        border.width: 5


        QmlTBIDisplay
        {
            id: operationviewId
            width: 1024
            height:450
            anchors.centerIn: viewbackgroundrectId

            Component.onCompleted:
            {
                operationviewId.scaleToWidth = operationviewId.width;
                Max.newOperationMatProcessed.connect(operationviewId.recieveCVMat);
            }
        }

    }

}
