import QtQuick 2.0
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import "qrc:/qml-components"
import "qrc:/qml-pages"
import tbi.vision.components 1.0

Item {

    id: rootpageId
    anchors.fill: parent
    visible: true

    //Custom Properties----------------------------------------------
    readonly property string pagename: "ImageProcessingSettings Page"

    property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string backgroundimage: "qrc:/Icons/dark_steel_texture.jpg"
    property real backgroundopacity: .6
    property real controlsopacity: 1
    property color textcolor: Qt.rgba(1,1,.95,1)

    //State Constants
    readonly property string camerastateframeactive: "CameraStateFrameActive"
    readonly property string camerastateframenotactive: "CameraStateFrameNotActive"
    readonly property string blurstateframeactive: "BlurStateFrameActive"
    readonly property string blurstateframenotactive: "BlurStateFrameNotActive"
    readonly property string thresholdstateframeactive: "ThresholdStateFrameActive"
    readonly property string thresholdstateframenotactive: "ThresholdStateFrameNotActive"
    readonly property string pixelcolumnstateframenotactive: "PixelColumnStateFrameNotActive"
    readonly property string pixelcolumnstateframeactive: "PixelColumnStateFrameActive"

    //Frame Constants. For Display Switching and Signal Connections to Max
    readonly property string emptyframe: "NoFrame"
    readonly property string rawframe: "Camera"
    readonly property string blurframe: "Blur"
    readonly property string thresholdframe: "Threshold"
    readonly property string pixelcolumnframe: "PixelColumn"

    //For Rectangle Placements
    //The MainView Rect Derives From FrameSelect and Settings
    readonly property color focuscolor: Qt.rgba(0,1,0,.6)
    readonly property color nonfocuscolor: Qt.rgba(1,1,1,.1)
    readonly property int rectborderwidths: 5
    readonly property int rectpadding: 20

    readonly property int frameselectrectx: 20
    readonly property int frameselectrecty: 60
    readonly property int frameselectrectheight: 200
    readonly property int frameselectrectwidth: rootpageId.width-2*rootpageId.rectpadding

    readonly property int settingsrectheight: 730
    readonly property int settingsrectwidth: 800
    readonly property int settingsrectx: rootpageId.width - rootpageId.settingsrectwidth - rootpageId.rectpadding
    readonly property int settingsrecty: rootpageId.frameselectrecty+rootpageId.frameselectrectheight+rootpageId.rectpadding

    //Signals--------------------------------------------------------
    signal destroyPage()
    signal aboutToDestroy()
    signal completed()

    //Misc Functions-------------------------------------------------
    function grabFocus()
    {
        frameselectcontrollerId.focus = true;
    }

    function signalDestroyPage()
    {

        Mary.updateCameraAOIToMarysSettings();
        Mary.saveMaryToFile();
        aboutToDestroy();
        destroyPage();
    }

    function triggerTobyNextFrame()
    {
        Toby.triggerCamera();
    }

    //Slots---------------------------------------------------------
    Component.onCompleted:
    {
        //Adjust Camera AOI to Max
        Toby.setCameraAOIToMax();
        Max.processingComplete.connect(rootpageId.triggerTobyNextFrame);
        //Connect the Application Singletons to the QML Objects.
        rootpageId.state = rootpageId.camerastateframeactive;
        rootpageId.completed();
        console.log("QML: ImageProcessingSettingPage Created.");
    }

    //Page States---------------------------------------------------
    states:
        [
        State //Camera Selected Frame Focused
        {
            name: rootpageId.camerastateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: true;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: camerasettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.rawframe}
            //Set The Interior Control State
            PropertyChanges {target: camerasettingsrectId; state: "NonFocused";}

        },
        State //Camera Selected and Focused
        {
            name: rootpageId.camerastateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: true;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: camerasettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSelect
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.rawframe}
            //Set The Interior Control State
            PropertyChanges {target: camerasettingsrectId; state: "CameraAOIWidthHeightState";}
        },
        State //Blur Selected Frame Focused
        {
            name: rootpageId.blurstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: true;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: blursettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.blurframe}
        },
        State //Blur Selected and Focused
        {
            name: rootpageId.blurstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: true;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: blursettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: blursettingscontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.blurframe}
            //Set The Interior Control State
            PropertyChanges {target: blursettingsrectId; state: "BlurState";}
        },
        State //Threshold Selected Frame Focused
        {
            name: rootpageId.thresholdstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: true;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: thresholdsettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.thresholdframe}
        },
        State //Threshold Selected and Focused
        {
            name: rootpageId.thresholdstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: true;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: thresholdsettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.thresholdframe}
            //Set The Interior Control State
            PropertyChanges {target: thresholdsettingsrectId; state: "MinState";}
        },
        State //Pixel Column Selected Frame Focused
        {
            name: rootpageId.pixelcolumnstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: true;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: pixelcolumnborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.pixelcolumnframe}
        },
        State //Pixel Column Selected and Focused
        {
            name: rootpageId.pixelcolumnstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: true;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: pixelcolumnborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.pixelcolumnframe}
            //Set The Interior Control State
            PropertyChanges {target: thresholdsettingsrectId; state: "MinState";}
        }

    ]

    //OML Components------------------------------------------------
    //Font for UI
    FontLoader
    {
        id: fontId
        source: rootpageId.fontsource
    }

    //Background Solid----------------------------------------------
    Rectangle
    {
        anchors.fill: rootpageId
        color: "black"
    }

    //Background Image----------------------------------------------
    Image
    {
        focus: false
        id: backgroundimageId
        source: rootpageId.backgroundimage
        anchors.fill: parent
        opacity: rootpageId.backgroundopacity
    }

    //Top Text------------------------------------------------------
    Text
    {
        id: toptextId
        font.family: fontId.name
        y: 20
        x:25
        text: "Pipeline Selection"
        font.pixelSize: 30
        color: rootpageId.textcolor
    }

    //Right Text----------------------------------------------------
    Text
    {
        id: righttextId
        font.family: fontId.name
        y: 20
        x:frameselectionrectId.x+frameselectionrectId.width-righttextId.implicitWidth
        text: "Image Processing Settings"
        font.pixelSize: 30
        color: rootpageId.textcolor
    }

    //Frame Selection Rectangle-------------------------------------
    Rectangle
    {
        id: frameselectionrectId
        x:rootpageId.frameselectrectx
        y:rootpageId.frameselectrecty
        height:rootpageId.frameselectrectheight
        width: rootpageId.frameselectrectwidth
        color: "transparent"


        ControllerObject
        {

            id:frameselectcontrollerId
            focus: false

            onBlackButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onGreenButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onRedButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onUpButtonPressed:
            {

            }

            onDownButtonPressed:
            {
                switch(mainviewrectId.displayed_frame)
                {
                case rootpageId.rawframe:
                    rootpageId.state = rootpageId.camerastateframenotactive;
                    break;
                case rootpageId.blurframe:
                    rootpageId.state = rootpageId.blurstateframenotactive;
                    break;
                case rootpageId.thresholdframe:
                    rootpageId.state = rootpageId.thresholdstateframenotactive;
                    break;
                case rootpageId.pixelcolumnframe:
                    rootpageId.state = rootpageId.pixelcolumnstateframenotactive;
                    break;
                }
            }

            onLeftButtonPressed:
            {
                framelistviewId.decrementCurrentIndex();
            }

            onRightButtonPressed:
            {
                framelistviewId.incrementCurrentIndex();
            }

            onUpButtonReleased:
            {

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

        Rectangle
        {
            anchors.fill: frameselectionrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: frameselectionrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: frameselectborderrectId
            anchors.fill: frameselectionrectId
            color: "transparent"
            border.width: rootpageId.rectborderwidths
            border.color: rootpageId.nonfocuscolor
            radius: 10
        }

        //ListModel
        ListModel
        {
            id: modelId

            /*
            //ListElemet Settings Cannot Use Properties to
            //Set the frame elements.
            ListElement
            {
                frame: "Camera"
            }

            ListElement
            {
                frame: "Blur"
            }

            ListElement
            {
                frame: "Threshold"
            }
            */
        }

        //Menu Delegate
        Component
        {
            id: delegateId
            Column
            {
                spacing: 10
                QmlTBIDisplay
                {
                    id: delegateframeId
                    width:200
                    height:150
                    scaleToWidth: 200
                    //Handle Max Signal/Slot Connections
                    Component.onCompleted:
                    {
                        switch(frame)
                        {
                        case rootpageId.rawframe:
                            Max.newRawMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.blurframe:
                            Max.newBlurMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.thresholdframe:
                            Max.newThresholdMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.pixelcolumnframe:
                            Max.newPixelColumnMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        }
                    }
                }
                Text
                {
                    id: delegatetextId
                    anchors.horizontalCenter: delegateframeId.horizontalCenter
                    font.family: fontId.name
                    text: frame
                    font.pixelSize: 20
                    color: rootpageId.textcolor
                }
            }
        }

        ListView
        {
            id: framelistviewId
            anchors.centerIn: frameselectionrectId
            height: frameselectionrectId.height - 10
            width: frameselectionrectId.width - 10
            model: modelId
            delegate: delegateId
            highlight: Rectangle{id: frameselecthighlightrectId; color: "green"; radius: 5; opacity: .4}
            orientation: Qt.Horizontal
            layoutDirection: Qt.LeftToRight
            spacing: 25
            visible: true

            Component.onCompleted:
            {
                //Add Items to model
                //This Way Will Help with using properties in the frame setting for Listmodel.
                modelId.append({"frame": rootpageId.rawframe});
                modelId.append({"frame": rootpageId.blurframe});
                modelId.append({"frame": rootpageId.thresholdframe});
                modelId.append({"frame": rootpageId.pixelcolumnframe});

            }

            onCurrentIndexChanged:
            {

                switch(modelId.get(framelistviewId.currentIndex).frame)
                {
                case rootpageId.rawframe:
                    rootpageId.state = rootpageId.camerastateframeactive;
                    break;
                case rootpageId.blurframe:
                    rootpageId.state = rootpageId.blurstateframeactive;
                    break;
                case rootpageId.thresholdframe:
                    rootpageId.state = rootpageId.thresholdstateframeactive;
                    break;
                case rootpageId.pixelcolumnframe:
                    rootpageId.state = rootpageId.pixelcolumnstateframeactive;
                    break;
                }
            }

        }
    }

    //Camera Settings Rectangle-------------------------------------
    Rectangle
    {
        id: camerasettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            camerasettingscontrollerId.focus = true;
        }

        onVisibleChanged:
        {
            if(camerasettingsrectId.visible)
            {
                cameradeviceinfotextId.text = Toby.getCameraInfo();
            }
            else
            {
                camerasettingsrectId.state = "NonFocused";
            }
        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: cameraAOIadjustemtId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: exposuresliderId; state: exposuresliderId.nothighlightedstate;}
                PropertyChanges{target: gainsliderId; state: gainsliderId.nothighlightedstate;}

            },
            State
            {
                name: "CameraAOIWidthHeightState"
                PropertyChanges{target: cameraAOIadjustemtId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: exposuresliderId; state: exposuresliderId.nothighlightedstate;}
                PropertyChanges{target: gainsliderId; state: gainsliderId.nothighlightedstate;}

            },
            State
            {
                name: "CameraExposureState"
                PropertyChanges{target: cameraAOIadjustemtId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: exposuresliderId; state: exposuresliderId.highlightedstate;}
                PropertyChanges{target: gainsliderId; state: gainsliderId.nothighlightedstate;}

            },
            State
            {
                name: "CameraGainState"
                PropertyChanges{target: cameraAOIadjustemtId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: exposuresliderId; state: exposuresliderId.nothighlightedstate;}
                PropertyChanges{target: gainsliderId; state: gainsliderId.highlightedstate;}

            }

        ]

        ControllerObject
        {

            id:camerasettingscontrollerId
            focus: false

            onFocusChanged:
            {

            }

            onBlackButtonPressed:
            {

                rootpageId.signalDestroyPage();
            }

            onGreenButtonPressed:
            {
                switch(camerasettingsrectId.state)
                {
                case "CameraAOIWidthHeightState":
                    cameraAOIadjustemtId.grabFocus();
                    break;
                case "CameraExposureState":
                    exposuresliderId.grabFocus();
                    break;
                case "CameraGainState":
                    gainsliderId.grabFocus();
                    break;

                }
            }

            onRedButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onUpButtonPressed:
            {
                switch(camerasettingsrectId.state)
                {
                case "CameraAOIWidthHeightState":
                    camerasettingsrectId.state = "NonFocused";
                    rootpageId.state = rootpageId.camerastateframeactive;
                    break;
                case "CameraExposureState":
                    camerasettingsrectId.state = "CameraAOIWidthHeightState";
                    break;
                case "CameraGainState":
                    camerasettingsrectId.state = "CameraExposureState";
                    break;
                }


            }

            onDownButtonPressed:
            {
                switch(camerasettingsrectId.state)
                {
                case "CameraAOIWidthHeightState":
                    camerasettingsrectId.state = "CameraExposureState";
                    break;
                case "CameraExposureState":
                    camerasettingsrectId.state = "CameraGainState";
                    break;


                }
            }

            onLeftButtonPressed:
            {

            }

            onRightButtonPressed:
            {

            }

            onUpButtonReleased:
            {

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

        Rectangle
        {
            anchors.fill: camerasettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: camerasettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: camerasettingsborderrectId
            anchors.fill: camerasettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        //Settings Text
        Text
        {
            id: cameratextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Camera Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        //Camera Info
        Text
        {
            id: cameradeviceinfotextId
            font.family: fontId.name
            y: 60
            x:10
            font.pixelSize: 20
            color: rootpageId.textcolor
        }

        //Camera AOI Adjustment Button
        Rectangle
        {
            id: cameraAOIadjustemtId
            x:20
            y:100
            width: 600
            height: 75
            color: "transparent"
            border.width: rootpageId.rectborderwidths
            border.color: rootpageId.nonfocuscolor


            function grabFocus()
            {
                cameraAOIadjustmentcontrollerId.focus = true;
            }

            Text
            {
                id: cameraAOIadjustmenttextId
                anchors.centerIn: parent
                font.family: fontId.name
                font.pointSize: 15
                height: cameraAOIadjustmenttextId.implicitHeight
                width: cameraAOIadjustmenttextId.implicitWidth
                text: "Change Camera AOI Height/Width"
                color: rootpageId.textcolor
            }

            ControllerObject
            {

                id: cameraAOIadjustmentcontrollerId
                focus: false
                useAutoRepeatonSticks: true

                onFocusChanged:
                {
                    if(cameraAOIadjustmentcontrollerId.focus)
                    {
                        cameraaoirectId.border.color = Qt.rgba(0,.5,0,1);
                        cameraAOIadjustemtId.color = Qt.rgba(0,1,0,.2);
                    }
                    else
                    {
                        cameraaoirectId.border.color = Qt.rgba(.5,0,0,1);
                        cameraAOIadjustemtId.color = "transparent";
                    }

                }

                onBlackButtonPressed:
                {
                    camerasettingsrectId.grabFocus();
                }

                onGreenButtonPressed:
                {
                    camerasettingsrectId.grabFocus();
                }

                onRedButtonPressed:
                {
                    camerasettingsrectId.grabFocus();
                }

                onUpButtonPressed:
                {
                    Mary.pylon_aoiheight = Mary.pylon_aoiheight + 20
                }

                onDownButtonPressed:
                {
                    Mary.pylon_aoiheight = Mary.pylon_aoiheight - 20
                }

                onLeftButtonPressed:
                {
                    Mary.pylon_aoiwidth = Mary.pylon_aoiwidth + 20
                }

                onRightButtonPressed:
                {
                    Mary.pylon_aoiwidth = Mary.pylon_aoiwidth - 20
                }

                onUpButtonReleased:
                {

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


        }

        //Exposure Slider
        SliderSettingsObject
        {
            id: exposuresliderId
            controlname: "CameraExposureSlider"
            x:20
            y:195
            messagetext: "Exposure: " + Mary.pylon_exposure + " micro sec.";
            valuefrom: 30
            valueto: 10000
            stepsize: 10
            majorstepsize: 500
            controlstickautorepeat: true
            fontsize: 15


            Component.onCompleted:
            {
                exposuresliderId.value = Mary.getCameraExposure();
            }

            onEndFocus:
            {
                camerasettingsrectId.grabFocus();
                exposuresliderId.state = exposuresliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.pylon_exposure = exposuresliderId.value;
            }
        }

        //Gain Slider
        SliderSettingsObject
        {
            id: gainsliderId
            controlname: "CameraGainSlider"
            x:20
            y:exposuresliderId.y+exposuresliderId.height+20
            messagetext: "Gain: " + Mary.pylon_gain;
            valuefrom: 0
            valueto: 360
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true
            fontsize: 15


            Component.onCompleted:
            {
                 gainsliderId.value = Mary.getCameraGain();
            }

            onEndFocus:
            {
                camerasettingsrectId.grabFocus();
                gainsliderId.state = exposuresliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.pylon_gain =  gainsliderId.value;
            }
        }

    }

    //Blur Settings Rectangle---------------------------------------
    Rectangle
    {
        id: blursettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            blursettingscontrollerId.focus = true;

        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: blursettingsrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: blursliderId; state: blursliderId.nothighlightedstate;}

            },
            State
            {
                name: "BlurState"
                PropertyChanges{target: blursettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: blursliderId; state: blursliderId.highlightedstate;}
            }
        ]

        ControllerObject
        {

            id:blursettingscontrollerId
            focus: false


            onBlackButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onGreenButtonPressed:
            {
                switch(blursettingsrectId.state)
                {
                case "BlurState":
                    blursliderId.grabFocus();
                    break;
                }
            }

            onRedButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onUpButtonPressed:
            {
                blursettingscontrollerId.state = "NonFocused"
                rootpageId.state = rootpageId.blurstateframeactive;
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

        Rectangle
        {
            anchors.fill: blursettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: blursettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: blursettingsborderrectId
            anchors.fill: blursettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        //Settings Text
        Text
        {
            id: blurtextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Blur Frame Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        SliderSettingsObject
        {
            id: blursliderId
            controlname: "BlurSliderControl"
            x:20
            y:60
            messagetext: "Blur: " + Mary.cv_blur;
            valuefrom: 1
            valueto: 49
            stepsize: 2
            majorstepsize: 4
            controlstickautorepeat: true

            Component.onCompleted:
            {
                blursliderId.value = Mary.getCVBlurValue();
            }

            onEndFocus:
            {
                blursettingsrectId.grabFocus();
                blursliderId.state = blursliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.cv_blur = blursliderId.value;
            }


        }

    }

    //Threshold Settings Rectangle----------------------------------
    Rectangle
    {
        id: thresholdsettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            thresholdsettingscontrollerId.focus = true;
        }

        states:
        [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: thresholdsettingsrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: thresholdminsliderId; state: thresholdminsliderId.nothighlightedstate;}
                PropertyChanges{target: thresholdmaxsliderId; state: thresholdmaxsliderId.nothighlightedstate;}
            },
            State
            {
                name: "MinState"
                PropertyChanges{target: thresholdsettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: thresholdminsliderId; state: thresholdminsliderId.highlightedstate;}
                PropertyChanges{target: thresholdmaxsliderId; state: thresholdmaxsliderId.nothighlightedstate;}

            },
            State
            {
                name: "MaxState"
                PropertyChanges{target: thresholdsettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: thresholdminsliderId; state: thresholdminsliderId.nothighlightedstate;}
                PropertyChanges{target: thresholdmaxsliderId; state: thresholdmaxsliderId.highlightedstate;}
            }
        ]

        ControllerObject
        {

            id:thresholdsettingscontrollerId
            focus: false

            onBlackButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onGreenButtonPressed:
            {
                switch(thresholdsettingsrectId.state)
                {
                case "MinState":
                    thresholdminsliderId.grabFocus();
                    break;
                case "MaxState":
                    thresholdmaxsliderId.grabFocus();
                    break;
                }
            }

            onRedButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onUpButtonPressed:
            {
                switch(thresholdsettingsrectId.state)
                {
                case "MinState":
                    rootpageId.state = rootpageId.thresholdstateframeactive;
                    thresholdsettingsrectId.state = "NonFocused";
                    break;
                case "MaxState":
                    thresholdsettingsrectId.state = "MinState";
                    break;

                }


            }

            onDownButtonPressed:
            {
                switch(thresholdsettingsrectId.state)
                {
                case "MinState":
                    thresholdsettingsrectId.state = "MaxState";
                    break;
                }
            }

            onLeftButtonPressed:
            {

            }

            onRightButtonPressed:
            {

            }

            onUpButtonReleased:
            {

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

        Rectangle
        {
            anchors.fill: thresholdsettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: thresholdsettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: thresholdsettingsborderrectId
            anchors.fill: thresholdsettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        //Settings Text
        Text
        {
            id: thresholdtextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Threshold Frame Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        //Min Threshold Slider
        SliderSettingsObject
        {
            id: thresholdminsliderId
            controlname: "MinThresholdSlider"
            x:20
            y:60
            messagetext: "Threshold Min: " + Mary.cv_thresholdmin;
            valuefrom: 0
            valueto: Mary.cv_thresholdmax-1
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true


            Component.onCompleted:
            {
                thresholdminsliderId.value = Mary.getCVThresholdMinValue();
            }

            onEndFocus:
            {
                thresholdsettingsrectId.grabFocus();
                thresholdminsliderId.state = thresholdminsliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.cv_thresholdmin = thresholdminsliderId.value;
            }
        }


        //Max Threshold Slider
        SliderSettingsObject
        {
            id: thresholdmaxsliderId
            x:20
            y:thresholdminsliderId.y+thresholdminsliderId.height+20
            messagetext: "Threshold Max: " + Mary.cv_thresholdmax
            valuefrom: Mary.cv_thresholdmin + 1
            valueto: 255
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true


           Component.onCompleted:
           {
               thresholdmaxsliderId.value = Mary.getCVThresholdMaxValue();
           }

           onEndFocus:
           {
               thresholdsettingsrectId.grabFocus();
               thresholdmaxsliderId.state = thresholdmaxsliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.cv_thresholdmax = thresholdmaxsliderId.value;
           }

        }

    }

    //Pixel Column Settings Rectangle
    Rectangle
    {
        id: pixelcolumnsettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        Rectangle
        {
            anchors.fill: pixelcolumnsettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: pixelcolumnsettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: pixelcolumnborderrectId
            anchors.fill: pixelcolumnsettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        ControllerObject
        {

            id:pixelcolumncontrollerId
            focus: false

            onBlackButtonPressed:
            {
            }

            onGreenButtonPressed:
            {
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {
                rootpageId.state = rootpageId.pixelcolumnstateframeactive;
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

        //Settings Text
        Text
        {
            id: pixelcolumntextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Pixel Column Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        //Total Image Intensity
        Text
        {
        id: pixelcolumn_ttitextId
        text: "Total Image Intensity: " + Mary.pc_max_tii
        }
    }

    //Main View Rectangle-------------------------------------------
    Rectangle
    {
        id: mainviewrectId
        visible: true
        width: rootpageId.settingsrectx - rootpageId.frameselectrectx - rootpageId.rectpadding
        height: rootpageId.settingsrectheight
        x: rootpageId.frameselectrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        //Encapsulated To Make Private
        QtObject
        {
            id: mainviewrect_privateId

            property string attached_frame: rootpageId.rawframe

            function detachMaxSignal()
            {
                switch(mainviewrect_privateId.attached_frame)
                {
                case rootpageId.rawframe:
                    Max.newRawMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    //console.log("Detaching Max.newRawMatProcessed");
                    break;
                case rootpageId.blurframe:
                    Max.newBlurMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    //console.log("Detaching Max.newBlurMatProcessed");
                    break;
                case rootpageId.thresholdframe:
                    Max.newThresholdMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    //console.log("Detaching Max.newThresholdMatProcessed");
                    break;
                case rootpageId.pixelcolumnframe:
                    Max.newPixelColumnMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                }
                mainviewrect_privateId.attached_frame = rootpageId.emptyframe;
            }

            function attachMaxSignal (_frame)
            {
                //Now Reconnect To New Frame Signal From Max
                switch(_frame)
                {
                case rootpageId.rawframe:
                    Max.newRawMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.rawframe;
                    //console.log("Connecting Max.newRawMatProcessed");
                    break;
                case rootpageId.blurframe:
                    Max.newBlurMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.blurframe;
                    //console.log("Connecting Max.newBlurMatProcessed");
                    break;
                case rootpageId.thresholdframe:
                    Max.newThresholdMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.thresholdframe;
                    //console.log("Connecting Max.newThresholdMatProcessed");
                    break;
                case rootpageId.pixelcolumnframe:
                    Max.newPixelColumnMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.pixelcolumnframe;
                    break;
                default:
                    mainviewrectId.displayed_frame = rootpageId.emptyframe;
                    break;
                }
            }
        }

        property string displayed_frame: rootpageId.rawframe

        onDisplayed_frameChanged:
        {
            //console.log("");
            //console.log("onDisplayed_frameChanged Starting. displayed_frame = " + displayed_frame + " - attached_frame = " + mainviewrect_privateId.attached_frame);
            mainviewrect_privateId.detachMaxSignal();
            mainviewrect_privateId.attachMaxSignal(mainviewrectId.displayed_frame);
            //console.log("onDisplayed_frameChanged Finished. displayed_frame = " + displayed_frame + " - attached_frame = " + mainviewrect_privateId.attached_frame);
            //console.log("");
        }

        Component.onCompleted:
        {

        }

        Rectangle
        {
            anchors.fill: mainviewrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: mainviewrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            anchors.fill: mainviewrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        QmlTBIDisplay
        {
            id: mainviewdisplayId
            anchors.centerIn: mainviewrectId
            width: mainviewrectId.width - 2*rootpageId.rectborderwidths - 5
            //The Camera Has a Native Resolution of 720 x 540
            //It is 1.333 times as wide is it is High. Or .75 as High as it is Wide
            //height: mainviewdisplayId.width * .75
            height: mainviewrectId.height - 2*rootpageId.rectborderwidths - 5

            Component.onCompleted:
            {
                mainviewdisplayId.scaleToWidth = mainviewdisplayId.width
                Max.newRawMatProcessed.connect(mainviewdisplayId.recieveCVMat);
            }

            //AOI Rectangle
            Rectangle
            {
                id: cameraaoirectId
                color: "transparent"
                anchors.centerIn: parent
                width: Mary.pylon_aoiwidth * scalex
                height: Mary.pylon_aoiheight * scaley
                border.color: Qt.rgba(.5,0,0,1);
                border.width: 2
                readonly property real scalex: mainviewdisplayId.width/Mary.pylon_maxwidth
                readonly property real  scaley: mainviewdisplayId.height/Mary.pylon_maxheight
            }
        }
    }

    //Graphical Key-------------------------------------------------
    ControllerKeyObject
    {
        id: controlkeyId
        greenbuttonmessage: "Select"
        redbuttonmessage: "Back"
        blackbuttonmessage: "Save"
        enableblackbutton: true
    }
}
