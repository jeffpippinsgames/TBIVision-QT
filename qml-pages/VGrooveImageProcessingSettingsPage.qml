import QtQuick 2.0
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import "qrc:/qml-components"
import "qrc:/qml-pages"
import "qrc:/qml-dialogs"
import tbi.vision.components 1.0

Item {

    id: rootpageId
    anchors.fill: parent
    visible: true

    readonly property bool showdebug: false
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
    readonly property string preblurstateframeactive: "PreBlurStateFrameActive"
    readonly property string preblurstateframenotactive: "PreBlurStateFrameNotActive"
    readonly property string postblurstateframeactive: "PostBlurStateFrameActive"
    readonly property string postblurstateframenotactive: "PostBlurStateFrameNotActive"
    readonly property string erodestateframeactive: "ErodeStateFrameActive"
    readonly property string erodestateframenotactive: "ErodeStateFrameNotActive"
    readonly property string edgedetectframeactive: "EdgeStateFrameActive"
    readonly property string edgedetectframenotactive: "EdgeStateFrameNotActive"
    readonly property string thresholdstateframeactive: "ThresholdStateFrameActive"
    readonly property string thresholdstateframenotactive: "ThresholdStateFrameNotActive"
    readonly property string pixelcolumnstateframenotactive: "PixelColumnStateFrameNotActive"
    readonly property string pixelcolumnstateframeactive: "PixelColumnStateFrameActive"
    readonly property string inlierdatasetstateframenotactive: "inlierdatasetStateFrameNotActive"
    readonly property string inlierdatasetstateframeactive: "inlierdatasetStateFrameActive"
    readonly property string ransacstateframenotactive: "RansacStateFrameNotActive" //Top Surface Lines
    readonly property string ransacstateframeactive: "RansacStateFrameActive" //Top Surface Lines
    readonly property string geoconstructionstateframenotactive: "geoconstructionStateFrameNotActive"
    readonly property string geoconstructionstateframeactive: "geoconstructionStateFrameActive"
    readonly property string operationstateframenotactive: "OperationStateFrameNotActive" //Operation
    readonly property string operationstateframeactive: "OperationStateFrameActive" //Operation

    //Frame Constants. For Display Switching and Signal Connections to Max
    readonly property string emptyframe: "NoFrame"
    readonly property string rawframe: "1-Camera"
    readonly property string preblurframe: "2-Pre Edge Blur"
    readonly property string erodeframe: "3-Erode"
    readonly property string edgeframe: "4-Laser Boundry"
    readonly property string postblurframe: "5- Laser Extraction"
    readonly property string thresholdframe: "6-Threshold"
    readonly property string pixelcolumnframe: "7-Gausian Declustering"
    readonly property string ransacframe: "8-Inlier Ransacs"
    readonly property string inlierdatasetframe: "9-Inlier DataSets"
    readonly property string geoconstructionframe: "10-Geo Constructions"
    readonly property string operationframe: "11-Operations"

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
        //Stop The Camera and Still Image Processing For Transition
        Toby.stopCamera();
        //Toby.closeStillImagetoProcess();

        //Save Mary and Update Singletons
        Mary.saveSettingsToDefaultFile();

        //disconnect all connected signals
        Max.processingComplete.disconnect(rootpageId.triggerTobyNextFrame);
        mainviewrectId.disconnectSignal();
        operationsettingsrectId.disconnectAllSignals();

        Max.emitExtraMats = false;

    }

    function triggerTobyNextFrame()
    {
        imagegrabtimerId.restart();
    }


    //Slots---------------------------------------------------------
    Component.onCompleted:
    {
        WeldJointType.setJointToVGroove();

        //Connect Signals
        Max.processingComplete.connect(rootpageId.triggerTobyNextFrame);

        //Set The State of the Root Component
        rootpageId.state = rootpageId.camerastateframeactive;

        //Setup and Start Camera
        Toby.setCameraAOIToMax();
        Toby.startCamera();
        Max.emitExtraMats = true;
    }

    //Page States---------------------------------------------------
    states:
        [
        State //Camera Selected Frame Focused
        {
            name: rootpageId.camerastateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: true;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: camerasettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
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
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: camerasettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSelect
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.rawframe}
            //Set The Interior Control State
            PropertyChanges {target: camerasettingsrectId; state: "CameraAOIWidthHeightState";}
        },
        State //Pre Blur Selected Frame Focused
        {
            name: rootpageId.preblurstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: true;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: preblursettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.preblurframe}
        },
        State //Pre Blur Selected and Focused
        {
            name: rootpageId.preblurstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: true;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: preblursettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.preblurframe}
            //Set The Interior Control State
            PropertyChanges {target: preblursettingsrectId; state: "BlurState";}
        },

        State //Erode Selected Frame Focused
        {
            name: rootpageId.erodestateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: true;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: erodesettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.erodeframe}
        },
        State //Erode Selected and Focused
        {
            name: rootpageId.erodestateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: true;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: erodesettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.erodeframe}
            //Set The Interior Control State
            PropertyChanges {target: erodesettingsrectId; state: "ErodeState";}
        },

        State //Edge Selected Frame Focused
        {
            name: rootpageId.edgedetectframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: true;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: edgesettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.edgeframe}
        },
        State //Edge Selected and Focused
        {
            name: rootpageId.edgedetectframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: true;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: edgesettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.edgeframe}
            //Set The Interior Control State
            PropertyChanges {target: edgesettingsrectId; state: "EdgeMinState";}
        },

        State //Post Blur Selected Frame Focused
        {
            name: rootpageId.postblurstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: true;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: postblursettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.postblurframe}
        },
        State //Post Blur Selected and Focused
        {
            name: rootpageId.postblurstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: true;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: postblursettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.postblurframe}
            //Set The Interior Control State
            PropertyChanges {target: postblursettingsrectId; state: "BlurState";}
        },

        State //Threshold Selected Frame Focused
        {
            name: rootpageId.thresholdstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: true;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: thresholdsettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.thresholdframe}
        },
        State //Threshold Selected and Focused
        {
            name: rootpageId.thresholdstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: true;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: thresholdsettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
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
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: true;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: pixelcolumnborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.pixelcolumnframe}
        },
        State //Pixel Column Selected and Focused
        {
            name: rootpageId.pixelcolumnstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: true;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: pixelcolumnborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.pixelcolumnframe}
            //Set The Interior Control State
            PropertyChanges {target: pixelcolumnsettingsrectId; state: "MaxTII";}
        },
        State //inlierdataset Selected Frame Focused
        {
            name: rootpageId.inlierdatasetstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: true;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}

            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: inlierdatasetborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.inlierdatasetframe}
        },
        State //inlierdataset Selected and Focused
        {
            name: rootpageId.inlierdatasetstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: true;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: inlierdatasetborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.inlierdatasetframe}
            //Set The Interior Control State
            PropertyChanges {target: inlierdatasetsettingsrectId; state: "DiscState";}
        },
        State //Ransac Selected Frame Focused
        {
            name: rootpageId.ransacstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: true;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}

            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: ransacborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.ransacframe}
        },
        State //Ransac Selected and Focused
        {
            name: rootpageId.ransacstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: true;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: ransacborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.ransacframe}
            //Set The Interior Control State
            PropertyChanges {target: ransacsettingsrectId; state: "MasterRansac";}
        },
        State //Geometric Construction Frame Focused
        {
            name: rootpageId.geoconstructionstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: true}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: geoconstructionborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.geoconstructionframe}
        },
        State //Geometric Construction Selected and Focused
        {
            name: rootpageId.geoconstructionstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: true}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: ransacborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.geoconstructionframe}
            //Set The Interior Control State
            PropertyChanges {target: geoconstructionsettingsrectId; state: "SplitDistance";}
        },
        State //Operation Selected Frame Focused
        {
            name: rootpageId.operationstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: true}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: operationborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.operationframe}
        },
        State //Operation Selected and Focused
        {
            name: rootpageId.operationstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: preblursettingsrectId; visible: false;}
            PropertyChanges{ target: erodesettingsrectId; visible: false;}
            PropertyChanges{ target: edgesettingsrectId; visible: false;}
            PropertyChanges{ target: postblursettingsrectId; visible: false;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: true}

            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: operationborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: preblursettingscontrollerId; focus: false}
            PropertyChanges{target: erodesettingscontrollerId; focus: false}
            PropertyChanges{target: edgesettingscontrollerId; focus: false}
            PropertyChanges{target: postblursettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: true}
            //SetMainViewRect Display
            PropertyChanges{target: mainviewrectId; displayed_frame: rootpageId.operationframe}
            //Set The Interior Control State
            PropertyChanges {target: operationsettingsrectId; state: "ManualMotorControl";}
        }

    ]

    //OML Components------------------------------------------------
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
        text: "VGroove Pipeline"
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
                case rootpageId.preblurframe:
                    rootpageId.state = rootpageId.preblurstateframenotactive;
                    break;
                case rootpageId.erodeframe:
                    rootpageId.state = rootpageId.erodestateframenotactive;
                    break;
                case rootpageId.edgeframe:
                    rootpageId.state = rootpageId.edgedetectframenotactive;
                    break;
                case rootpageId.postblurframe:
                    rootpageId.state = rootpageId.postblurstateframenotactive;
                    break;
                case rootpageId.thresholdframe:
                    rootpageId.state = rootpageId.thresholdstateframenotactive;
                    break;
                case rootpageId.pixelcolumnframe:
                    rootpageId.state = rootpageId.pixelcolumnstateframenotactive;
                    break;
                case rootpageId.inlierdatasetframe:
                    rootpageId.state = rootpageId.inlierdatasetstateframenotactive;
                    break;
                case rootpageId.ransacframe:
                    rootpageId.state = rootpageId.ransacstateframenotactive;
                    break;
                case rootpageId.geoconstructionframe:
                    rootpageId.state = rootpageId.geoconstructionstateframenotactive;
                    break;
                case rootpageId.operationframe:
                    rootpageId.state = rootpageId.operationstateframenotactive;
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
                        case rootpageId.preblurframe:
                            Max.newPreBlurMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.erodeframe:
                            Max.newErodeMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.edgeframe:
                            Max.newEdgeMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.postblurframe:
                            Max.newPostBlurMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.thresholdframe:
                            Max.newThresholdMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.pixelcolumnframe:
                            Max.newPixelColumnMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.inlierdatasetframe:
                            Max.newInlierDataSetMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.ransacframe:
                            Max.newRansacMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.geoconstructionframe:
                            Max.newGeoConstructionMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case rootpageId.operationframe:
                            Max.newOperationMatProcessed.connect(delegateframeId.recieveCVMat);
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
            spacing: 38
            visible: true

            Component.onCompleted:
            {
                //Add Items to model
                //This Way Will Help with using properties in the frame setting for Listmodel.
                modelId.append({"frame": rootpageId.rawframe});
                modelId.append({"frame": rootpageId.preblurframe});
                modelId.append({"frame": rootpageId.erodeframe});
                modelId.append({"frame": rootpageId.edgeframe});
                modelId.append({"frame": rootpageId.postblurframe});
                modelId.append({"frame": rootpageId.thresholdframe});
                modelId.append({"frame": rootpageId.pixelcolumnframe});
                modelId.append({"frame": rootpageId.ransacframe});
                modelId.append({"frame": rootpageId.inlierdatasetframe});
                modelId.append({"frame": rootpageId.geoconstructionframe});
                modelId.append({"frame": rootpageId.operationframe});
            }

            onCurrentIndexChanged:
            {

                switch(modelId.get(framelistviewId.currentIndex).frame)
                {
                case rootpageId.rawframe:
                    rootpageId.state = rootpageId.camerastateframeactive;
                    break;
                case rootpageId.preblurframe:
                    rootpageId.state = rootpageId.preblurstateframeactive;
                    break;
                case rootpageId.erodeframe:
                    rootpageId.state = rootpageId.erodestateframeactive;
                    break;
                case rootpageId.edgeframe:
                    rootpageId.state = rootpageId.edgedetectframeactive;
                    break;
                case rootpageId.postblurframe:
                    rootpageId.state = rootpageId.postblurstateframeactive;
                    break;
                case rootpageId.thresholdframe:
                    rootpageId.state = rootpageId.thresholdstateframeactive;
                    break;
                case rootpageId.pixelcolumnframe:
                    rootpageId.state = rootpageId.pixelcolumnstateframeactive;
                    break;
                case rootpageId.inlierdatasetframe:
                    rootpageId.state = rootpageId.inlierdatasetstateframeactive;
                    break;
                case rootpageId.ransacframe:
                    rootpageId.state = rootpageId.ransacstateframeactive;
                    break;
                case rootpageId.geoconstructionframe:
                    rootpageId.state = rootpageId.geoconstructionstateframeactive;
                    break;
                case rootpageId.operationframe:
                    rootpageId.state = rootpageId.operationstateframeactive;
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
                cameradeviceinfotextId.text = PylonCameraParameters.cameraName;
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
                PropertyChanges{target: stillimageselectionbuttonId; border.color: rootpageId.nonfocuscolor;}

            },
            State
            {
                name: "CameraAOIWidthHeightState"
                PropertyChanges{target: cameraAOIadjustemtId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: exposuresliderId; state: exposuresliderId.nothighlightedstate;}
                PropertyChanges{target: gainsliderId; state: gainsliderId.nothighlightedstate;}
                PropertyChanges{target: stillimageselectionbuttonId; border.color: rootpageId.nonfocuscolor;}

            },
            State
            {
                name: "CameraExposureState"
                PropertyChanges{target: cameraAOIadjustemtId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: exposuresliderId; state: exposuresliderId.highlightedstate;}
                PropertyChanges{target: gainsliderId; state: gainsliderId.nothighlightedstate;}
                PropertyChanges{target: stillimageselectionbuttonId; border.color: rootpageId.nonfocuscolor;}

            },
            State
            {
                name: "CameraGainState"
                PropertyChanges{target: cameraAOIadjustemtId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: exposuresliderId; state: exposuresliderId.nothighlightedstate;}
                PropertyChanges{target: gainsliderId; state: gainsliderId.highlightedstate;}
                PropertyChanges{target: stillimageselectionbuttonId; border.color: rootpageId.nonfocuscolor;}

            },
            State
            {
                name: "StillImageSelectionState"
                PropertyChanges{target: cameraAOIadjustemtId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: exposuresliderId; state: exposuresliderId.nothighlightedstate;}
                PropertyChanges{target: gainsliderId; state: gainsliderId.nothighlightedstate;}
                PropertyChanges{target: stillimageselectionbuttonId; border.color: rootpageId.focuscolor;}
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
                case "StillImageSelectionState":
                    Toby.updateStillImageFileList();
                    Toby.stopCamera();
                    stillimagefiledialogId.visible = true;
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
                case "StillImageSelectionState":
                    camerasettingsrectId.state = "CameraGainState"
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
                case "CameraGainState":
                    camerasettingsrectId.state = "StillImageSelectionState";
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
            text: PylonCameraParameters.cameraName
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
                    PylonCameraParameters.aoiHeight += 20;

                }

                onDownButtonPressed:
                {
                    PylonCameraParameters.aoiHeight -= 20;
                }

                onLeftButtonPressed:
                {
                    PylonCameraParameters.aoiWidth += 20;
                }

                onRightButtonPressed:
                {
                    PylonCameraParameters.aoiWidth -= 20;
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
            messagetext: "Exposure: " + PylonCameraParameters.exposure + " micro sec.";
            valuefrom: 30
            valueto: 10000
            stepsize: 10
            majorstepsize: 500
            controlstickautorepeat: true
            fontsize: 15


            Component.onCompleted:
            {
                exposuresliderId.value = PylonCameraParameters.exposure;
            }

            onEndFocus:
            {
                camerasettingsrectId.grabFocus();
                exposuresliderId.state = exposuresliderId.highlightedstate;
            }

            onValueChanged:
            {
                PylonCameraParameters.exposure = exposuresliderId.value;
            }
        }

        //Gain Slider
        SliderSettingsObject
        {
            id: gainsliderId
            controlname: "CameraGainSlider"
            x:20
            y:exposuresliderId.y+exposuresliderId.height+20
            messagetext: "Gain: " + PylonCameraParameters.gain
            valuefrom: 0
            valueto: 360
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true
            fontsize: 15


            Component.onCompleted:
            {
                gainsliderId.value = PylonCameraParameters.gain;
            }

            onEndFocus:
            {
                camerasettingsrectId.grabFocus();
                gainsliderId.state = exposuresliderId.highlightedstate;
            }

            onValueChanged:
            {
                PylonCameraParameters.gain =  gainsliderId.value;
            }
        }

        //Still Image Selection
        Rectangle
        {
            id: stillimageselectionbuttonId
            x:20
            y:gainsliderId.y+gainsliderId.height+20
            width: 600
            height: 75
            color: "transparent"
            border.width: rootpageId.rectborderwidths
            border.color: rootpageId.nonfocuscolor

            Text
            {
                id: stillimageselectiontextId
                anchors.centerIn: stillimageselectionbuttonId
                font.family: fontId.name
                font.pointSize: 15
                height: stillimageselectiontextId.implicitHeight
                width: stillimageselectiontextId.implicitWidth
                text: "Not Processing Still Image: Click to Set"
                color: rootpageId.textcolor
            }
        }
    }

    //Pre Blur Settings Rectangle---------------------------------------
    Rectangle
    {
        id: preblursettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            preblursettingscontrollerId.focus = true;

        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: preblursettingsrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: preblursliderId; state: preblursliderId.nothighlightedstate;}

            },
            State
            {
                name: "BlurState"
                PropertyChanges{target: preblursettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: preblursliderId; state: preblursliderId.highlightedstate;}
            }
        ]

        ControllerObject
        {

            id:preblursettingscontrollerId
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
                switch(preblursettingsrectId.state)
                {
                case "BlurState":
                    preblursliderId.grabFocus();
                    break;
                }
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {
                preblursettingscontrollerId.state = "NonFocused"
                rootpageId.state = rootpageId.preblurstateframeactive;
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
            anchors.fill: preblursettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: preblursettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: preblursettingsborderrectId
            anchors.fill: preblursettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        //Settings Text
        Text
        {
            id: preblurtextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Blur Frame Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        SliderSettingsObject
        {
            id: preblursliderId
            controlname: "BlurSliderControl"
            x:20
            y:60
            messagetext: "Blur: " + VGrooveGausianDistroParameters.preBlurValue;
            valuefrom: 1
            valueto: 49
            stepsize: 2
            majorstepsize: 4
            controlstickautorepeat: true

            Component.onCompleted:
            {
                preblursliderId.value = VGrooveGausianDistroParameters.preBlurValue;
            }

            onEndFocus:
            {
                preblursettingsrectId.grabFocus();
                preblursliderId.state = preblursliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.preBlurValue = preblursliderId.value;
            }


        }

    }

    //Erode Settings Rectangle---------------------------------------
    Rectangle
    {
        id: erodesettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            erodesettingscontrollerId.focus = true;

        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: erodesettingsrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: erodesliderId; state: erodesliderId.nothighlightedstate;}

            },
            State
            {
                name: "ErodeState"
                PropertyChanges{target: erodesettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: erodesliderId; state: erodesliderId.highlightedstate;}
            }
        ]

        ControllerObject
        {

            id:erodesettingscontrollerId
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
                switch(erodesettingsrectId.state)
                {
                case "ErodeState":
                    erodesliderId.grabFocus();
                    break;
                }
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {
                erodesettingscontrollerId.state = "NonFocused"
                rootpageId.state = rootpageId.erodestateframeactive;
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
            anchors.fill: erodesettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: erodesettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: erodesettingsborderrectId
            anchors.fill: erodesettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        //Settings Text
        Text
        {
            id: erodetextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Erode Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        SliderSettingsObject
        {
            id: erodesliderId
            controlname: "ErodeSliderControl"
            x:20
            y:60
            messagetext: "Erode Iterations: " + VGrooveGausianDistroParameters.erodeIterations;
            valuefrom: 0
            valueto: 15
            stepsize: 1
            majorstepsize: 1
            controlstickautorepeat: true

            Component.onCompleted:
            {
                erodesliderId.value = VGrooveGausianDistroParameters.erodeIterations;
            }

            onEndFocus:
            {
                erodesettingsrectId.grabFocus();
                erodesliderId.state = erodesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.erodeIterations = erodesliderId.value;
            }
        }
    }

    //Edge Settings Rectangle---------------------------------------
    Rectangle
    {
        id: edgesettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            edgesettingscontrollerId.focus = true;

        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: edgesettingsrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: edgeminsliderId; state: edgeminsliderId.nothighlightedstate;}
                PropertyChanges{target: edgemaxsliderId; state: edgemaxsliderId.nothighlightedstate;}

            },
            State
            {
                name: "EdgeMinState"
                PropertyChanges{target: edgesettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: edgeminsliderId; state: edgeminsliderId.highlightedstate;}
                PropertyChanges{target: edgemaxsliderId; state: edgemaxsliderId.nothighlightedstate;}
            },
            State
            {
                name: "EdgeMaxState"
                PropertyChanges{target: edgesettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: edgeminsliderId; state: edgeminsliderId.nothighlightedstate;}
                PropertyChanges{target: edgemaxsliderId; state: edgemaxsliderId.highlightedstate;}
            }
        ]

        ControllerObject
        {

            id:edgesettingscontrollerId
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
                switch(edgesettingsrectId.state)
                {
                case "EdgeMinState":
                    edgeminsliderId.grabFocus();
                    break;
                case "EdgeMaxState":
                    edgemaxsliderId.grabFocus();
                    break;
                }
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {
                switch(edgesettingsrectId.state)
                {
                case "EdgeMinState":
                    edgesettingsrectId.state = "NonFocused"
                    rootpageId.state = rootpageId.edgedetectframeactive;
                    break;
                case "EdgeMaxState":
                    edgesettingsrectId.state = "EdgeMinState";
                    break;
                }


            }

            onDownButtonPressed:
            {
                switch(edgesettingsrectId.state)
                {
                case "EdgeMinState":
                    edgesettingsrectId.state = "EdgeMaxState"
                    break;
                case "EdgeMaxState":
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
            anchors.fill: edgesettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: edgesettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: edgesettingsborderrectId
            anchors.fill: edgesettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        //Settings Text
        Text
        {
            id: edgetextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Laser Boundry Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        SliderSettingsObject
        {
            id: edgeminsliderId
            controlname: "EdgeMinSliderControl"
            x:20
            y:60
            messagetext: "Boundry Filter Min Threshold: " + VGrooveGausianDistroParameters.edgeMin;
            valuefrom: 1
            valueto: edgemaxsliderId.value
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true

            Component.onCompleted:
            {
                edgeminsliderId.value = VGrooveGausianDistroParameters.edgeMin;
            }

            onEndFocus:
            {
                edgesettingsrectId.grabFocus();
                edgeminsliderId.state = edgeminsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.edgeMin = edgeminsliderId.value;
            }


        }

        SliderSettingsObject
        {
            id: edgemaxsliderId
            controlname: "EdgeMaxSliderControl"
            x:20
            y: edgeminsliderId.y + edgeminsliderId.height + 20
            messagetext: "Boundry Filter Max Threshold: " + VGrooveGausianDistroParameters.edgeMax;
            valuefrom: edgeminsliderId.value
            valueto: 255
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true

            Component.onCompleted:
            {
                edgemaxsliderId.value = VGrooveGausianDistroParameters.edgeMax;
            }

            onEndFocus:
            {
                edgesettingsrectId.grabFocus();
                edgemaxsliderId.state = edgemaxsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.edgeMax = edgemaxsliderId.value;
            }


        }


    }

    //Post Blur Settings Rectangle---------------------------------------
    Rectangle
    {
        id: postblursettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            postblursettingscontrollerId.focus = true;

        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: postblursettingsrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: postblursliderId; state: postblursliderId.nothighlightedstate;}
                PropertyChanges{target: laserboundryminsliderId; state: laserboundryminsliderId.nothighlightedstate;}
                PropertyChanges{target: laserboundrymaxsliderId; state: laserboundrymaxsliderId.nothighlightedstate;}

            },
            State
            {
                name: "BlurState"
                PropertyChanges{target: postblursettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: postblursliderId; state: postblursliderId.highlightedstate;}
                PropertyChanges{target: laserboundryminsliderId; state: laserboundryminsliderId.nothighlightedstate;}
                PropertyChanges{target: laserboundrymaxsliderId; state: laserboundrymaxsliderId.nothighlightedstate;}
            },
            State
            {
                name: "MinState"
                PropertyChanges{target: postblursettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: postblursliderId; state: postblursliderId.nothighlightedstate;}
                PropertyChanges{target: laserboundryminsliderId; state: laserboundryminsliderId.highlightedstate;}
                PropertyChanges{target: laserboundrymaxsliderId; state: laserboundrymaxsliderId.nothighlightedstate;}
            },
            State
            {
                name: "MaxState"
                PropertyChanges{target: postblursettingsrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: postblursliderId; state: postblursliderId.nothighlightedstate;}
                PropertyChanges{target: laserboundryminsliderId; state: laserboundryminsliderId.nothighlightedstate;}
                PropertyChanges{target: laserboundrymaxsliderId; state: laserboundrymaxsliderId.highlightedstate;}
            }
        ]

        ControllerObject
        {

            id:postblursettingscontrollerId
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
                switch(postblursettingsrectId.state)
                {
                case "NonFocused":
                    break;
                case "BlurState":
                    postblursliderId.grabFocus();
                    break;
                case "MinState":
                    laserboundryminsliderId.grabFocus();
                    break;
                case "MaxState":
                    laserboundrymaxsliderId.grabFocus();
                    break;
                }
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {
                switch(postblursettingsrectId.state)
                {
                case "NonFocused":

                    break;
                case "BlurState":
                    postblursettingsrectId.state = "NonFocused"
                    rootpageId.state = rootpageId.postblurstateframeactive;
                    break;
                case "MinState":
                    postblursettingsrectId.state = "BlurState";
                    break;
                case "MaxState":
                    postblursettingsrectId.state = "MinState";
                    break;
                }


            }

            onDownButtonPressed:
            {
                switch(postblursettingsrectId.state)
                {
                case "NonFocused":
                    break;
                case "BlurState":
                    postblursettingsrectId.state ="MinState";
                    break;
                case "MinState":
                    postblursettingsrectId.state = "MaxState";
                    break;
                case "MaxState":
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
            anchors.fill: postblursettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: postblursettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: postblursettingsborderrectId
            anchors.fill: postblursettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        //Settings Text
        Text
        {
            id: postblurtextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Post Edge Blur Frame Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        SliderSettingsObject
        {
            id: postblursliderId
            controlname: "PostBlurSliderControl"
            x:20
            y:60
            messagetext: "Blur: " + VGrooveGausianDistroParameters.postBlurValue;
            valuefrom: 1
            valueto: 49
            stepsize: 2
            majorstepsize: 4
            controlstickautorepeat: true

            Component.onCompleted:
            {
                postblursliderId.value = VGrooveGausianDistroParameters.postBlurValue;
            }

            onEndFocus:
            {
                postblursettingsrectId.grabFocus();
                postblursliderId.state = postblursliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.postBlurValue = postblursliderId.value;
            }


        }

        SliderSettingsObject
        {
            id: laserboundryminsliderId
            controlname: "MinLaserBoundrySliderControl"
            x:20
            y: postblursliderId.y+postblursliderId.height+20
            messagetext: "Min Masking Boundry: " + VGrooveGausianDistroParameters.minLaserBoundry;
            valuefrom: 0
            valueto: laserboundrymaxsliderId.value - 1
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true

            Component.onCompleted:
            {
                laserboundryminsliderId.value = VGrooveGausianDistroParameters.minLaserBoundry;
            }

            onEndFocus:
            {
                postblursettingsrectId.grabFocus();
                laserboundryminsliderId.state = laserboundryminsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.minLaserBoundry = laserboundryminsliderId.value;
            }
        }

        SliderSettingsObject
        {
            id: laserboundrymaxsliderId
            controlname: "MaxLaserBoundrySliderControl"
            x:20
            y: laserboundryminsliderId.y+laserboundryminsliderId.height+20
            messagetext: "Max Masking Boundry: " + VGrooveGausianDistroParameters.maxLaserBoundry;
            valuefrom: laserboundryminsliderId.value+1
            valueto: 30
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true

            Component.onCompleted:
            {
                laserboundrymaxsliderId.value = VGrooveGausianDistroParameters.maxLaserBoundry;
            }

            onEndFocus:
            {
                postblursettingsrectId.grabFocus();
                laserboundrymaxsliderId.state = laserboundrymaxsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.maxLaserBoundry = laserboundrymaxsliderId.value;
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
            messagetext: "Threshold Min: " + VGrooveGausianDistroParameters.minThresholdValue;
            valuefrom: 0
            valueto: VGrooveGausianDistroParameters.maxThresholdValue-1
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true


            Component.onCompleted:
            {
                thresholdminsliderId.value = VGrooveGausianDistroParameters.minThresholdValue;
            }

            onEndFocus:
            {
                thresholdsettingsrectId.grabFocus();
                thresholdminsliderId.state = thresholdminsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.minThresholdValue = thresholdminsliderId.value;
            }
        }


        //Max Threshold Slider
        SliderSettingsObject
        {
            id: thresholdmaxsliderId
            x:20
            y:thresholdminsliderId.y+thresholdminsliderId.height+20
            messagetext: "Threshold Max: " + VGrooveGausianDistroParameters.maxThresholdValue
            valuefrom: VGrooveGausianDistroParameters.minThresholdValue + 1
            valueto: 255
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true


            Component.onCompleted:
            {
                thresholdmaxsliderId.value = VGrooveGausianDistroParameters.maxThresholdValue;
            }

            onEndFocus:
            {
                thresholdsettingsrectId.grabFocus();
                thresholdmaxsliderId.state = thresholdmaxsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.maxThresholdValue = thresholdmaxsliderId.value;
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
            text: "Gausian Declustering Settings"
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
            text: "Total Image Intensity: " + VGrooveGausianDistroParameters.totalImageIntensity
        }

        //Max TTI Slider
        SliderSettingsObject
        {
            id: maxtiisliderId
            x: 20
            y: pixelcolumn_ttitextId.y + pixelcolumn_ttitextId.height + 20
            messagetext: "Max Image Intensity: " + VGrooveGausianDistroParameters.maxImageIntensity
            valuefrom: VGrooveGausianDistroParameters.minImageIntensity + 1
            valueto: 15000000
            stepsize: 1
            majorstepsize: 50000
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                maxtiisliderId.value = VGrooveGausianDistroParameters.maxImageIntensity;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                maxtiisliderId.state = maxtiisliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.maxImageIntensity = maxtiisliderId.value;
            }

        }

        //Min TTI Slider
        SliderSettingsObject
        {
            id: mintiisliderId
            x: 20
            y: maxtiisliderId.y + maxtiisliderId.height + 20
            messagetext: "Min Image Intensity: " + VGrooveGausianDistroParameters.minImageIntensity
            valuefrom: 0
            valueto: VGrooveGausianDistroParameters.maxImageIntensity - 1
            stepsize: 1
            majorstepsize: 50000
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                mintiisliderId.value = VGrooveGausianDistroParameters.minImageIntensity;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                mintiisliderId.state = mintiisliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.minImageIntensity = mintiisliderId.value;
            }

        }

        //Max Cluster Size Slider
        SliderSettingsObject
        {
            id: maxclustersizesliderId
            x: 20
            y: mintiisliderId.y + mintiisliderId.height + 20
            messagetext: "Max Pixel Cluster Size: " + VGrooveGausianDistroParameters.maxClusterSize
            valuefrom: VGrooveGausianDistroParameters.minClusterSize + 1
            valueto: 200
            stepsize: 1
            majorstepsize: 10
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                maxclustersizesliderId.value = VGrooveGausianDistroParameters.maxClusterSize;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                maxclustersizesliderId.state = maxclustersizesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.maxClusterSize = maxclustersizesliderId.value;
            }

        }

        //Min Cluster Size Slider
        SliderSettingsObject
        {
            id: minclustersizesliderId
            x: 20
            y: maxclustersizesliderId.y + maxclustersizesliderId.height + 20
            messagetext: "Min Pixel Cluster Size: " + VGrooveGausianDistroParameters.minClusterSize
            valuefrom: 1
            valueto: VGrooveGausianDistroParameters.maxClusterSize - 1
            stepsize: 1
            majorstepsize: 10
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                minclustersizesliderId.value = VGrooveGausianDistroParameters.minClusterSize;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                minclustersizesliderId.state = minclustersizesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.minClusterSize = minclustersizesliderId.value;
            }

        }

        //Max Clusters in Column Slider
        SliderSettingsObject
        {
            id: maxclustersincolsliderId
            x: 20
            y: minclustersizesliderId.y + minclustersizesliderId.height + 20
            messagetext: "Max Clusters in Column: " + VGrooveGausianDistroParameters.maxClustersInColumn
            valuefrom: 1
            valueto: 10
            stepsize: 1
            majorstepsize: 1
            controlstickautorepeat: true
            fontsize: 15

            Component.onCompleted:
            {
                maxclustersincolsliderId.value = VGrooveGausianDistroParameters.maxClustersInColumn;
            }

            onEndFocus:
            {
                // grabFocus of Root Rectangle
                pixelcolumnsettingsrectId.grabFocus();
                maxclustersincolsliderId.state = maxclustersincolsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.maxClustersInColumn = maxclustersincolsliderId.value;
            }

        }

    }

    //Inlier Data Settings Rectangle
    Rectangle
    {
        id: inlierdatasetsettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            inlierdatasetcontrollerId.focus = true;
        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: inlierdatasetborderrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: discontinuitysliderId; state: discontinuitysliderId.nothighlightedstate;}
            },
            State
            {
                name: "DiscState"
                PropertyChanges{target: inlierdatasetborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: discontinuitysliderId; state: discontinuitysliderId.highlightedstate;}
            }
        ]

        Rectangle
        {
            anchors.fill: inlierdatasetsettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: inlierdatasetsettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: inlierdatasetborderrectId
            anchors.fill: inlierdatasetsettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        ControllerObject
        {

            id:inlierdatasetcontrollerId
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
                switch(inlierdatasetsettingsrectId.state)
                {
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

                switch(inlierdatasetsettingsrectId.state)
                {
                case "DiscState":
                    rootpageId.state = inlierdatasetstateframeactive;
                    inlierdatasetsettingsrectId.state = "NonFocused";
                    break;
                }
            }

            onDownButtonPressed:
            {
                switch(inlierdatasetsettingsrectId.state)
                {

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
            id: inlierdatasettextId
            font.family: fontId.name
            y: 10
            x:10
            text: "inlierdataset Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }


        //Discontinuity Slider
        SliderSettingsObject
        {
            id: discontinuitysliderId
            controlname: "MaxDiscontinuities"
            x:20
            y: 100
            messagetext: "Max Distrobuted Deviation: " + VGrooveGausianDistroParameters.declusterDistrobutionStandardDeviation;
            valuefrom: 0
            valueto: 700
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;



            Component.onCompleted:
            {
                discontinuitysliderId.value = VGrooveGausianDistroParameters.declusterDistrobutionStandardDeviation;
            }

            onEndFocus:
            {
                inlierdatasetsettingsrectId.grabFocus();
                discontinuitysliderId.state = discontinuitysliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.declusterDistrobutionStandardDeviation = discontinuitysliderId.value;
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
                PropertyChanges{target: masterransacparamId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: leftransacparamId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: rightransacparamId; border.color:rootpageId.nonfocuscolor;}
            },
            State
            {
                name: "MasterRansac"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: masterransacparamId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: leftransacparamId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: rightransacparamId; border.color:rootpageId.nonfocuscolor;}
            },
            State
            {
                name: "LeftRansac"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: masterransacparamId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: leftransacparamId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: rightransacparamId; border.color:rootpageId.nonfocuscolor;}
            },
            State
            {
                name: "RightRansac"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: masterransacparamId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: leftransacparamId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: rightransacparamId; border.color:rootpageId.focuscolor;}
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
                case "MasterRansac":
                    masterransacparamId.grabFocus();
                    break;
                case "LeftRansac":
                    leftransacparamId.grabFocus();
                    break;
                case "RightRansac":
                    rightransacparamId.grabFocus();
                    break;
                }
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {
                switch(ransacsettingsrectId.state)
                {
                case "MasterRansac":
                    rootpageId.state = ransacstateframeactive;
                    ransacsettingsrectId.state = "NonFocused";
                    break;
                case "LeftRansac":
                    ransacsettingsrectId.state = "MasterRansac";
                    break;
                case "RightRansac":
                    ransacsettingsrectId.state = "LeftRansac";
                    break;
                }
            }

            onDownButtonPressed:
            {
                switch(ransacsettingsrectId.state)
                {
                case "MasterRansac":
                    ransacsettingsrectId.state = "LeftRansac";
                    break;
                case "LeftRansac":
                    break;
                case "RightRansac":
                    break;
                }
            }

            onLeftButtonPressed:
            {
                switch(ransacsettingsrectId.state)
                {
                case "MasterRansac":
                    break;
                case "LeftRansac":
                    break;
                case "RightRansac":
                    ransacsettingsrectId.state = "LeftRansac";
                    break;
                }
            }

            onRightButtonPressed:
            {
                switch(ransacsettingsrectId.state)
                {
                case "MasterRansac":
                    break;
                case "LeftRansac":
                    ransacsettingsrectId.state = "RightRansac";
                    break;
                case "RightRansac":
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
            id: ransacsettingstextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Ransac Line Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }


        RansacControlParameterObject
        {
            id: masterransacparamId
            x: 25
            y: 60
            ransacparameter: MasterTSLRansacParameters
            titletext: "Master Ransac Parameters"

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                masterransacparamId.state = "NonFocused";
            }
        }

        RansacControlParameterObject
        {
            id: leftransacparamId
            x: 25
            y: 400
            ransacparameter: VGrooveLeftTSLRansacParameters
            titletext: "Left Ransac Parameters"

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                leftransacparamId.state = "NonFocused";
            }
        }

        RansacControlParameterObject
        {
            id: rightransacparamId
            x: 425
            y: 400
            ransacparameter: VGrooveRightTSLRansacParameters
            titletext: "Right Ransac Parameters"

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                rightransacparamId.state = "NonFocused";
            }
        }
    }

    //TBI DataSet Visualization Settings Rectangle
    Rectangle
    {
        id: geoconstructionsettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            geoconstructioncontrollerId.focus = true;
        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: geoconstructionborderrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: geoconstructiondistancetsliderId; state: geoconstructiondistancetsliderId.nothighlightedstate;}
            },
            State
            {
                name: "SplitDistance"
                PropertyChanges{target: geoconstructionborderrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: geoconstructiondistancetsliderId; state: geoconstructiondistancetsliderId.highlightedstate;}
            }
        ]

        ControllerObject
        {

            id:geoconstructioncontrollerId
            focus: false

            onBlackButtonPressed:
            {
            }

            onGreenButtonPressed:
            {
                switch(geoconstructionsettingsrectId.state)
                {
                case "SplitDistance":
                    geoconstructiondistancetsliderId.grabFocus();
                    break;

                }
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {
                rootpageId.state = rootpageId.geoconstructionstateframeactive;
                geoconstructionsettingsrectId.state = "NonFocused";
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
            anchors.fill: geoconstructionsettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: geoconstructionsettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: geoconstructionborderrectId
            anchors.fill: geoconstructionsettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        //Settings Text
        Text
        {
            id: geoconstructiontextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Geometric Contstructions"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }

        //Slider to be Removed
        SliderSettingsObject
        {
            id: geoconstructiondistancetsliderId
            x:20
            y: 60
            messagetext: "Geometric Constructions "
            valuefrom: 0
            valueto: 20
            stepsize: .5
            majorstepsize: 5
            controlstickautorepeat: true;



            Component.onCompleted:
            {

            }

            onEndFocus:
            {
                geoconstructionsettingsrectId.grabFocus();
                geoconstructiondistancetsliderId.state = geoconstructiondistancetsliderId.highlightedstate;
            }

            onValueChanged:
            {

            }
        }


    }

    //Operations Page Setttings Rectangle
    Rectangle
    {
        id: operationsettingsrectId
        visible: false
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

        function grabFocus()
        {
            operationcontrollerId.focus = true;
        }

        function disconnectAllSignals()
        {
            MicroControllerStatusPacket.xMotionStatusChanged.disconnect(xmotionstatustextId.updateMsg);
            MicroControllerStatusPacket.zMotionStatusChanged.disconnect(zmotionstatustextId.updateMsg);
            MicroControllerStatusPacket.xLimitSwitchChanged.disconnect(xlimitstatetextId.updateMsg);
            MicroControllerStatusPacket.zLimitSwitchChanged.disconnect(zlimitstatetextId.updateMsg);
            MicroControllerStatusPacket.controlModeChanged.disconnect(controlmodetextId.updateMsg);
            MicroControllerStatusPacket.motorCalibrationCycleChanged.disconnect(calibcyclestatustextId.updateMsg);
        }

        states:
            [
            State
            {
                name: "NonFocused"
                PropertyChanges{target: operationborderrectId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: manualmotorcontrolbuttonId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: motorcalibrationselectionbuttonId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: recordtracktobuttonId; border.color:rootpageId.nonfocuscolor;}
            },
            State
            {
                name: "ManualMotorControl"
                PropertyChanges{target: operationborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: manualmotorcontrolbuttonId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: motorcalibrationselectionbuttonId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: recordtracktobuttonId; border.color:rootpageId.nonfocuscolor;}
            },
            State
            {
                name: "MotorCalibration"
                PropertyChanges{target: operationborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: manualmotorcontrolbuttonId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: motorcalibrationselectionbuttonId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: recordtracktobuttonId; border.color:rootpageId.nonfocuscolor;}
            },
            State
            {
                name: "RecordTrackTo"
                PropertyChanges{target: operationborderrectId; border.color:rootpageId.focuscolor;}
                PropertyChanges{target: manualmotorcontrolbuttonId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: motorcalibrationselectionbuttonId; border.color:rootpageId.nonfocuscolor;}
                PropertyChanges{target: recordtracktobuttonId; border.color:rootpageId.focuscolor;}

            }
        ]
        //OML Components------------------------------------------------
        ControllerObject
        {

            id:operationcontrollerId
            focus: false

            onBlackButtonPressed:
            {


            }

            onGreenButtonPressed:
            {
                switch(operationsettingsrectId.state)
                {
                case "ManualMotorControl":
                    manualmotorcontrolbuttonId.grabFocus();
                    break;
                case "MotorCalibration":

                    break;
                case "RecordTrackTo":

                    break;
                }
            }

            onRedButtonPressed:
            {

            }

            onUpButtonPressed:
            {
                switch(operationsettingsrectId.state)
                {
                case "ManualMotorControl":
                    rootpageId.state = operationstateframeactive;
                    operationsettingsrectId.state = "NonFocused";
                    break;
                case "MotorCalibration":
                    operationsettingsrectId.state = "ManualMotorControl";
                    break;
                case "RecordTrackTo":
                    operationsettingsrectId.state = "MotorCalibration";
                    break;


                }
            }

            onDownButtonPressed:
            {
                switch(operationsettingsrectId.state)
                {
                case "ManualMotorControl":
                    operationsettingsrectId.state = "MotorCalibration";
                    break;
                case "MotorCalibration":
                    operationsettingsrectId.state = "RecordTrackTo";
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
            anchors.fill: operationsettingsrectId
            color: "black"
            opacity: .8
            radius: 10
        }

        Rectangle
        {
            anchors.fill: operationsettingsrectId
            color: "darkred"
            opacity: .05
            radius: 10
        }

        Rectangle
        {
            id: operationborderrectId
            anchors.fill: operationsettingsrectId
            color: "transparent"
            border.color: rootpageId.nonfocuscolor
            border.width: rootpageId.rectborderwidths
            radius: 10
        }

        //Settings Text
        Text
        {
            id: operationtextId
            font.family: fontId.name
            y: 10
            x:10
            text: "Operational Settings"
            font.pixelSize: 25
            color: rootpageId.textcolor
        }






        //------------------------------------------------------------------------
        //Manual Motor Control Button
        Rectangle
        {
            id: manualmotorcontrolbuttonId
            x:20
            y:75
            width: 600
            height: 75
            color: "transparent"
            border.width: rootpageId.rectborderwidths
            border.color: rootpageId.nonfocuscolor

            function grabFocus()
            {
                manualmotorcontrollerId.focus = true;
            }

            Rectangle
            {
                id: manualmotorcontrolhighlight
                anchors.fill: manualmotorcontrolbuttonId
                color: "green"
                opacity: 0
            }

            ControllerObject
            {
                id: manualmotorcontrollerId
                focus: false

                onFocusChanged:
                {
                    if(manualmotorcontrollerId.focus)
                    {
                        manualmotorcontrolhighlight.opacity = .5;
                    }
                    else
                    {
                        manualmotorcontrolhighlight.opacity = 0;
                    }
                }

                onBlackButtonPressed:
                {
                    Gary.sendStopMovement();
                    operationsettingsrectId.grabFocus();
                }

                onGreenButtonPressed:
                {
                    Max.attemptToToggleControlState();
                }

                onRedButtonPressed:
                {
                    Gary.setControlModeToManual();
                }

                onUpButtonPressed:
                {
                    switch(MicroControllerStatusPacket.controlMode)
                    {
                    case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                        Gary.sendJogUp();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                        break;
                    default:
                        break;
                    }
                }

                onDownButtonPressed:
                {
                    switch(MicroControllerStatusPacket.controlMode)
                    {
                    case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                        Gary.sendJogDown();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                        break;
                    default:
                        break;
                    }

                }

                onLeftButtonPressed:
                {
                    switch(MicroControllerStatusPacket.controlMode)
                    {
                    case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                        Gary.sendJogLeft();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                        Gary.sendJogLeft();
                        break;
                    default:
                        break;
                    }

                }

                onRightButtonPressed:
                {
                    switch(MicroControllerStatusPacket.controlMode)
                    {
                    case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                        Gary.sendJogRight();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                        Gary.sendJogRight();
                        break;
                    default:
                        break;
                    }

                }

                onUpButtonReleased:
                {
                    switch(MicroControllerStatusPacket.controlMode)
                    {
                    case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                        Gary.sendStopMovement();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                        break;
                    default:
                        break;
                    }

                }

                onDownButtonReleased:
                {
                    switch(MicroControllerStatusPacket.controlMode)
                    {
                    case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                        Gary.sendStopMovement();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                        break;
                    default:
                        break;
                    }

                }

                onLeftButtonReleased:
                {
                    switch(MicroControllerStatusPacket.controlMode)
                    {
                    case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                        Gary.sendStopMovement();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                        Gary.sendStopMovement();
                        break;
                    default:
                        break;
                    }

                }

                onRightButtonReleased:
                {
                    switch(MicroControllerStatusPacket.controlMode)
                    {
                    case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                        Gary.sendStopMovement();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                        Gary.sendStopMovement();
                        break;
                    default:
                        break;
                    }

                }

            }

            Text
            {
                id: manualmotorcontroltextId
                anchors.centerIn: manualmotorcontrolbuttonId
                font.family: fontId.name
                font.pointSize: 20
                height: manualmotorcontroltextId.implicitHeight
                width: manualmotorcontroltextId.implicitWidth
                text: "Operate"
                color: rootpageId.textcolor
            }
        }

        //Controller Status
        Text
        {
            id: laststatusguidtextId
            x: 20
            y: manualmotorcontrolbuttonId.y + manualmotorcontrolbuttonId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: laststatusguidtextId.implicitHeight
            width: laststatusguidtextId.implicitWidth
            text: "Last Controller Status GUID = " + MicroControllerStatusPacket.currentControlStatusGUID
            color: rootpageId.textcolor
        }

        //X Motion Status
        Text
        {
            id: xmotionstatustextId
            x: 20
            y: laststatusguidtextId.y + laststatusguidtextId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: xmotionstatustextId.implicitHeight
            width: xmotionstatustextId.implicitWidth
            text: "X Axis Motion Status = " + _msg
            color: rootpageId.textcolor

            property string _msg: ""

            Component.onCompleted:
            {
                MicroControllerStatusPacket.xMotionStatusChanged.connect(xmotionstatustextId.updateMsg);
            }

            function updateMsg()
            {
                switch(MicroControllerStatusPacket.xMotionStatus)
                {
                case GaryMotionStatus.TBI_MOTION_STATUS_IDLE:
                    _msg = "TBI_MOTION_STATUS_IDLE";
                    break;
                case GaryMotionStatus.TBI_MOTIION_STATUS_HOMING:
                    _msg = "TBI_MOTION_STATUS_HOMING";
                    break;
                case GaryMotionStatus.TBI_MOTION_STATUS_JOGGING:
                    _msg = "TBI_MOTION_STATUS_JOGGING";
                    break;
                case GaryMotionStatus.TBI_MOTION_STATUS_GENERAL_ERROR:
                    _msg = "TBI_MOTION_STATUS_GENERAL_ERROR";
                    break;
                case GaryMotionStatus.TBI_MOTION_STATUS_LIMIT_TRIPPED:
                    _msg = "TBI_MOTION_STATUS_LIMIT_TRIPPED";
                    break;
                case GaryMotionStatus.TBI_MOTION_STATUS_MOVING:
                    _msg = "TBI_MOTION_STATUS_MOVING";
                    break;
                }
            }
        }

        //Z Motion Status
        Text
        {
            id: zmotionstatustextId
            x: 20
            y: xmotionstatustextId.y + xmotionstatustextId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: zmotionstatustextId.implicitHeight
            width: zmotionstatustextId.implicitWidth
            text: "Z Axis Motion Status = " + _msg;
            color: rootpageId.textcolor

            property string _msg: ""

            Component.onCompleted:
            {
                MicroControllerStatusPacket.zMotionStatusChanged.connect(zmotionstatustextId.updateMsg);
            }

            function updateMsg()
            {
                switch(MicroControllerStatusPacket.zMotionStatus)
                {
                case GaryMotionStatus.TBI_MOTION_STATUS_IDLE:
                    _msg = "TBI_MOTION_STATUS_IDLE";
                    break;
                case GaryMotionStatus.TBI_MOTIION_STATUS_HOMING:
                    _msg = "TBI_MOTION_STATUS_HOMING";
                    break;
                case GaryMotionStatus.TBI_MOTION_STATUS_JOGGING:
                    _msg = "TBI_MOTION_STATUS_JOGGING";
                    break;
                case GaryMotionStatus.TBI_MOTION_STATUS_GENERAL_ERROR:
                    _msg = "TBI_MOTION_STATUS_GENERAL_ERROR";
                    break;
                case GaryMotionStatus.TBI_MOTION_STATUS_LIMIT_TRIPPED:
                    _msg = "TBI_MOTION_STATUS_LIMIT_TRIPPED";
                    break;
                case GaryMotionStatus.TBI_MOTION_STATUS_MOVING:
                    _msg = "TBI_MOTION_STATUS_MOVING";
                    break;
                }
            }
        }

        //X Position
        Text
        {
            id: xpositiontextId
            x: 20
            y: zmotionstatustextId.y + zmotionstatustextId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: xpositiontextId.implicitHeight
            width: xpositiontextId.implicitWidth
            text: "X Position = " + MicroControllerStatusPacket.xPosition
            color: rootpageId.textcolor
        }

        //Z Position
        Text
        {
            id: zpositiontextId
            x: 20
            y: xpositiontextId.y + xpositiontextId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: zpositiontextId.implicitHeight
            width: zpositiontextId.implicitWidth
            text: "Z Position = " + MicroControllerStatusPacket.zPosition;
            color: rootpageId.textcolor
        }

        //X Limit Switch state:
        Text
        {
            id: xlimitstatetextId
            x: 20
            y: zpositiontextId.y + zpositiontextId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: xlimitstatetextId.implicitHeight
            width: xlimitstatetextId.implicitWidth
            text: "X Limit State = " + _msg;
            color: rootpageId.textcolor

            property string _msg: ""

            Component.onCompleted:
            {
                MicroControllerStatusPacket.xLimitSwitchChanged.connect(xlimitstatetextId.updateMsg);
            }

            function updateMsg()
            {
                switch(MicroControllerStatusPacket.xLimitSwitch)
                {
                case GaryLimitSwitch.TBI_LIMIT_SWITCH_STATE_OK:
                    _msg = "Ok";
                    break;
                case GaryLimitSwitch.TBI_LIMIT_SWITCH_STATE_TRIPPED:
                    _msg = "Tripped";
                    break;
                }
            }

        }
        //Z Limit Switch state:
        Text
        {
            id: zlimitstatetextId
            x: 20
            y: xlimitstatetextId.y + xlimitstatetextId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: zlimitstatetextId.implicitHeight
            width: zlimitstatetextId.implicitWidth
            text: "Z Limit State = " + _msg;
            color: rootpageId.textcolor

            property string _msg: ""

            Component.onCompleted:
            {
                MicroControllerStatusPacket.zLimitSwitchChanged.connect(zlimitstatetextId.updateMsg);
            }

            function updateMsg()
            {
                switch(MicroControllerStatusPacket.zLimitSwitch)
                {
                case GaryLimitSwitch.TBI_LIMIT_SWITCH_STATE_OK:
                    _msg = "Ok";
                    break;
                case GaryLimitSwitch.TBI_LIMIT_SWITCH_STATE_TRIPPED:
                    _msg = "Tripped";
                    break;
                }
            }

        }
        //Control mode
        Text
        {
            id: controlmodetextId
            x: 20
            y: zlimitstatetextId.y + zlimitstatetextId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: controlmodetextId.implicitHeight
            width: controlmodetextId.implicitWidth
            text: "Control Mode = " + _msg;
            color: rootpageId.textcolor

            property string _msg: ""

            Component.onCompleted:
            {
                MicroControllerStatusPacket.controlModeChanged.connect(controlmodetextId.updateMsg);
            }


            function updateMsg()
            {
                switch(MicroControllerStatusPacket.controlMode)
                {
                case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                    _msg = "TBI_CONTROL_MODE_MANUAL_MODE";
                    break;
                case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                    _msg = "TBI_CONTROL_MODE_FULLAUTO_MODE";
                    break;
                case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                    _msg = "TBI_CONTROL_MODE_HEIGHTONLY";
                    break;
                case GaryControlMode.TBI_CONTROL_MODE_MOTORCALIBRATION:
                    _msg = "TBI_CONTROL_MODE_MOTORCALIBRATION";
                    break;
                }
            }


        }



        //-----------------------------------------------------------
        //Motor Calibration Button
        Rectangle
        {
            id: motorcalibrationselectionbuttonId
            x:20
            y:controlmodetextId.y + controlmodetextId.height + 10
            width: 600
            height: 75
            color: "transparent"
            border.width: rootpageId.rectborderwidths
            border.color: rootpageId.nonfocuscolor

            Text
            {
                id: motorcalibrationtextId
                anchors.centerIn: motorcalibrationselectionbuttonId
                font.family: fontId.name
                font.pointSize: 20
                height: motorcalibrationtextId.implicitHeight
                width: motorcalibrationtextId.implicitWidth
                text: "Perform Motor Calibration"
                color: rootpageId.textcolor
            }
        }
        Text
        {
            id: calibcyclestatustextId
            x:20
            y: motorcalibrationselectionbuttonId.y + motorcalibrationselectionbuttonId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: calibcyclestatustextId.implicitHeight
            width: calibcyclestatustextId.implicitWidth
            text: "Current Calibration Cycle = " + _msg
            color: rootpageId.textcolor

            property string _msg: ""

            Component.onCompleted:
            {
                MicroControllerStatusPacket.motorCalibrationCycleChanged.connect(calibcyclestatustextId.updateMsg);
            }

            function updateMsg()
            {
                switch(MicroControllerStatusPacket.motorCalibrationCycle)
                {
                case GaryMotorCalibrationCycleStatus.TBI_MOTORCAL_OFF:
                    _msg = "Calibration Not Running"
                    break;
                case GaryMotorCalibrationCycleStatus.TBI_MOTORCAL_GETPNT1PAUSE:
                    _msg = "Aquiring Point 1";
                    break;
                case GaryMotorCalibrationCycleStatus.TBI_MOTORCAL_MAKINGFIRSTMOVE:
                    _msg = "Making First Move";
                    break;
                case GaryMotorCalibrationCycleStatus.TBI_MOTORCAL_GETPNT2PAUSE:
                    _msg = "Aquiring Point 2";
                    break;
                case GaryMotorCalibrationCycleStatus.TBI_MOTORCAL_FINISHING:
                    _msg = "Finishing Calibration.";
                    break;
                }
            }

        }

        Text
        {
            id: xstepsperpixeltextId
            x:20
            y: calibcyclestatustextId.y + calibcyclestatustextId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: xstepsperpixeltextId.implicitHeight
            width: xstepsperpixeltextId.implicitWidth
            text: "X Steps Per Pixel = " + MotionControlParams.xStepsPerPixel
            color: rootpageId.textcolor
        }
        Text
        {
            id: zstepsperpixeltextId
            x: 20
            y: xstepsperpixeltextId.y + xstepsperpixeltextId.height + 1
            font.family: fontId.name
            font.pointSize: 15
            height: zstepsperpixeltextId.implicitHeight
            width: zstepsperpixeltextId.implicitWidth
            text: "Z Steps Per Pixel = " + MotionControlParams.zStepsPerPixel
            color: rootpageId.textcolor
        }

        //Record Track To Point
        Rectangle
        {
            id: recordtracktobuttonId
            x:20
            y:zstepsperpixeltextId.y + zstepsperpixeltextId.height + 20
            width: 600
            height: 75
            color: "transparent"
            border.width: rootpageId.rectborderwidths
            border.color: rootpageId.nonfocuscolor

            Text
            {
                id: recordtracktotextId
                anchors.centerIn: recordtracktobuttonId
                font.family: fontId.name
                font.pointSize: 20
                height: recordtracktotextId.implicitHeight
                width: recordtracktotextId.implicitWidth
                text: "Set Track To Point"
                color: rootpageId.textcolor
            }
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
                    break;
                case rootpageId.preblurframe:
                    Max.newPreBlurMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.erodeframe:
                    Max.newErodeMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.edgeframe:
                    Max.newEdgeMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.postblurframe:
                    Max.newPostBlurMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.thresholdframe:
                    Max.newThresholdMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.pixelcolumnframe:
                    Max.newPixelColumnMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.inlierdatasetframe:
                    Max.newInlierDataSetMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.ransacframe:
                    Max.newRansacMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.geoconstructionframe:
                    Max.newGeoConstructionMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
                    break;
                case rootpageId.operationframe:
                    Max.newOperationMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
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
                    break;
                case rootpageId.preblurframe:
                    Max.newPreBlurMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.preblurframe;
                    break;
                case rootpageId.erodeframe:
                    Max.newErodeMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.erodeframe;
                    break;
                case rootpageId.edgeframe:
                    Max.newEdgeMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.edgeframe;
                    break;
                case rootpageId.postblurframe:
                    Max.newPostBlurMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.postblurframe;
                    break;
                case rootpageId.thresholdframe:
                    Max.newThresholdMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.thresholdframe;
                    break;
                case rootpageId.pixelcolumnframe:
                    Max.newPixelColumnMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.pixelcolumnframe;
                    break;
                case rootpageId.inlierdatasetframe:
                    Max.newInlierDataSetMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.inlierdatasetframe;
                    break;
                case rootpageId.ransacframe:
                    Max.newRansacMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.ransacframe;
                    break;
                case rootpageId.geoconstructionframe:
                    Max.newGeoConstructionMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.geoconstructionframe;
                    break;
                case rootpageId.operationframe:
                    Max.newOperationMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.operationframe;
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
            mainviewrect_privateId.detachMaxSignal();
            mainviewrect_privateId.attachMaxSignal(mainviewrectId.displayed_frame);
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
                width: PylonCameraParameters.aoiWidth * scalex
                height: PylonCameraParameters.aoiHeight * scaley
                border.color: Qt.rgba(.5,0,0,1);
                border.width: 2
                readonly property real scalex: mainviewdisplayId.width/PylonCameraParameters.maxWidth
                readonly property real  scaley: mainviewdisplayId.height/PylonCameraParameters.maxHeight
            }
        }
    }

    //Settings Text
    Text
    {
        id: pipelinestatustextId
        font.family: fontId.name
        y: 300
        x:40
        text: "Pipeline Status:" + ProcessingPipeLineStatus.statusString
        color: "red"
        font.pixelSize: 25
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

    //Still Image File Open Dialog
    StillImageFileOpenDialog
    {
        id: stillimagefiledialogId
        visible: false

        onVisibleChanged:
        {
            if(stillimagefiledialogId.visible == true)
            {
                stillimagefiledialogId.grabFocus();
            }
            else
            {
                camerasettingsrectId.grabFocus();
                rootpageId.triggerTobyNextFrame();
            }

        }


    }

}
