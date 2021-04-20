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

    //Frame Constants. For Display Switching and Signal Connections to Max
    readonly property string emptyframe: "NoFrame"
    readonly property string rawframe: "Camera"
    readonly property string blurframe: "Blur"
    readonly property string thresholdframe: "Threshold"

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
        aboutToDestroy();
        destroyPage();
    }

    //Slots
    Component.onCompleted:
    {
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
           //Set The Highlighted Rect
           PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
           PropertyChanges{ target: camerasettingsborderrectId; border.color: rootpageId.nonfocuscolor}
           //SetController focus To FrameSele
           PropertyChanges{target: blursettingscontrollerId; focus: false}
           PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
           PropertyChanges{target: camerasettingscontrollerId; focus: false}
           PropertyChanges{target: frameselectcontrollerId; focus: true}
           //SetMainViewRect Display
           PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.rawframe}

       },
       State //Camera Selected and Focused
       {
           name: rootpageId.camerastateframenotactive
           //Set Displayed Setting Rect
           PropertyChanges{ target: camerasettingsrectId; visible: true;}
           PropertyChanges{ target: blursettingsrectId; visible: false;}
           PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
           //Set The Highlighted Rect
           PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
           PropertyChanges{ target: camerasettingsborderrectId; border.color: rootpageId.focuscolor}
           //SetController focus To FrameSelect
           PropertyChanges{target: blursettingscontrollerId; focus: false}
           PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
           PropertyChanges{target: frameselectcontrollerId; focus: false}
           PropertyChanges{target: camerasettingscontrollerId; focus: true}
           //SetMainViewRect Display
           PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.rawframe}
       },
       State //Blur Selected Frame Focused
       {
           name: rootpageId.blurstateframeactive
           //Set Displayed Setting Rect
           PropertyChanges{ target: camerasettingsrectId; visible: false;}
           PropertyChanges{ target: blursettingsrectId; visible: true;}
           PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
           //Set The Highlighted Rect
           PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
           PropertyChanges{ target: blursettingsborderrectId; border.color: rootpageId.nonfocuscolor}
           //SetController focus To FrameSele
           PropertyChanges{target: blursettingscontrollerId; focus: false}
           PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
           PropertyChanges{target: camerasettingscontrollerId; focus: false}
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
           //Set The Highlighted Rect
           PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
           PropertyChanges{ target: blursettingsborderrectId; border.color: rootpageId.focuscolor}
           //SetController focus To FrameSele
           PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
           PropertyChanges{target: camerasettingscontrollerId; focus: false}
           PropertyChanges{target: frameselectcontrollerId; focus: false}
           PropertyChanges{target: blursettingscontrollerId; focus: true}
           //SetMainViewRect Display
           PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.blurframe}
       },
       State //Threshold Selected Frame Focused
       {
           name: rootpageId.thresholdstateframeactive
           //Set Displayed Setting Rect
           PropertyChanges{ target: camerasettingsrectId; visible: false;}
           PropertyChanges{ target: blursettingsrectId; visible: false;}
           PropertyChanges{ target: thresholdsettingsrectId; visible: true;}
           //Set The Highlighted Rect
           PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
           PropertyChanges{ target: thresholdsettingsborderrectId; border.color: rootpageId.nonfocuscolor}
           //SetController focus To FrameSele
           PropertyChanges{target: blursettingscontrollerId; focus: false}
           PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
           PropertyChanges{target: camerasettingscontrollerId; focus: false}
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
           //Set The Highlighted Rect
           PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
           PropertyChanges{ target: thresholdsettingsborderrectId; border.color: rootpageId.focuscolor}
           //SetController focus To FrameSele
           PropertyChanges{target: camerasettingscontrollerId; focus: false}
           PropertyChanges{target: blursettingscontrollerId; focus: false}
           PropertyChanges{target: frameselectcontrollerId; focus: false}
           PropertyChanges{target: thresholdsettingscontrollerId; focus: true}
           //SetMainViewRect Display
           PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.thresholdframe}
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

        onVisibleChanged:
        {
            if(camerasettingsrectId.visible)
            {
                cameradeviceinfotextId.text = Toby.getCameraInfo();
            }
        }

        ControllerObject
        {

            id:camerasettingscontrollerId
            focus: false

            onBlackButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onGreenButtonPressed:
            {
            }

            onRedButtonPressed:
            {
                rootpageId.signalDestroyPage();
            }

            onUpButtonPressed:
            {
                rootpageId.state = rootpageId.camerastateframeactive;
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

        ControllerObject
        {

            id:blursettingscontrollerId
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

        ControllerObject
        {

            id:thresholdsettingscontrollerId
            focus: false

            onBlackButtonPressed:
            {
            }

            onGreenButtonPressed:
            {
            }

            onRedButtonPressed:
            {
                rootpage_privateId.setState(rootpageId.thresholdstateframeactive);
            }

            onUpButtonPressed:
            {
                rootpageId.state = rootpageId.state = rootpageId.thresholdstateframeactive;
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
