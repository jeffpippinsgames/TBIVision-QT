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
        imagegrabtimerId.restart();
    }

    //Slots
    Component.onCompleted:
    {
        Max.processingComplete.connect(mainpageID.triggerTobyNextFrame);
        Toby.startCamera();
        Max.emitExtraMats = false;
    }

    //Signals
    signal destroyPage(string _transition_page)

    //Required For Every Page
    PagesDescriptionObject
    {
        id:pagesId
    }

    //OML Components
    Timer
    {
        id: imagegrabtimerId
        interval: 1
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered:
        {
            Toby.triggerNextFrame();
        }
    }

    ControllerObject
    {

        id:controllerId
        useAutoRepeatonSticks: false;
        useAutoRepeatonButtons: false;
        //focus: true

        onBlackButtonPressed:
        {
            switch(Gary.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                Max.manualControlModeVGroove();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                cleanupForDestruction();
                destroyPage(pagesId.mainmenupage);
                break;
            }
        }

        onGreenButtonPressed:
        {
            switch(Gary.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Max.fullAutoControlModeVGroove();
                break;
            }
        }

        onRedButtonPressed:
        {
            Max.manualControlModeVGroove();
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

    //Background Rectangle For QmlTBIDisplay
    Rectangle
    {
        id: viewbackgroundrectId
        width:1920-5
        height:1080-5
        x:50
        y: 100
        color: "black"
        border.color: Qt.rgba(1,1,1,.1)
        border.width: 5
        anchors.centerIn: parent


        QmlTBIDisplay
        {
            id: operationviewId
            width: viewbackgroundrectId.width-5
            height:viewbackgroundrectId.height-5
            anchors.centerIn: viewbackgroundrectId

            Component.onCompleted:
            {
                operationviewId.scaleToWidth = operationviewId.width;
                Max.newOperationMatProcessed.connect(operationviewId.recieveCVMat);
            }
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
        font.pointSize: 20
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

    //Control Key Display
    ControllerKeyObject
    {
        id: controlkeyId
        enableredbutton: false
        greenbuttonmessage: "Activate"
        blackbuttonmessage: "Menu"
        buttonspacing: 150
    }

}
