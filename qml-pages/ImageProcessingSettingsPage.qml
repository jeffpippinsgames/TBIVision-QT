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
    readonly property string pagename: "Pipeline Settings Page"

    //property string fontsource: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource: "qrc:/Fonts/EurostileBold.ttf"
    //property string fontsource: "qrc:/Fonts/Measurements.ttf"
    //property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string fontsource: "qrc:/Fonts/PermanentMarker-Regular.ttf"
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
    readonly property string skeletalstateframenotactive: "SkeletalStateFrameNotActive"
    readonly property string skeletalstateframeactive: "SkeletalStateFrameActive"
    readonly property string ransacstateframenotactive: "RansacStateFrameNotActive" //Top Surface Lines
    readonly property string ransacstateframeactive: "RansacStateFrameActive" //Top Surface Lines
    readonly property string splitmergestateframenotactive: "SplitMergeStateFrameNotActive" //Topography Lines
    readonly property string splitmergestateframeactive: "SplitMergeStateFrameActive" //Topography Lines

    //Frame Constants. For Display Switching and Signal Connections to Max
    readonly property string emptyframe: "NoFrame"
    readonly property string rawframe: "1-Camera"
    readonly property string blurframe: "2-Blur"
    readonly property string thresholdframe: "3-Threshold"
    readonly property string pixelcolumnframe: "4-PixelColumn"
    readonly property string skeletalframe: "5-Skeletal"
    readonly property string ransacframe: "6-Ransac"
    readonly property string splitmergeframe: "7-SplitMerge"

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
    signal destroyPage(string _transition_page)

    //Misc Functions-------------------------------------------------
    function grabFocus()
    {
        frameselectcontrollerId.focus = true;
    }

    function cleanupForDestruction()
    {
        //Stop The Camera For Transition
        Toby.stopCamera();

        //Save Mary and Update Singletons
        Mary.saveMaryToFile();
        Mary.broadcastSingletonSignals();


        //disconnect all connected signals
        Max.processingComplete.disconnect(rootpageId.triggerTobyNextFrame);
        Max.skeletalArrayChanged.disconnect(skeletalarrayId.updateArray);
        mainviewrectId.disconnectSignal();        
    }

    function triggerTobyNextFrame()
    {
        Toby.triggerCamera();
    }

    //Slots---------------------------------------------------------
    Component.onCompleted:
    {

        //Connect Signals
        Max.processingComplete.connect(rootpageId.triggerTobyNextFrame);

        //Set The State of the Root Component
        rootpageId.state = rootpageId.camerastateframeactive;


        //Load Mary Settings and Update QML and Singletons
        Mary.loadMaryFromFile();


        //Setup and Start Camera
        Toby.setCameraAOIToMax();
        Toby.startCamera();
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
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: camerasettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
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
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: camerasettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSelect
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
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
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: blursettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
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
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: blursettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
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
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: thresholdsettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
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
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: thresholdsettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
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
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: pixelcolumnborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
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
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: pixelcolumnborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.pixelcolumnframe}
            //Set The Interior Control State
            PropertyChanges {target: pixelcolumnsettingsrectId; state: "MaxTII";}
        },
        State //Skeletal Selected Frame Focused
        {
            name: rootpageId.skeletalstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: skeletalsettingsrectId; visible: true;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}

            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: skeletalborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.skeletalframe}
        },
        State //Skeletal Selected and Focused
        {
            name: rootpageId.skeletalstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: skeletalsettingsrectId; visible: true;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: skeletalborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.skeletalframe}
            //Set The Interior Control State
            PropertyChanges {target: skeletalsettingsrectId; state: "ArrayState";}
        },
        State //Ransac Selected Frame Focused
        {
            name: rootpageId.ransacstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: true;}

            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: ransacborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.ransacframe}
        },
        State //Ransac Selected and Focused
        {
            name: rootpageId.ransacstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: true;}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: ransacborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.ransacframe}
            //Set The Interior Control State
            PropertyChanges {target: ransacsettingsrectId; state: "RansacLeftTSLMinAngle";}
        },
        State //Split Merge Selected Frame Focused
        {
            name: rootpageId.splitmergestateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}

            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: ransacborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.splitmergeframe}
        },
        State //Split Merge Selected and Focused
        {
            name: rootpageId.splitmergestateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: skeletalsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: ransacborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: skeletalcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.splitmergeframe}
            //Set The Interior Control State
            //PropertyChanges {target: ransacsettingsrectId; state: "RansacLeftTSLMinAngle";}
        }



    ]

    //OML Components------------------------------------------------

    //Required For Every Page
    PagesDescriptionObject
    {
        id:pagesId
    }

    //Font for UI
    FontLoader
    {
        id: fontId
        source: rootpageId.fontsource

        Component.onCompleted:
        {

        }
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
        text: "Processing Pipeline"
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
        text: "Settings"
        font.pixelSize: 30
        color: rootpageId.textcolor

    }

    //Time in Loop Text----------------------------------------------------
    Text
    {
        id: timeinlooptextId
        font.family: fontId.name
        y: 20
        x:toptextId.x+toptextId.implicitWidth+100
        text:  Max.timeinloop
        font.pixelSize: 25
        color: rootpageId.textcolor

    }

    //Camera FPS Text
    Text
    {
        id: camerafpstextId
        font.family: fontId.name
        text: Toby.camera_fps
        font.pixelSize: 25
        x: timeinlooptextId.x + 300
        y: 20
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

            onFocusChanged:
            {
                if(focus)
                {
                    controlkeyId.enableblackbutton = true;
                }
            }

            onBlackButtonPressed:
            {
                cleanupForDestruction();
                destroyPage(pagesId.mainmenupage);
            }

            onGreenButtonPressed:
            {
                cleanupForDestruction();
                destroyPage(pagesId.mainmenupage);
            }

            onRedButtonPressed:
            {
                cleanupForDestruction();
                destroyPage(pagesId.mainmenupage);
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
                case rootpageId.skeletalframe:
                    rootpageId.state = rootpageId.skeletalstateframenotactive;
                    break;
                case rootpageId.ransacframe:
                    rootpageId.state = rootpageId.ransacstateframenotactive;
                    break;
                case rootpageId.splitmergeframe:
                    rootpageId.state = rootpageId.splitmergestateframenotactive;
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
                        case rootpageId.skeletalframe:
                            Max.newSkeletalMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.ransacframe:
                            Max.newRansacMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.splitmergeframe:
                            Max.newSplitMergeMatProcessed.connect(delegateframeId.recieveCVMat);
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

        //Frame Selection ListView
        ListView
        {
            id: framelistviewId
            anchors.centerIn: frameselectionrectId
            height: frameselectionrectId.height - 10
            width: frameselectionrectId.width - 10
            model: modelId
            delegate: delegateId
            highlight: Rectangle{id: frameselecthighlightrectId; color: "yellow"; radius: 5; opacity: .2}
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
                modelId.append({"frame": rootpageId.skeletalframe});
                modelId.append({"frame": rootpageId.ransacframe});
                modelId.append({"frame": rootpageId.splitmergeframe});

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
                case rootpageId.skeletalframe:
                    rootpageId.state = rootpageId.skeletalstateframeactive;
                    break;
                case rootpageId.ransacframe:
                    rootpageId.state = rootpageId.ransacstateframeactive;
                    break;
                case rootpageId.splitmergeframe:
                    rootpageId.state = rootpageId.splitmergestateframeactive;
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
                if(focus)
                {
                    controlkeyId.enableblackbutton = false;
                }
            }

            onBlackButtonPressed:
            {


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
            text: Toby.getCameraInfo()
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
            messagetext: "Gain: " + Mary.pylon_gain
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

            onFocusChanged:
            {
                if(focus)
                {
                    controlkeyId.enableblackbutton = false;
                }
            }

            onBlackButtonPressed:
            {

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

            onFocusChanged:
            {
                if(focus)
                {
                    controlkeyId.enableblackbutton = false;
                }
            }

            onBlackButtonPressed:
            {

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

        function grabFocus()
        {
            pixelcolumncontrollerId.focus = true;
        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: pixelcolumnborderrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: maxtiisliderId; state: maxtiisliderId.nothighlightedstate;}
                PropertyChanges{target: mintiisliderId; state: mintiisliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersizesliderId; state: maxclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: minclustersizesliderId; state: minclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersincolsliderId; state: maxclustersincolsliderId.nothighlightedstate;}

            },
            State
            {
                name: "MaxTII"
                PropertyChanges{target: pixelcolumnborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: maxtiisliderId; state: maxtiisliderId.highlightedstate;}
                PropertyChanges{target: mintiisliderId; state: mintiisliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersizesliderId; state: maxclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: minclustersizesliderId; state: minclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersincolsliderId; state: maxclustersincolsliderId.nothighlightedstate;}

            },
            State
            {
                name: "MinTII"
                PropertyChanges{target: pixelcolumnborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: maxtiisliderId; state: maxtiisliderId.nothighlightedstate;}
                PropertyChanges{target: mintiisliderId; state: mintiisliderId.highlightedstate;}
                PropertyChanges{target: maxclustersizesliderId; state: maxclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: minclustersizesliderId; state: minclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersincolsliderId; state: maxclustersincolsliderId.nothighlightedstate;}
            },
            State
            {
                name: "MaxCS"
                PropertyChanges{target: pixelcolumnborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: maxtiisliderId; state: maxtiisliderId.nothighlightedstate;}
                PropertyChanges{target: mintiisliderId; state: mintiisliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersizesliderId; state: maxclustersizesliderId.highlightedstate;}
                PropertyChanges{target: minclustersizesliderId; state: minclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersincolsliderId; state: maxclustersincolsliderId.nothighlightedstate;}
            },
            State
            {
                name: "MinCS"
                PropertyChanges{target: pixelcolumnborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: maxtiisliderId; state: maxtiisliderId.nothighlightedstate;}
                PropertyChanges{target: mintiisliderId; state: mintiisliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersizesliderId; state: maxclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: minclustersizesliderId; state: minclustersizesliderId.highlightedstate;}
                PropertyChanges{target: maxclustersincolsliderId; state: maxclustersincolsliderId.nothighlightedstate;}
            },
            State
            {
                name: "MaxCIC"
                PropertyChanges{target: pixelcolumnborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: maxtiisliderId; state: maxtiisliderId.nothighlightedstate;}
                PropertyChanges{target: mintiisliderId; state: mintiisliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersizesliderId; state: maxclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: minclustersizesliderId; state: minclustersizesliderId.nothighlightedstate;}
                PropertyChanges{target: maxclustersincolsliderId; state: maxclustersincolsliderId.highlightedstate;}
            }

        ]

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

            onFocusChanged:
            {
                if(focus)
                {
                    controlkeyId.enableblackbutton = false;
                }
            }

            onBlackButtonPressed:
            {

            }

            onGreenButtonPressed:
            {
                switch(pixelcolumnsettingsrectId.state)
                {
                case "MaxTII":
                    maxtiisliderId.grabFocus();
                    break;
                case "MinTII":
                    mintiisliderId.grabFocus();
                    break;
                case "MaxCS":
                    maxclustersizesliderId.grabFocus();
                    break;
                case "MinCS":
                    minclustersizesliderId.grabFocus();
                    break;
                case "MaxCIC":
                    maxclustersincolsliderId.grabFocus();
                    break;

                }
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {

                switch(pixelcolumnsettingsrectId.state)
                {
                case "MaxTII":
                    rootpageId.state = rootpageId.pixelcolumnstateframeactive;
                    pixelcolumnsettingsrectId.state = "NonFocused";
                    break;
                case "MinTII":
                    pixelcolumnsettingsrectId.state = "MaxTII";
                    break;
                case "MaxCS":
                    pixelcolumnsettingsrectId.state = "MinTII";
                    break;
                case "MinCS":
                    pixelcolumnsettingsrectId.state = "MaxCS";
                    break;
                case "MaxCIC":
                    pixelcolumnsettingsrectId.state = "MinCS";
                    break;
                }
            }

            onDownButtonPressed:
            {
                switch(pixelcolumnsettingsrectId.state)
                {
                case "MaxTII":
                    pixelcolumnsettingsrectId.state = "MinTII";
                    break;
                case "MinTII":
                    pixelcolumnsettingsrectId.state = "MaxCS";
                    break;
                case "MaxCS":
                    pixelcolumnsettingsrectId.state = "MinCS";
                    break;
                case "MinCS":
                    pixelcolumnsettingsrectId.state = "MaxCIC";
                    break;
                case "MaxCIC":
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
            font.family: fontId.name
            height: pixelcolumn_ttitextId.implicitHeight
            width: pixelcolumn_ttitextId.implicitWidth
            y: 60
            x: 10
            font.pixelSize: 20
            color: rootpageId.textcolor
            text: "Total Image Intensity: " + Max.total_image_intensity
        }

        //Max TTI Slider
        SliderSettingsObject
        {
            id: maxtiisliderId
            x: 20
            y: pixelcolumn_ttitextId.y + pixelcolumn_ttitextId.height + 20
            messagetext: "Max Image Intensity: " + Mary.pc_max_tii
            valuefrom: Mary.pc_min_tii + 1
            valueto: 15000000
            stepsize: 1
            majorstepsize: 50000
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                maxtiisliderId.value = Mary.pc_max_tii;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                maxtiisliderId.state = maxtiisliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.pc_max_tii = maxtiisliderId.value;
            }

        }

        //Min TTI Slider
        SliderSettingsObject
        {
            id: mintiisliderId
            x: 20
            y: maxtiisliderId.y + maxtiisliderId.height + 20
            messagetext: "Min Image Intensity: " + Mary.pc_min_tii
            valuefrom: 0
            valueto: Mary.pc_max_tii - 1
            stepsize: 1
            majorstepsize: 50000
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                mintiisliderId.value = Mary.pc_min_tii;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                mintiisliderId.state = mintiisliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.pc_min_tii = mintiisliderId.value;
            }

        }

        //Max Cluster Size Slider
        SliderSettingsObject
        {
            id: maxclustersizesliderId
            x: 20
            y: mintiisliderId.y + mintiisliderId.height + 20
            messagetext: "Max Pixel Cluster Size: " + Mary.pc_max_cluster_size
            valuefrom: Mary.pc_min_cluster_size + 1
            valueto: 200
            stepsize: 1
            majorstepsize: 10
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                maxclustersizesliderId.value = Mary.pc_max_cluster_size;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                maxclustersizesliderId.state = maxclustersizesliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.pc_max_cluster_size = maxclustersizesliderId.value;
            }

        }

        //Min Cluster Size Slider
        SliderSettingsObject
        {
            id: minclustersizesliderId
            x: 20
            y: maxclustersizesliderId.y + maxclustersizesliderId.height + 20
            messagetext: "Min Pixel Cluster Size: " + Mary.pc_min_cluster_size
            valuefrom: 3
            valueto: Mary.pc_max_cluster_size - 1
            stepsize: 1
            majorstepsize: 10
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                minclustersizesliderId.value = Mary.pc_min_cluster_size;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                minclustersizesliderId.state = minclustersizesliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.pc_min_cluster_size = minclustersizesliderId.value;
            }

        }

        //Max Clusters in Column Slider
        SliderSettingsObject
        {
            id: maxclustersincolsliderId
            x: 20
            y: minclustersizesliderId.y + minclustersizesliderId.height + 20
            messagetext: "Max Clusters in Column: " + Mary.pc_max_clusters_in_column
            valuefrom: 1
            valueto: 10
            stepsize: 1
            majorstepsize: 1
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                maxclustersincolsliderId.value = Mary.pc_max_clusters_in_column;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                maxclustersincolsliderId.state = maxclustersincolsliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.pc_max_clusters_in_column = maxclustersincolsliderId.value;
            }

        }

    }

    //Skeletal Settings Rectangle
    Rectangle
    {
        id: skeletalsettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            skeletalcontrollerId.focus = true;
        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: skeletalborderrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: skeletalarrayId; state: skeletalarrayId.nothighlightedstate;}
                PropertyChanges{target: discontinuitysliderId; state: discontinuitysliderId.nothighlightedstate;}
            },
            State
            {
                name: "ArrayState"
                PropertyChanges{target: skeletalborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: skeletalarrayId; state: skeletalarrayId.highlightedstate;}
                PropertyChanges{target: discontinuitysliderId; state: discontinuitysliderId.nothighlightedstate;}
            },
            State
            {
                name: "DiscState"
                PropertyChanges{target: skeletalborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: skeletalarrayId; state: skeletalarrayId.nothighlightedstate;}
                PropertyChanges{target: discontinuitysliderId; state: discontinuitysliderId.highlightedstate;}
            }
        ]

        Rectangle
        {
            anchors.fill: skeletalsettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: skeletalsettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: skeletalborderrectId
            anchors.fill: skeletalsettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        ControllerObject
        {

            id:skeletalcontrollerId
            focus: false

            onFocusChanged:
            {
                if(focus)
                {
                    controlkeyId.enableblackbutton = false;
                }
            }

            onBlackButtonPressed:
            {

            }

            onGreenButtonPressed:
            {
                switch(skeletalsettingsrectId.state)
                {
                case "ArrayState":
                    skeletalarrayId.grabFocus();
                    break;
                case "DiscState":
                    discontinuitysliderId.grabFocus();
                    break;

                }
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {

                switch(skeletalsettingsrectId.state)
                {
                case "ArrayState":
                    rootpageId.state = skeletalstateframeactive;
                    skeletalsettingsrectId.state = "NonFocused";
                    break;
                case "DiscState":
                    skeletalsettingsrectId.state = "ArrayState";
                    break;
                }
            }

            onDownButtonPressed:
            {
                switch(skeletalsettingsrectId.state)
                {
                case "ArrayState":
                    skeletalsettingsrectId.state = "DiscState";
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

        //Settings Text
        Text
        {
            id: skeletaltextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Skeletal Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        //Skeletal Array
        ArrayViewerObject
        {
            id: skeletalarrayId
            x:20
            y:100
            width:760
            title_text: "Skeletal Array"
            controlstickautorepeat: true

            function updateArray(_value, _index)
            {
                if(_index < skeletalarrayId.listModelSize())
                {
                    skeletalarrayId.updateElementValue(_value, _index);
                }
                else
                {
                    skeletalarrayId.addListElement(_value, _index);
                }
            }

            Component.onCompleted:
            {
                Max.skeletalArrayChanged.connect(skeletalarrayId.updateArray);
            }

            onEndFocus:
            {
                skeletalsettingsrectId.grabFocus();
                skeletalsettingsrectId.state = "ArrayState";
            }
        }

        //Discontinuity Slider
        SliderSettingsObject
        {
            id: discontinuitysliderId
            controlname: "MaxDiscontinuities"
            x:20
            y: skeletalarrayId.y + skeletalarrayId.height + 20
            messagetext: "Maximum Discontinuities: " + Mary.sk_max_discontinuity;
            valuefrom: 0
            valueto: 700
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;



            Component.onCompleted:
            {
                discontinuitysliderId.value = Mary.sk_max_discontinuity;
            }

            onEndFocus:
            {
                skeletalsettingsrectId.grabFocus();
                discontinuitysliderId.state = discontinuitysliderId.highlightedstate;
            }

            onValueChanged:
            {
                Mary.sk_max_discontinuity = discontinuitysliderId.value;
            }
        }



    }

    //Ransac Settings Rectangle
    Rectangle
    {
       id: ransacsettingsrectId
       visible: false
       width: rootpageId.settingsrectwidth
       height: rootpageId.settingsrectheight
       x: rootpageId.settingsrectx
       y: rootpageId.settingsrecty
       color: "transparent"

       readonly property int sliderwidth: 380
       readonly property int sliderheight: 45
       readonly property int sliderborderwidth: 3
       readonly property int slidersverticalspacing: 5
       readonly property int sliderleftx: 10
       readonly property int sliderrightx: 410
       readonly property int slidertextsize: 15

       function grabFocus()
       {
           ransaccontrollerId.focus = true;
       }

       states:
       [
           State
           {
               name: "NonFocused"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.nonfocuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacLeftTSLMinAngle"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.highlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacLeftTSLMaxAngle"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.highlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}

           },
           State
           {
               name: "RansacLeftTSLMinVotes"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.highlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacLeftTSLDistanceThreshold"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.highlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacLeftTSLIterations"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.highlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightTSLMinAngle"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.highlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightTSLMaxAngle"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.highlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightTSLMinVotes"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.highlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightTSLDistanceThreshold"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.highlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightTSLIterations"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.highlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacLeftBWLMinAngle"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.highlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacLeftBWLMaxAngle"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.highlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacLeftBWLMinVotes"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.highlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacLeftBWLDistanceThreshold"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.highlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacLeftBWLIterations"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.highlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightBWLMinAngle"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.highlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightBWLMaxAngle"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.highlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightBWLMinVotes"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.highlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightBWLDistanceThreshold"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.highlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
           },
           State
           {
               name: "RansacRightBWLIterations"
               PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

               PropertyChanges{target: ransaclefttslminanglesliderId; state: ransaclefttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslmaxanglesliderId; state: ransaclefttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrighttslminanglesliderId; state: ransacrighttslminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslmaxanglesliderId; state: ransacrighttslmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacleftbwlminanglesliderId; state: ransacleftbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlmaxanglesliderId; state: ransacleftbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

               PropertyChanges{target: ransacrightbwlminanglesliderId; state: ransacrightbwlminanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlmaxanglesliderId; state: ransacrightbwlmaxanglesliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
               PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.highlightedstate;}
           }
       ]

       Rectangle
       {
           anchors.fill: ransacsettingsrectId
           color: "black"
           opacity: .8
           radius: 10
       }

       Rectangle
       {
           anchors.fill: ransacsettingsrectId
           color: "darkred"
           opacity: .05
           radius: 10
       }

       Rectangle
       {
           id: ransacborderrectId
           anchors.fill: ransacsettingsrectId
           color: "transparent"
           border.color: rootpageId.nonfocuscolor
           border.width: rootpageId.rectborderwidths
           radius: 10
       }

       Rectangle
       {
           id: ransacmiddlelineId
           anchors.centerIn: ransacsettingsrectId
           height: ransacsettingsrectId.height*.75
           width:6
           color: "transparent"
           border.color: "darkred"
           border.width: 3
           radius: 2
       }


       ControllerObject
       {

           id:ransaccontrollerId
           focus: false

           onFocusChanged:
           {

           }

           onBlackButtonPressed:
           {

           }

           onGreenButtonPressed:
           {
               switch(ransacsettingsrectId.state)
                {
                case "RansacLeftTSLMinAngle":
                    ransaclefttslminanglesliderId.grabFocus();
                    break;
                case "RansacLeftTSLMaxAngle":
                    ransaclefttslmaxanglesliderId.grabFocus();
                    break;
                case "RansacLeftTSLMinVotes":
                    ransaclefttslminvotessliderId.grabFocus();
                    break;
                case "RansacLeftTSLDistanceThreshold":
                    ransaclefttsldistancethresholdsliderId.grabFocus();
                    break;
                case "RansacLeftTSLIterations":
                    ransaclefttsliterationssliderId.grabFocus();
                    break;


                case "RansacRightTSLMinAngle":
                    ransacrighttslminanglesliderId.grabFocus();
                    break;
                case "RansacRightTSLMaxAngle":
                    ransacrighttslmaxanglesliderId.grabFocus();
                    break;
                case "RansacRightTSLMinVotes":
                    ransacrighttslminvotessliderId.grabFocus();
                    break;
                case "RansacRightTSLDistanceThreshold":
                    ransacrighttsldistancethresholdsliderId.grabFocus();
                    break;
                case "RansacRightTSLIterations":
                    ransacrighttsliterationssliderId.grabFocus();
                    break;


                case "RansacLeftBWLMinAngle":
                    ransacleftbwlminanglesliderId.grabFocus();
                    break;
                case "RansacLeftBWLMaxAngle":
                    ransacleftbwlmaxanglesliderId.grabFocus();
                    break;
                case "RansacLeftBWLMinVotes":
                    ransacleftbwlminvotessliderId.grabFocus();
                    break;
                case "RansacLeftBWLDistanceThreshold":
                    ransacleftbwldistancethresholdsliderId.grabFocus();
                    break;
                case "RansacLeftBWLIterations":
                    ransacleftbwliterationssliderId.grabFocus();
                    break;


                case "RansacRightBWLMinAngle":
                    ransacrightbwlminanglesliderId.grabFocus();
                    break;
                case "RansacRightBWLMaxAngle":
                    ransacrightbwlmaxanglesliderId.grabFocus();
                    break;
                case "RansacRightBWLMinVotes":
                    ransacrightbwlminvotessliderId.grabFocus();
                    break;
                case "RansacRightBWLDistanceThreshold":
                    ransacrightbwldistancethresholdsliderId.grabFocus();
                    break;
                case "RansacRightBWLIterations":
                    ransacrightbwliterationssliderId.grabFocus();
                    break;
                }

           }

           onRedButtonPressed:
           {

           }

           onUpButtonPressed:
           {

               //Min Angle Slider
               //Max Angle Slider
               //Distance Threshold
               //Iterations
               //Min Votes
               switch(ransacsettingsrectId.state)
               {
               case "RansacLeftTSLMinAngle":
                   ransacsettingsrectId.state = "NonFocused";
                   rootpageId.state = rootpageId.ransacstateframeactive;
                   break;
               case "RansacLeftTSLMaxAngle":
                   ransacsettingsrectId.state = "RansacLeftTSLMinAngle";
                   break;
               case "RansacLeftTSLMinVotes":
                   ransacsettingsrectId.state = "RansacLeftTSLIterations";
                   break;
               case "RansacLeftTSLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacLeftTSLMaxAngle";
                   break;
               case "RansacLeftTSLIterations":
                   ransacsettingsrectId.state = "RansacLeftTSLDistanceThreshold"
                   break;

               case "RansacRightTSLMinAngle":
                   ransacsettingsrectId.state = "NonFocused";
                   rootpageId.state = rootpageId.ransacstateframeactive;
                   break;
               case "RansacRightTSLMaxAngle":
                   ransacsettingsrectId.state = "RansacRightTSLMinAngle";
                   break;
               case "RansacRightTSLMinVotes":
                   ransacsettingsrectId.state = "RansacRightTSLIterations";
                   break;
               case "RansacRightTSLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacRightTSLMaxAngle";
                   break;
               case "RansacRightTSLIterations":
                   ransacsettingsrectId.state = "RansacRightTSLDistanceThreshold";
                   break;


               case "RansacLeftBWLMinAngle":
                   ransacsettingsrectId.state = "RansacLeftTSLMinVotes";
                   break;
               case "RansacLeftBWLMaxAngle":
                   ransacsettingsrectId.state = "RansacLeftBWLMinAngle";
                   break;
               case "RansacLeftBWLMinVotes":
                   ransacsettingsrectId.state = "RansacLeftBWLIterations";
                   break;
               case "RansacLeftBWLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacLeftBWLMaxAngle";
                   break;
               case "RansacLeftBWLIterations":
                   ransacsettingsrectId.state = "RansacLeftBWLDistanceThreshold";
                   break;

               case "RansacRightBWLMinAngle":
                   ransacsettingsrectId.state = "RansacRightTSLMinVotes";
                   break;
               case "RansacRightBWLMaxAngle":
                   ransacsettingsrectId.state = "RansacRightBWLMinAngle";
                   break;
               case "RansacRightBWLMinVotes":
                   ransacsettingsrectId.state = "RansacRightBWLIterations";
                   break;
               case "RansacRightBWLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacRightBWLMaxAngle";
                   break;
               case "RansacRightBWLIterations":
                   ransacsettingsrectId.state = "RansacRightBWLDistanceThreshold"
                   break;
               }
           }

           onDownButtonPressed:
           {
               //Min Angle Slider
               //Max Angle Slider
               //Distance Threshold
               //Iterations
               //Min Votes
               switch(ransacsettingsrectId.state)
               {
               case "RansacLeftTSLMinAngle":
                   ransacsettingsrectId.state = "RansacLeftTSLMaxAngle";
                   break;
               case "RansacLeftTSLMaxAngle":
                   ransacsettingsrectId.state = "RansacLeftTSLDistanceThreshold";
                   break;
               case "RansacLeftTSLMinVotes":
                   ransacsettingsrectId.state = "RansacLeftBWLMinAngle";
                   break;
               case "RansacLeftTSLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacLeftTSLIterations";
                   break;
               case "RansacLeftTSLIterations":
                   ransacsettingsrectId.state = "RansacLeftTSLMinVotes";
                   break;


               case "RansacRightTSLMinAngle":
                   ransacsettingsrectId.state = "RansacRightTSLMaxAngle";
                   break;
               case "RansacRightTSLMaxAngle":
                   ransacsettingsrectId.state = "RansacRightTSLDistanceThreshold";
                   break;
               case "RansacRightTSLMinVotes":
                   ransacsettingsrectId.state = "RansacRightBWLMinAngle";
                   break;
               case "RansacRightTSLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacRightTSLIterations";
                   break;
               case "RansacRightTSLIterations":
                   ransacsettingsrectId.state = "RansacRightTSLMinVotes";
                   break;


               case "RansacLeftBWLMinAngle":
                   ransacsettingsrectId.state = "RansacLeftBWLMaxAngle";
                   break;
               case "RansacLeftBWLMaxAngle":
                   ransacsettingsrectId.state = "RansacLeftBWLDistanceThreshold";
                   break;
               case "RansacLeftBWLMinVotes":
                   ransacsettingsrectId.state = "RansacLeftBWLMinVotes";
                   break;
               case "RansacLeftBWLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacLeftBWLIterations";
                   break;
               case "RansacLeftBWLIterations":
                   ransacsettingsrectId.state = "RansacLeftBWLMinVotes";
                   break;

               case "RansacRightBWLMinAngle":
                   ransacsettingsrectId.state = "RansacRightBWLMaxAngle";
                   break;
               case "RansacRightBWLMaxAngle":
                   ransacsettingsrectId.state = "RansacRightBWLDistanceThreshold";
                   break;
               case "RansacRightBWLMinVotes":
                   ransacsettingsrectId.state = "RansacRightBWLMinVotes"
                   break;
               case "RansacRightBWLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacRightBWLIterations";
                   break;
               case "RansacRightBWLIterations":
                   ransacsettingsrectId.state = "RansacRightBWLMinVotes";
                   break;
               }
           }

           onLeftButtonPressed:
           {
               switch(ransacsettingsrectId.state)
               {
               case "RansacLeftTSLMinAngle":
                   break;
               case "RansacLeftTSLMaxAngle":
                   break;
               case "RansacLeftTSLMinVotes":
                   break;
               case "RansacLeftTSLDistanceThreshold":
                   break;
               case "RansacLeftTSLIterations":
                   break;


               case "RansacRightTSLMinAngle":
                   ransacsettingsrectId.state = "RansacLeftTSLMinAngle";
                   break;
               case "RansacRightTSLMaxAngle":
                   ransacsettingsrectId.state = "RansacLeftTSLMaxAngle";
                   break;
               case "RansacRightTSLMinVotes":
                   ransacsettingsrectId.state = "RansacLeftTSLMinVotes";
                   break;
               case "RansacRightTSLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacLeftTSLDistanceThreshold";
                   break;
               case "RansacRightTSLIterations":
                   ransacsettingsrectId.state = "RansacLeftTSLIterations"
                   break;


               case "RansacLeftBWLMinAngle":
                   break;
               case "RansacLeftBWLMaxAngle":
                   break;
               case "RansacLeftBWLMinVotes":
                   break;
               case "RansacLeftBWLDistanceThreshold":
                   break;
               case "RansacLeftBWLIterations":
                   break;


               case "RansacRightBWLMinAngle":
                   ransacsettingsrectId.state = "RansacLeftBWLMinAngle";
                   break;
               case "RansacRightBWLMaxAngle":
                   ransacsettingsrectId.state = "RansacLeftBWLMaxAngle";
                   break;
               case "RansacRightBWLMinVotes":
                   ransacsettingsrectId.state = "RansacLeftBWLMinVotes";
                   break;
               case "RansacRightBWLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacLeftBWLDistanceThreshold";
                   break;
               case "RansacRightBWLIterations":
                   ransacsettingsrectId.state = "RansacLeftBWLIterations";
                   break;
               }
           }

           onRightButtonPressed:
           {
               switch(ransacsettingsrectId.state)
               {
               case "RansacLeftTSLMinAngle":
                   ransacsettingsrectId.state = "RansacRightTSLMinAngle";
                   break;
               case "RansacLeftTSLMaxAngle":
                   ransacsettingsrectId.state = "RansacRightTSLMaxAngle";
                   break;
               case "RansacLeftTSLMinVotes":
                   ransacsettingsrectId.state = "RansacRightTSLMinVotes";
                   break;
               case "RansacLeftTSLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacRightTSLDistanceThreshold";
                   break;
               case "RansacLeftTSLIterations":
                   ransacsettingsrectId.state = "RansacRightTSLIterations";
                   break;


               case "RansacRightTSLMinAngle":
                   break;
               case "RansacRightTSLMaxAngle":
                   break;
               case "RansacRightTSLMinVotes":
                   break;
               case "RansacRightTSLDistanceThreshold":
                   break;
               case "RansacRightTSLIterations":
                   break;


               case "RansacLeftBWLMinAngle":
                   ransacsettingsrectId.state = "RansacRightBWLMinAngle";
                   break;
               case "RansacLeftBWLMaxAngle":
                   ransacsettingsrectId.state = "RansacRightBWLMaxAngle";
                   break;
               case "RansacLeftBWLMinVotes":
                   ransacsettingsrectId.state = "RansacRightBWLMinVotes"
                   break;
               case "RansacLeftBWLDistanceThreshold":
                   ransacsettingsrectId.state = "RansacRightBWLDistanceThreshold"
                   break;
               case "RansacLeftBWLIterations":
                   ransacsettingsrectId.state = "RansacRightBWLIterations"
                   break;


               case "RansacRightBWLMinAngle":
                   break;
               case "RansacRightBWLMaxAngle":
                   break;
               case "RansacRightBWLMinVotes":
                   break;
               case "RansacRightBWLDistanceThreshold":
                   break;
               case "RansacRightBWLIterations":
                   break;
               }
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
           id: ransactextId
           font.family: fontId.name
           y: 10
           x:10
           text: "Ransac Settings"
           font.pixelSize: 25
           color: rootpageId.textcolor
       }

       //Settings Text
       Text
       {
           id: ransactextleftId
           font.family: fontId.name
           y: 55
           x:10
           text: "Left Top Surface Lines(TSL)"
           font.pixelSize: 20
           width: ransactextleftId.implicitWidth
           height: ransactextleftId.implicitHeight
           color: Qt.rgba(224/255,252/255,13/255,1)
       }

       //Settings Text
       Text
       {
           id: ransactextrightId
           font.family: fontId.name
           y: 55
           x: ransacmiddlelineId.x+ransacmiddlelineId.width + 10
           text: "Right Top Surface Lines(TSL)"
           font.pixelSize: 20
           color: Qt.rgba(224/255,252/255,13/255,1)
       }

       //Settings Text
       Text
       {
           id: ransacbwltextleftId
           font.family: fontId.name
           y: 400
           x:10
           text: "Left Bevel Wall Lines(BWL)"
           font.pixelSize: 20
           width: ransactextleftId.implicitWidth
           height: ransactextleftId.implicitHeight
           color: Qt.rgba(0/255, 255/255, 179/255, 1)
       }

       //Settings Text
       Text
       {
           id: ransacbwltextrightId
           font.family: fontId.name
           y: 400
           x: ransacmiddlelineId.x+ransacmiddlelineId.width + 10
           text: "Right Bevel Wall Lines(BWL)"
           font.pixelSize: 20
           color: Qt.rgba(0/255, 255/255, 179/255, 1)
       }



       //Left TSL-----------------------------------------
       //Left TSL Min Angle Slider
       SliderSettingsObject
       {
           id: ransaclefttslminanglesliderId
           x:ransacsettingsrectId.sliderleftx
           y: ransactextleftId.y+ransactextleftId.height+5
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Min Angle: " + Mary.rn_left_tsl_min_angle;
           valuefrom: -90
           valueto: Mary.rn_left_tsl_max_angle - 1
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransaclefttslminanglesliderId.value = Mary.rn_left_tsl_min_angle;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
              ransaclefttslminanglesliderId.state = ransaclefttslminanglesliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_tsl_min_angle = ransaclefttslminanglesliderId.value;
           }
       }

       //Left TSL Max Angle Slider
       SliderSettingsObject
       {
           id: ransaclefttslmaxanglesliderId
           x: ransacsettingsrectId.sliderleftx
           y: ransaclefttslminanglesliderId.y + ransaclefttslminanglesliderId.height + ransacsettingsrectId.slidersverticalspacing
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Max Angle: " + Mary.rn_left_tsl_max_angle;
           valuefrom: Mary.rn_left_tsl_min_angle + 1
           valueto: 90
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {

               ransaclefttslmaxanglesliderId.value = Mary.rn_left_tsl_max_angle;
           }

           onEndFocus:
           {
               ransacsettingsrectId.grabFocus();
              ransaclefttslmaxanglesliderId.state = ransaclefttslmaxanglesliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_tsl_max_angle = ransaclefttslmaxanglesliderId.value;
           }
       }

       //Left TSL Distance Threshold
       SliderSettingsObject
       {

           id: ransaclefttsldistancethresholdsliderId
           x:ransacsettingsrectId.sliderleftx
           y: ransacsettingsrectId.slidersverticalspacing + ransaclefttslmaxanglesliderId.y + ransaclefttslmaxanglesliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Distance Threshold: " + Mary.rn_left_tsl_distance_threshold;
           valuefrom: 1
           valueto: 10
           stepsize: 1
           majorstepsize: 1
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransaclefttsldistancethresholdsliderId.value = Mary.rn_left_tsl_distance_threshold;
           }

           onEndFocus:
           {
               ransacsettingsrectId.grabFocus();
              ransaclefttsldistancethresholdsliderId.state = ransaclefttsldistancethresholdsliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_tsl_distance_threshold = ransaclefttsldistancethresholdsliderId.value;
           }
       }

       //Left TSL Iterations
       SliderSettingsObject
       {

           id: ransaclefttsliterationssliderId
           x:ransacsettingsrectId.sliderleftx
           y: ransacsettingsrectId.slidersverticalspacing + ransaclefttsldistancethresholdsliderId.y + ransaclefttsldistancethresholdsliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Iterations: " + Mary.rn_left_tsl_iterations;
           valuefrom: 20
           valueto: 300
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransaclefttsliterationssliderId.value = Mary.rn_left_tsl_iterations;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
              ransaclefttsliterationssliderId.state = ransaclefttsliterationssliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_tsl_iterations = ransaclefttsliterationssliderId.value;
           }
       }

       //Left TSL Min Votes
       SliderSettingsObject
       {

           id: ransaclefttslminvotessliderId
           x:ransacsettingsrectId.sliderleftx
           y: ransacsettingsrectId.slidersverticalspacing + ransaclefttsliterationssliderId.y + ransaclefttsliterationssliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Min Votes: " + Mary.rn_left_tsl_votes;
           valuefrom: 1
           valueto: 200
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransaclefttslminvotessliderId.value = Mary.rn_left_tsl_votes
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
               ransaclefttslminvotessliderId.state = ransaclefttslminvotessliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_tsl_votes = ransaclefttslminvotessliderId.value;
           }
       }
       //--------------------------------------------------
       //--------------------------------------------------
       //--------------------------------------------------
       //--------------------------------------------------
       //Left BWL-----------------------------------------
       //Left BWL Min Angle Slider
       SliderSettingsObject
       {
           id: ransacleftbwlminanglesliderId
           x:ransacsettingsrectId.sliderleftx
           y: 430
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Min Angle: " + Mary.rn_left_bwl_min_angle;
           valuefrom: -90
           valueto: Mary.rn_left_bwl_max_angle - 1
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacleftbwlminanglesliderId.value = Mary.rn_left_bwl_min_angle;
           }

           onEndFocus:
           {
               ransacsettingsrectId.grabFocus();
               ransacleftbwlminanglesliderId.state = ransacleftbwlminanglesliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_bwl_min_angle = ransacleftbwlminanglesliderId.value;
           }
       }

       //Left BWL Max Angle Slider
       SliderSettingsObject
       {
           id: ransacleftbwlmaxanglesliderId
           x: ransacsettingsrectId.sliderleftx
           y: ransacleftbwlminanglesliderId.y + ransacleftbwlminanglesliderId.height + ransacsettingsrectId.slidersverticalspacing
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Max Angle: " + Mary.rn_left_bwl_max_angle;
           valuefrom: Mary.rn_left_bwl_min_angle + 1
           valueto: 90
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacleftbwlmaxanglesliderId.value = Mary.rn_left_bwl_max_angle;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
              ransacleftbwlmaxanglesliderId.state = ransacleftbwlmaxanglesliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_bwl_max_angle = ransacleftbwlmaxanglesliderId.value;
           }
       }

       //Left BWL Distance Threshold
       SliderSettingsObject
       {

           id: ransacleftbwldistancethresholdsliderId
           x:ransacsettingsrectId.sliderleftx
           y: ransacsettingsrectId.slidersverticalspacing + ransacleftbwlmaxanglesliderId.y + ransacleftbwlmaxanglesliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Distance Threshold: " + Mary.rn_left_bwl_distance_threshold;
           valuefrom: 1
           valueto: 10
           stepsize: 1
           majorstepsize: 1
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacleftbwldistancethresholdsliderId.value = Mary.rn_left_bwl_distance_threshold;
           }

           onEndFocus:
           {
               ransacsettingsrectId.grabFocus();
               ransacleftbwldistancethresholdsliderId.state = ransacleftbwldistancethresholdsliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_bwl_distance_threshold = ransacleftbwldistancethresholdsliderId.value;
           }
       }

       //Left BWL Iterations
       SliderSettingsObject
       {

           id: ransacleftbwliterationssliderId
           x:ransacsettingsrectId.sliderleftx
           y: ransacsettingsrectId.slidersverticalspacing + ransacleftbwldistancethresholdsliderId.y + ransacleftbwldistancethresholdsliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Iterations: " + Mary.rn_left_bwl_iterations;
           valuefrom: 20
           valueto: 300
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacleftbwliterationssliderId.value = Mary.rn_left_bwl_iterations;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
              ransacleftbwliterationssliderId.state = ransacleftbwliterationssliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_bwl_iterations = ransacleftbwliterationssliderId.value;
           }
       }

       //Left BWL Min Votes
       SliderSettingsObject
       {

           id: ransacleftbwlminvotessliderId
           x:ransacsettingsrectId.sliderleftx
           y: ransacsettingsrectId.slidersverticalspacing + ransacleftbwliterationssliderId.y + ransacleftbwliterationssliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Min Votes: " + Mary.rn_left_bwl_votes;
           valuefrom: 1
           valueto: 200
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacleftbwlminvotessliderId.value = Mary.rn_left_bwl_votes;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
              ransacleftbwlminvotessliderId.state = ransacleftbwlminvotessliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_left_bwl_votes = ransacleftbwlminvotessliderId.value;
           }
       }
       //--------------------------------------------------


       //right TSL-----------------------------------------
       //right TSL Min Angle Slider
       SliderSettingsObject
       {
           id: ransacrighttslminanglesliderId
           x:ransacsettingsrectId.sliderrightx
           y: ransactextrightId.y+ransactextrightId.height+5
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Min Angle: " + Mary.rn_right_tsl_min_angle;
           valuefrom: -90
           valueto: Mary.rn_right_tsl_max_angle - 1
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrighttslminanglesliderId.value = Mary.rn_right_tsl_min_angle;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
               ransacrighttslminanglesliderId.state = ransacrighttslminanglesliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_right_tsl_min_angle = ransacrighttslminanglesliderId.value;
           }
       }

       //right TSL Max Angle Slider
       SliderSettingsObject
       {
           id: ransacrighttslmaxanglesliderId
           x: ransacsettingsrectId.sliderrightx
           y: ransacrighttslminanglesliderId.y + ransacrighttslminanglesliderId.height + ransacsettingsrectId.slidersverticalspacing
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Max Angle: " + Mary.rn_right_tsl_max_angle;
           valuefrom: Mary.rn_right_tsl_min_angle + 1
           valueto: 90
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrighttslmaxanglesliderId.value = Mary.rn_right_tsl_max_angle;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
              ransacrighttslmaxanglesliderId.state = ransacrighttslmaxanglesliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_right_tsl_max_angle = ransacrighttslmaxanglesliderId.value;
           }
       }

       //right TSL Distance Threshold
       SliderSettingsObject
       {

           id: ransacrighttsldistancethresholdsliderId
           x:ransacsettingsrectId.sliderrightx
           y: ransacsettingsrectId.slidersverticalspacing + ransacrighttslmaxanglesliderId.y + ransacrighttslmaxanglesliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Distance Threshold: " + Mary.rn_right_tsl_distance_threshold;
           valuefrom: 1
           valueto: 10
           stepsize: 1
           majorstepsize: 1
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrighttsldistancethresholdsliderId.value = Mary.rn_right_tsl_distance_threshold;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
               ransacrighttsldistancethresholdsliderId.state = ransacrighttsldistancethresholdsliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_right_tsl_distance_threshold = ransacrighttsldistancethresholdsliderId.value;
           }
       }

       //right TSL Iterations
       SliderSettingsObject
       {

           id: ransacrighttsliterationssliderId
           x:ransacsettingsrectId.sliderrightx
           y: ransacsettingsrectId.slidersverticalspacing + ransacrighttsldistancethresholdsliderId.y + ransacrighttsldistancethresholdsliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Iterations: " + Mary.rn_right_tsl_iterations;
           valuefrom: 20
           valueto: 300
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrighttsliterationssliderId.value = Mary.rn_right_tsl_iterations;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
               ransacrighttsliterationssliderId.state = ransacrighttsliterationssliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_right_tsl_iterations = ransacrighttsliterationssliderId.value;
           }
       }

       //right TSL Min Votes
       SliderSettingsObject
       {

           id: ransacrighttslminvotessliderId
           x:ransacsettingsrectId.sliderrightx
           y: ransacsettingsrectId.slidersverticalspacing + ransacrighttsliterationssliderId.y + ransacrighttsliterationssliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "TSL Min Votes: " + Mary.rn_right_tsl_votes;
           valuefrom: 1
           valueto: 200
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrighttslminvotessliderId.value = Mary.rn_right_tsl_votes;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
           }

           onValueChanged:
           {
              Mary.rn_right_tsl_votes = ransacrighttslminvotessliderId.value;
           }
       }
       //--------------------------------------------------
       //--------------------------------------------------
       //--------------------------------------------------
       //--------------------------------------------------
       //right BWL-----------------------------------------
       //right BWL Min Angle Slider
       SliderSettingsObject
       {
           id: ransacrightbwlminanglesliderId
           x:ransacsettingsrectId.sliderrightx
           y: 430
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Min Angle: " + Mary.rn_right_bwl_min_angle;
           valuefrom: -90
           valueto: Mary.rn_right_bwl_max_angle - 1
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrightbwlminanglesliderId.value = Mary.rn_right_bwl_min_angle;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
              ransacrightbwlminanglesliderId.state = ransacrightbwlminanglesliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_right_bwl_min_angle = ransacrightbwlminanglesliderId.value;
           }
       }

       //right BWL Max Angle Slider
       SliderSettingsObject
       {
           id: ransacrightbwlmaxanglesliderId
           x: ransacsettingsrectId.sliderrightx
           y: ransacrightbwlminanglesliderId.y + ransacrightbwlminanglesliderId.height + ransacsettingsrectId.slidersverticalspacing
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Max Angle: " + Mary.rn_right_bwl_max_angle;
           valuefrom: Mary.rn_right_bwl_min_angle + 1
           valueto: 90
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrightbwlmaxanglesliderId.value = Mary.rn_right_bwl_max_angle;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
              ransacrightbwlmaxanglesliderId.state = ransacrightbwlmaxanglesliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_right_bwl_max_angle = ransacrightbwlmaxanglesliderId.value;
           }
       }

       //right BWL Distance Threshold
       SliderSettingsObject
       {

           id: ransacrightbwldistancethresholdsliderId
           x:ransacsettingsrectId.sliderrightx
           y: ransacsettingsrectId.slidersverticalspacing + ransacrightbwlmaxanglesliderId.y + ransacrightbwlmaxanglesliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Distance Threshold: " + Mary.rn_right_bwl_distance_threshold;
           valuefrom: 1
           valueto: 10
           stepsize: 1
           majorstepsize: 1
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrightbwldistancethresholdsliderId.value = Mary.rn_right_bwl_distance_threshold;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
               ransacrightbwldistancethresholdsliderId.state = ransacrightbwldistancethresholdsliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_right_bwl_distance_threshold = ransacrightbwldistancethresholdsliderId.value;
           }
       }

       //right BWL Iterations
       SliderSettingsObject
       {

           id: ransacrightbwliterationssliderId
           x:ransacsettingsrectId.sliderrightx
           y: ransacsettingsrectId.slidersverticalspacing + ransacrightbwldistancethresholdsliderId.y + ransacrightbwldistancethresholdsliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Iterations: " + Mary.rn_right_bwl_iterations;
           valuefrom: 20
           valueto: 300
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrightbwliterationssliderId.value = Mary.rn_right_bwl_iterations;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
              ransacrightbwliterationssliderId.state = ransacrightbwliterationssliderId.highlightedstate;
           }

           onValueChanged:
           {
               Mary.rn_right_bwl_iterations = ransacrightbwliterationssliderId.value;
           }
       }

       //right BWL Min Votes
       SliderSettingsObject
       {

           id: ransacrightbwlminvotessliderId
           x:ransacsettingsrectId.sliderrightx
           y: ransacsettingsrectId.slidersverticalspacing + ransacrightbwliterationssliderId.y + ransacrightbwliterationssliderId.height
           width: ransacsettingsrectId.sliderwidth
           height: ransacsettingsrectId.sliderheight
           messagetext: "BWL Min Votes: " + Mary.rn_right_bwl_votes;
           valuefrom: 1
           valueto: 200
           stepsize: 1
           majorstepsize: 5
           controlstickautorepeat: true;
           fontsize: ransacsettingsrectId.slidertextsize
           highlightbordersize: ransacsettingsrectId.sliderborderwidth

           Component.onCompleted:
           {
               ransacrightbwlminvotessliderId.value = Mary.rn_right_bwl_votes;
           }

           onEndFocus:
           {
              ransacsettingsrectId.grabFocus();
               ransacrightbwlminvotessliderId.state = ransacrightbwlminvotessliderId.highlightedstate;
           }

           onValueChanged:
           {
              Mary.rn_right_bwl_votes = ransacrightbwlminvotessliderId.value;
           }
       }
       //--------------------------------------------------





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

        function disconnectSignal()
        {
            mainviewrect_privateId.detachMaxSignal();
        }

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
                case rootpageId.skeletalframe:
                    Max.newSkeletalMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.ransacframe:
                    Max.newRansacMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.splitmergeframe:
                    Max.newSplitMergeMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
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
                case rootpageId.skeletalframe:
                    Max.newSkeletalMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.skeletalframe;
                    break;
                case rootpageId.ransacframe:
                    Max.newRansacMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.ransacframe;
                    break;
                case rootpageId.splitmergeframe:
                    Max.newSplitMergeMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.splitmergeframe;
                    console.log("Connecting Max.newSplitMergeMatProcessed");
                    break;
                default:
                    mainviewrectId.displayed_frame = rootpageId.emptyframe;
                    break;
                }
            }
        }

        property string displayed_frame: rootpageId.emptyframe

        onDisplayed_frameChanged:
        {
            //console.log("");
            //console.log("onDisplayed_frameChanged Starting. displayed_frame = " + displayed_frame + " - attached_frame = " + mainviewrect_privateId.attached_frame);
            mainviewrect_privateId.detachMaxSignal();
            mainviewrect_privateId.attachMaxSignal(mainviewrectId.displayed_frame);
            //console.log("onDisplayed_frameChanged Finished. displayed_frame = " + displayed_frame + " - attached_frame = " + mainviewrect_privateId.attached_frame);
            //console.log("");
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
            x: rootpageId.rectborderwidths
            y: rootpageId.rectborderwidths
            //anchors.centerIn: mainviewrectId
            width: mainviewrectId.width - 2*rootpageId.rectborderwidths - 5
            //The Camera Has a Native Resolution of 720 x 540
            //It is 1.333 times as wide is it is High. Or .75 as High as it is Wide
            //height: mainviewdisplayId.width * .75
            height: mainviewrectId.height - 2*rootpageId.rectborderwidths - 5

            Component.onCompleted:
            {
                mainviewdisplayId.scaleToWidth = mainviewdisplayId.width
                Max.newRawMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                mainviewrectId.displayed_frame = rootpageId.rawframe;
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
        blackbuttonmessage: "Exit & Save"
        enableredbutton: false
        enableblackbutton: true
        buttonspacing: 120

    }
}
