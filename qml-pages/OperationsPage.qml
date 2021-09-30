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
    property int control_mode: MicroControllerStatusPacket.controlMode

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
        lasertimeouttimerId.stop();
    }

    function triggerTobyNextFrame()
    {
        imagegrabtimerId.restart();
    }

    onControl_modeChanged:
    {
        switch(control_mode)
        {
        case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
            if(MicroControllerStatusPacket.laserStatus)
            {
                if(!lasertimeouttimerId.running) lasertimeouttimerId.restart();

            }
            break;
        case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
            lasertimeouttimerId.stop();
            break;
        case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
            lasertimeouttimerId.stop();
            break;
        }
    }

    Timer
    {
        id: lasertimeouttimerId
        interval: 300000
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered:
        {
            if(MicroControllerStatusPacket.laserStatus)
            {
                lasertimeouttimerId.stop();
                Gary.toggleLaserPower();
            }
        }
    }

    //Slots
    Component.onCompleted:
    {
        Max.processingComplete.connect(mainpageID.triggerTobyNextFrame);
        if(Toby.processingStillImage)
        {
            triggerTobyNextFrame();
        }

        Toby.startCamera();
        Max.emitExtraMats = false;
        lasertimeouttimerId.stop();
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
                Max.attemptToToggleControlState();
                break;
            case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                if(!MicroControllerStatusPacket.laserStatus)
                {
                    Gary.toggleLaserPower();
                    if(!lasertimeouttimerId.running) lasertimeouttimerId.restart();

                }
                else
                {
                    Max.attemptToToggleControlState();
                }

                break;
            case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                Max.attemptToToggleControlState();
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

    //Camera Status
    PylonCameraStatusObject
    {
        id: camerastatusId
        rect_x: viewbackgroundrectId.width+5
        rect_y: 10
    }

    //Motion System Status
    MotionSystemStatusObject
    {
        id:motionsystemstatusId
        rect_x: viewbackgroundrectId.width+5
        rect_y: camerastatusId.rect_height + camerastatusId.rect_y + 5
    }

    //Laser Power Status
    LaserPowerStatusObject
    {
        id: laserpowerstatusId
        rect_x: viewbackgroundrectId.width+5
        rect_y: motionsystemstatusId.rect_height + motionsystemstatusId.rect_y + 5
    }

    //Control Mode
    ControlModeStatusObject
    {
        id: controlmodestatusId
        x: viewbackgroundrectId.width+5
        y: laserpowerstatusId.rect_y + laserpowerstatusId.rect_height + 125
    }

    TrackingStatusObject
    {
        id: trackingstatusId
        trackingtext: "TP " + Max.trackingpoint_x + "," + Max.trackingpoint_y
        showcontrol: Max.validtrackingpoint
        rect_x: viewbackgroundrectId.width+5
        rect_y: controlmodestatusId.y + controlmodestatusId.height + 4
        rect_color: trackingstatusId.mode1color

    }

    TrackingStatusObject
    {
        id: tracktostatusId
        trackingtext: "TTP: " + Max.tracktopoint_x + "," + Max.tracktopoint_y
        showcontrol: Max.validtracktopoint
        rect_x: viewbackgroundrectId.width+5
        rect_y: trackingstatusId.rect_y + trackingstatusId.rect_height + 5
        rect_color: tracktostatusId.mode2color
    }

    //X Axis Motor Status
    MotorStatusObject
    {
        id: xaxisstatusId
        axisname: "X Axis"
        position: MicroControllerStatusPacket.xPosition
        microcontrollermotionstatus: MicroControllerStatusPacket.xMotionStatus
        x: viewbackgroundrectId.width+5
        y: tracktostatusId.rect_height + tracktostatusId.rect_y + 5
    }

    //Z Axis Motor Status
    MotorStatusObject
    {
        id: zaxisstatusId
        axisname: "Z Axis"
        position: MicroControllerStatusPacket.zPosition
        microcontrollermotionstatus: MicroControllerStatusPacket.zMotionStatus
        x: xaxisstatusId.x + xaxisstatusId.width + 5
        y: xaxisstatusId.y
    }

    //PipeLine Status Text
    PipeLineProcessingStatusObject
    {
        id:pipelinestatusId
        status_x:viewbackgroundrectId.width+5
        status_y: 975
        statusstring_x: 10
        statusstring_y: 10
        showstatusstring: true
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
        y: 65
        color: Qt.rgba(1,.1,.1,1)
        opacity: .7
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
        x: 10
        y: camerafpstextId.y + camerafpstextId.height + 5
        color: Qt.rgba(1,.1,.1,1)
        opacity: .7
    }

    //Joint Type Selected
    Text
    {
        id: jointtextId
        text: "Processing Joint Type: " + WeldJointType.jointtypestring
        width: jointtextId.implicitWidth;
        height: jointtextId.implicitHeight;
        color: Qt.rgba(1,.1,.1,1)
        font.family: fontId.name
        font.pointSize: 30
        opacity: .7
        x: (viewbackgroundrectId.x + viewbackgroundrectId.width) - jointtextId.width - 20
        y: (viewbackgroundrectId.y + viewbackgroundrectId.height) - jointtextId.height - 10


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
