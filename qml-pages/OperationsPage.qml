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

    //property string fontsource2: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource2: "qrc:/Fonts/EurostileBold.ttf"
    //property string fontsource2: "qrc:/Fonts/Measurements.ttf"
    property string fontsource2: "qrc:/Fonts/Typo Draft Demo.otf"
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
        MicroControllerStatusPacket.controlModeChanged.disconnect(controlmodeselectedtextId.setControlModeText);
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

            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:      
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                cleanupForDestruction();
                destroyPage(pagesId.mainmenupage);
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                break;
            }

        }

        onGreenButtonPressed:
        {

            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Max.attemptToToggleVGrooveControlState();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                break;
            }
        }

        onRedButtonPressed:
        {
            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                Gary.setControlModeToManual();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                Gary.setControlModeToManual();
                break;
            }

        }

        onUpButtonPressed:
        {
            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Gary.sendJogUp();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                break;
            }
        }

        onDownButtonPressed:
        {
            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Gary.sendJogDown();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                break;
            }
        }

        onLeftButtonPressed:
        {
            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Gary.sendJogLeft();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                Gary.sendJogLeft();
                break;
            }

        }

        onRightButtonPressed:
        {
            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Gary.sendJogRight();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                Gary.sendJogRight();
                break;
            }

        }

        onUpButtonReleased:
        {
            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Gary.sendStopMovement();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                break;
            }

        }

        onDownButtonReleased:
        {
            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Gary.sendStopMovement();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                break;
            }

        }

        onLeftButtonReleased:
        {
            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Gary.sendStopMovement();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                Gary.sendStopMovement();
                break;
            }
        }

        onRightButtonReleased:
        {
            switch(MicroControllerStatusPacket.controlMode)
            {
            case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                Gary.sendStopMovement();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                Gary.sendStopMovement();
                break;
            }
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

    //Font for UI
    FontLoader
    {
        id: font2Id
        source: fontsource2
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
        width:1440+20
        height:1080
        x:0
        y: 0
        color: "black"
        border.color: Qt.rgba(1,1,1,.1)
        border.width: 5
        //anchors.centerIn: parent


        QmlTBIDisplay
        {
            id: operationviewId
            width: viewbackgroundrectId.width-20
            height:viewbackgroundrectId.height
            anchors.centerIn: viewbackgroundrectId

            Component.onCompleted:
            {
                operationviewId.scaleToWidth = operationviewId.width;
                Max.newOperationMatProcessed.connect(operationviewId.recieveCVMat);
            }
        }

    }

    Rectangle
    {
        id: controlmoderectId
        color: Qt.rgba(0,0,0,.8)
        radius: 10
        border.width: 3
        border.color: Qt.rgba(.8, .8, .0, .7)
        x:viewbackgroundrectId.width+5
        y:10
        width: 450
        height: 100

        Text
        {
            id: controlmodetextId
            focus: false
            font.family: font2Id.name
            text: "Selected Control Mode"
            font.pointSize: 20
            width: controlmodetextId.implicitWidth
            height: controlmodetextId.implicitHeight
            anchors.horizontalCenter: parent.horizontalCenter
            y: 5
            color: Qt.rgba(.7,.2,.2,1)
        }

        Text
        {
            id: controlmodeselectedtextId
            focus: false
            font.family: font2Id.name
            text: "Manual"
            font.pointSize: 30
            width: controlmodeselectedtextId.implicitWidth
            height: controlmodeselectedtextId.implicitHeight
            anchors.horizontalCenter: parent.horizontalCenter
            y: controlmodetextId.y + controlmodetextId.height+5
            color: Qt.rgba(.2,.8,.2,1)

            function setControlModeText()
            {
                switch(MicroControllerStatusPacket.controlMode)
                {
                case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                    controlmodeselectedtextId.text = "Manual";
                    controlmodeselectedtextId.color = Qt.rgba(.8,0,.0,1);
                    break;
                case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                    controlmodeselectedtextId.text = "Full";
                    controlmodeselectedtextId.color = Qt.rgba(0,.8,0,1);
                    break;
                case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                    controlmodeselectedtextId.text = "Height";
                    controlmodeselectedtextId.color = Qt.rgba(.8,.8,0,1);
                    break;
                case GaryControlMode.TBI_CONTROL_MODE_MOTORCALIBRATION:
                    controlmodeselectedtextId.text = "Calibration";
                    controlmodeselectedtextId.color = Qt.rgba(0,0,.8,1);
                    break;
                }
            }

            Component.onCompleted:
            {
                controlmodeselectedtextId.setControlModeText();
                MicroControllerStatusPacket.controlModeChanged.connect(controlmodeselectedtextId.setControlModeText);
            }
        }
    }

    //Camera FPS Text
    Text
    {
        id: camerafpstextId
        visible: true
        focus: false
        font.family: fontId.name
        text: Toby.camera_fps
        font.pointSize: 15
        width: camerafpstextId.implicitWidth
        height: camerafpstextId.implicitHeight
        x: 10
        y: 10
        color: Qt.rgba(1,.1,.1,1)
    }

    //Time in Loop Text
    Text
    {
        id: timeinlooptextId
        visible: true
        focus: false
        font.family: fontId.name
        text: Max.timeinloop
        font.pointSize: 15
        width: timeinlooptextId.implicitWidth
        height: timeinlooptextId.implicitHeight
        x: camerafpstextId.width+30
        y: 10
        color: Qt.rgba(1,.1,.1,1)
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
