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
    readonly property string blurstateframeactive: "BlurStateFrameActive"
    readonly property string blurstateframenotactive: "BlurStateFrameNotActive"
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
    readonly property string blurframe: "2-Blur"
    readonly property string thresholdframe: "3-Threshold"
    readonly property string pixelcolumnframe: "4-Gausian Declustering"
    readonly property string ransacframe: "5-Inlier Ransacs"
    readonly property string inlierdatasetframe: "6-Inlier DataSets"
    readonly property string geoconstructionframe: "7-Geo Constructions"
    readonly property string operationframe: "8-Operations"

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
        Toby.closeStillImagetoProcess();

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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
        State //Blur Selected Frame Focused
        {
            name: rootpageId.blurstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: true;}
            PropertyChanges{ target: thresholdsettingsrectId; visible: false;}
            PropertyChanges{ target: pixelcolumnsettingsrectId; visible: false;}
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: blursettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
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
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.nonfocuscolor}
            PropertyChanges{ target: blursettingsborderrectId; border.color: rootpageId.focuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: thresholdsettingscontrollerId; focus: false}
            PropertyChanges{target: camerasettingscontrollerId; focus: false}
            PropertyChanges{target: frameselectcontrollerId; focus: false}
            PropertyChanges{target: pixelcolumncontrollerId; focus: false}
            PropertyChanges{target: inlierdatasetcontrollerId; focus: false}
            PropertyChanges{target: ransaccontrollerId; focus: false}
            PropertyChanges{target: geoconstructioncontrollerId; focus: false}
            PropertyChanges{target: operationcontrollerId; focus: false}
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
            PropertyChanges{ target: inlierdatasetsettingsrectId; visible: false;}
            PropertyChanges{ target: ransacsettingsrectId; visible: false;}
            PropertyChanges{ target: geoconstructionsettingsrectId; visible: false}
            PropertyChanges{ target: operationsettingsrectId; visible: false}
            //Set The Highlighted Rect
            PropertyChanges{ target: frameselectborderrectId; border.color: rootpageId.focuscolor}
            PropertyChanges{ target: thresholdsettingsborderrectId; border.color: rootpageId.nonfocuscolor}
            //SetController focus To FrameSele
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges {target: ransacsettingsrectId; state: "RansacLeftTSLidealangle";}
        },
        State //Split Merge Selected Frame Focused
        {
            name: rootpageId.geoconstructionstateframeactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
        State //Split Merge Selected and Focused
        {
            name: rootpageId.geoconstructionstateframenotactive
            //Set Displayed Setting Rect
            PropertyChanges{ target: camerasettingsrectId; visible: false;}
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
            PropertyChanges{ target: blursettingsrectId; visible: false;}
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
            PropertyChanges{target: blursettingscontrollerId; focus: false}
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
                case rootpageId.blurframe:
                    rootpageId.state = rootpageId.blurstateframenotactive;
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
                        case rootpageId.blurframe:
                            Max.newBlurMatProcessed.connect(delegateframeId.recieveCVMat);
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
                modelId.append({"frame": rootpageId.blurframe});
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
                case rootpageId.blurframe:
                    rootpageId.state = rootpageId.blurstateframeactive;
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
            messagetext: "Blur: " + VGrooveGausianDistroParameters.blurValue;
            valuefrom: 1
            valueto: 49
            stepsize: 2
            majorstepsize: 4
            controlstickautorepeat: true

            Component.onCompleted:
            {
                blursliderId.value = VGrooveGausianDistroParameters.blurValue;
            }

            onEndFocus:
            {
                blursettingsrectId.grabFocus();
                blursliderId.state = blursliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveGausianDistroParameters.blurValue = blursliderId.value;
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

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacLeftTSLidealangle"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.highlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacLeftTSLallowedanglevariance"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.highlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}

            },
            State
            {
                name: "RansacLeftTSLMinVotes"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.highlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacLeftTSLDistanceThreshold"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.highlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacLeftTSLIterations"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.highlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightTSLidealangle"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.highlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightTSLallowedanglevariance"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.highlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightTSLMinVotes"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.highlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightTSLDistanceThreshold"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.highlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightTSLIterations"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.highlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacLeftBWLidealangle"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.highlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacLeftBWLallowedanglevariance"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.highlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacLeftBWLMinVotes"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.highlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacLeftBWLDistanceThreshold"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.highlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacLeftBWLIterations"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.highlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightBWLidealangle"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.highlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightBWLallowedanglevariance"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.highlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightBWLMinVotes"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.highlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightBWLDistanceThreshold"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlminvotessliderId; state: ransacrightbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwldistancethresholdsliderId; state: ransacrightbwldistancethresholdsliderId.highlightedstate;}
                PropertyChanges{target: ransacrightbwliterationssliderId; state: ransacrightbwliterationssliderId.nothighlightedstate;}
            },
            State
            {
                name: "RansacRightBWLIterations"
                PropertyChanges{target: ransacborderrectId; border.color:rootpageId.focuscolor;}

                PropertyChanges{target: ransaclefttslidealanglesliderId; state: ransaclefttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslallowedanglevariancesliderId; state: ransaclefttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttslminvotessliderId; state: ransaclefttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsldistancethresholdsliderId; state: ransaclefttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransaclefttsliterationssliderId; state: ransaclefttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrighttslidealanglesliderId; state: ransacrighttslidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslallowedanglevariancesliderId; state: ransacrighttslallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttslminvotessliderId; state: ransacrighttslminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsldistancethresholdsliderId; state: ransacrighttsldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrighttsliterationssliderId; state: ransacrighttsliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacleftbwlidealanglesliderId; state: ransacleftbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlallowedanglevariancesliderId; state: ransacleftbwlallowedanglevariancesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwlminvotessliderId; state: ransacleftbwlminvotessliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwldistancethresholdsliderId; state: ransacleftbwldistancethresholdsliderId.nothighlightedstate;}
                PropertyChanges{target: ransacleftbwliterationssliderId; state: ransacleftbwliterationssliderId.nothighlightedstate;}

                PropertyChanges{target: ransacrightbwlidealanglesliderId; state: ransacrightbwlidealanglesliderId.nothighlightedstate;}
                PropertyChanges{target: ransacrightbwlallowedanglevariancesliderId; state: ransacrightbwlallowedanglevariancesliderId.nothighlightedstate;}
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
                case "RansacLeftTSLidealangle":
                    ransaclefttslidealanglesliderId.grabFocus();
                    break;
                case "RansacLeftTSLallowedanglevariance":
                    ransaclefttslallowedanglevariancesliderId.grabFocus();
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


                case "RansacRightTSLidealangle":
                    ransacrighttslidealanglesliderId.grabFocus();
                    break;
                case "RansacRightTSLallowedanglevariance":
                    ransacrighttslallowedanglevariancesliderId.grabFocus();
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


                case "RansacLeftBWLidealangle":
                    ransacleftbwlidealanglesliderId.grabFocus();
                    break;
                case "RansacLeftBWLallowedanglevariance":
                    ransacleftbwlallowedanglevariancesliderId.grabFocus();
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


                case "RansacRightBWLidealangle":
                    ransacrightbwlidealanglesliderId.grabFocus();
                    break;
                case "RansacRightBWLallowedanglevariance":
                    ransacrightbwlallowedanglevariancesliderId.grabFocus();
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
                //Allowed Angle Variance Slider
                //Distance Threshold
                //Iterations
                //Min Votes
                switch(ransacsettingsrectId.state)
                {
                case "RansacLeftTSLidealangle":
                    ransacsettingsrectId.state = "NonFocused";
                    rootpageId.state = rootpageId.ransacstateframeactive;
                    break;
                case "RansacLeftTSLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacLeftTSLidealangle";
                    break;
                case "RansacLeftTSLMinVotes":
                    ransacsettingsrectId.state = "RansacLeftTSLIterations";
                    break;
                case "RansacLeftTSLDistanceThreshold":
                    ransacsettingsrectId.state = "RansacLeftTSLallowedanglevariance";
                    break;
                case "RansacLeftTSLIterations":
                    ransacsettingsrectId.state = "RansacLeftTSLDistanceThreshold"
                    break;

                case "RansacRightTSLidealangle":
                    ransacsettingsrectId.state = "NonFocused";
                    rootpageId.state = rootpageId.ransacstateframeactive;
                    break;
                case "RansacRightTSLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacRightTSLidealangle";
                    break;
                case "RansacRightTSLMinVotes":
                    ransacsettingsrectId.state = "RansacRightTSLIterations";
                    break;
                case "RansacRightTSLDistanceThreshold":
                    ransacsettingsrectId.state = "RansacRightTSLallowedanglevariance";
                    break;
                case "RansacRightTSLIterations":
                    ransacsettingsrectId.state = "RansacRightTSLDistanceThreshold";
                    break;


                case "RansacLeftBWLidealangle":
                    ransacsettingsrectId.state = "RansacLeftTSLMinVotes";
                    break;
                case "RansacLeftBWLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacLeftBWLidealangle";
                    break;
                case "RansacLeftBWLMinVotes":
                    ransacsettingsrectId.state = "RansacLeftBWLIterations";
                    break;
                case "RansacLeftBWLDistanceThreshold":
                    ransacsettingsrectId.state = "RansacLeftBWLallowedanglevariance";
                    break;
                case "RansacLeftBWLIterations":
                    ransacsettingsrectId.state = "RansacLeftBWLDistanceThreshold";
                    break;

                case "RansacRightBWLidealangle":
                    ransacsettingsrectId.state = "RansacRightTSLMinVotes";
                    break;
                case "RansacRightBWLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacRightBWLidealangle";
                    break;
                case "RansacRightBWLMinVotes":
                    ransacsettingsrectId.state = "RansacRightBWLIterations";
                    break;
                case "RansacRightBWLDistanceThreshold":
                    ransacsettingsrectId.state = "RansacRightBWLallowedanglevariance";
                    break;
                case "RansacRightBWLIterations":
                    ransacsettingsrectId.state = "RansacRightBWLDistanceThreshold"
                    break;
                }
            }

            onDownButtonPressed:
            {
                //Min Angle Slider
                //Allowed Angle Variance Slider
                //Distance Threshold
                //Iterations
                //Min Votes
                switch(ransacsettingsrectId.state)
                {
                case "RansacLeftTSLidealangle":
                    ransacsettingsrectId.state = "RansacLeftTSLallowedanglevariance";
                    break;
                case "RansacLeftTSLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacLeftTSLDistanceThreshold";
                    break;
                case "RansacLeftTSLMinVotes":
                    ransacsettingsrectId.state = "RansacLeftBWLidealangle";
                    break;
                case "RansacLeftTSLDistanceThreshold":
                    ransacsettingsrectId.state = "RansacLeftTSLIterations";
                    break;
                case "RansacLeftTSLIterations":
                    ransacsettingsrectId.state = "RansacLeftTSLMinVotes";
                    break;


                case "RansacRightTSLidealangle":
                    ransacsettingsrectId.state = "RansacRightTSLallowedanglevariance";
                    break;
                case "RansacRightTSLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacRightTSLDistanceThreshold";
                    break;
                case "RansacRightTSLMinVotes":
                    ransacsettingsrectId.state = "RansacRightBWLidealangle";
                    break;
                case "RansacRightTSLDistanceThreshold":
                    ransacsettingsrectId.state = "RansacRightTSLIterations";
                    break;
                case "RansacRightTSLIterations":
                    ransacsettingsrectId.state = "RansacRightTSLMinVotes";
                    break;


                case "RansacLeftBWLidealangle":
                    ransacsettingsrectId.state = "RansacLeftBWLallowedanglevariance";
                    break;
                case "RansacLeftBWLallowedanglevariance":
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

                case "RansacRightBWLidealangle":
                    ransacsettingsrectId.state = "RansacRightBWLallowedanglevariance";
                    break;
                case "RansacRightBWLallowedanglevariance":
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
                case "RansacLeftTSLidealangle":
                    break;
                case "RansacLeftTSLallowedanglevariance":
                    break;
                case "RansacLeftTSLMinVotes":
                    break;
                case "RansacLeftTSLDistanceThreshold":
                    break;
                case "RansacLeftTSLIterations":
                    break;


                case "RansacRightTSLidealangle":
                    ransacsettingsrectId.state = "RansacLeftTSLidealangle";
                    break;
                case "RansacRightTSLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacLeftTSLallowedanglevariance";
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


                case "RansacLeftBWLidealangle":
                    break;
                case "RansacLeftBWLallowedanglevariance":
                    break;
                case "RansacLeftBWLMinVotes":
                    break;
                case "RansacLeftBWLDistanceThreshold":
                    break;
                case "RansacLeftBWLIterations":
                    break;


                case "RansacRightBWLidealangle":
                    ransacsettingsrectId.state = "RansacLeftBWLidealangle";
                    break;
                case "RansacRightBWLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacLeftBWLallowedanglevariance";
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
                case "RansacLeftTSLidealangle":
                    ransacsettingsrectId.state = "RansacRightTSLidealangle";
                    break;
                case "RansacLeftTSLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacRightTSLallowedanglevariance";
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


                case "RansacRightTSLidealangle":
                    break;
                case "RansacRightTSLallowedanglevariance":
                    break;
                case "RansacRightTSLMinVotes":
                    break;
                case "RansacRightTSLDistanceThreshold":
                    break;
                case "RansacRightTSLIterations":
                    break;


                case "RansacLeftBWLidealangle":
                    ransacsettingsrectId.state = "RansacRightBWLidealangle";
                    break;
                case "RansacLeftBWLallowedanglevariance":
                    ransacsettingsrectId.state = "RansacRightBWLallowedanglevariance";
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


                case "RansacRightBWLidealangle":
                    break;
                case "RansacRightBWLallowedanglevariance":
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
            text: "Inlier Ransac Settings"
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
        //Left TSL Ideal Angle Slider
        SliderSettingsObject
        {
            id: ransaclefttslidealanglesliderId
            x:ransacsettingsrectId.sliderleftx
            y: ransactextleftId.y+ransactextleftId.height+5
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "Ideal Angle: " + VGrooveLeftTSLRansacParameters.idealAngle
            valuefrom: -90
            valueto: 90
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth


            Component.onCompleted:
            {
                ransaclefttslidealanglesliderId.value = VGrooveLeftTSLRansacParameters.idealAngle;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransaclefttslidealanglesliderId.state = ransaclefttslidealanglesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftTSLRansacParameters.idealAngle = ransaclefttslidealanglesliderId.value;
            }
        }

        //Left TSL Allowed Angle Variance Slider
        SliderSettingsObject
        {
            id: ransaclefttslallowedanglevariancesliderId
            x: ransacsettingsrectId.sliderleftx
            y: ransaclefttslidealanglesliderId.y + ransaclefttslidealanglesliderId.height + ransacsettingsrectId.slidersverticalspacing
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "Angle Variance: " + VGrooveLeftTSLRansacParameters.allowedAngleVariance
            valuefrom: 0
            valueto: 30
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {

                ransaclefttslallowedanglevariancesliderId.value = VGrooveLeftTSLRansacParameters.allowedAngleVariance;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransaclefttslallowedanglevariancesliderId.state = ransaclefttslallowedanglevariancesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftTSLRansacParameters.allowedAngleVariance = ransaclefttslallowedanglevariancesliderId.value;
            }
        }

        //Left TSL Distance Threshold
        SliderSettingsObject
        {

            id: ransaclefttsldistancethresholdsliderId
            x:ransacsettingsrectId.sliderleftx
            y: ransacsettingsrectId.slidersverticalspacing + ransaclefttslallowedanglevariancesliderId.y + ransaclefttslallowedanglevariancesliderId.height
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "TSL Distance Threshold: " + VGrooveLeftTSLRansacParameters.distanceThreshold
            valuefrom: 1
            valueto: 10
            stepsize: 1
            majorstepsize: 1
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransaclefttsldistancethresholdsliderId.value = VGrooveLeftTSLRansacParameters.distanceThreshold;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransaclefttsldistancethresholdsliderId.state = ransaclefttsldistancethresholdsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftTSLRansacParameters.distanceThreshold = ransaclefttsldistancethresholdsliderId.value;
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
            messagetext: "TSL Iterations: " + VGrooveLeftTSLRansacParameters.iterations
            valuefrom: 20
            valueto: 1000
            stepsize: 1
            majorstepsize: 50
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransaclefttsliterationssliderId.value = VGrooveLeftTSLRansacParameters.iterations;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransaclefttsliterationssliderId.state = ransaclefttsliterationssliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftTSLRansacParameters.iterations = ransaclefttsliterationssliderId.value;
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
            messagetext: "TSL Min Votes: " + VGrooveLeftTSLRansacParameters.minVotes
            valuefrom: 1
            valueto: 600
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransaclefttslminvotessliderId.value = VGrooveLeftTSLRansacParameters.minVotes
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransaclefttslminvotessliderId.state = ransaclefttslminvotessliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftTSLRansacParameters.minVotes = ransaclefttslminvotessliderId.value;
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //Left BWL-----------------------------------------
        //Left BWL Ideal Angle Slider
        SliderSettingsObject
        {
            id: ransacleftbwlidealanglesliderId
            x:ransacsettingsrectId.sliderleftx
            y: 430
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "Ideal Angle: " + VGrooveLeftBWLRansacParameters.idealAngle;
            valuefrom: -90
            valueto: 90
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacleftbwlidealanglesliderId.value = VGrooveLeftBWLRansacParameters.idealAngle
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacleftbwlidealanglesliderId.state = ransacleftbwlidealanglesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftBWLRansacParameters.idealAngle = ransacleftbwlidealanglesliderId.value;
            }
        }

        //Left BWL Allowed Angle Variance Slider
        SliderSettingsObject
        {
            id: ransacleftbwlallowedanglevariancesliderId
            x: ransacsettingsrectId.sliderleftx
            y: ransacleftbwlidealanglesliderId.y + ransacleftbwlidealanglesliderId.height + ransacsettingsrectId.slidersverticalspacing
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "Angle Variance: " + VGrooveLeftBWLRansacParameters.allowedAngleVariance;
            valuefrom: 0
            valueto: 30
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacleftbwlallowedanglevariancesliderId.value = VGrooveLeftBWLRansacParameters.allowedAngleVariance;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacleftbwlallowedanglevariancesliderId.state = ransacleftbwlallowedanglevariancesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftBWLRansacParameters.allowedAngleVariance = ransacleftbwlallowedanglevariancesliderId.value;
            }
        }

        //Left BWL Distance Threshold
        SliderSettingsObject
        {

            id: ransacleftbwldistancethresholdsliderId
            x:ransacsettingsrectId.sliderleftx
            y: ransacsettingsrectId.slidersverticalspacing + ransacleftbwlallowedanglevariancesliderId.y + ransacleftbwlallowedanglevariancesliderId.height
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "BWL Distance Threshold: " + VGrooveLeftBWLRansacParameters.distanceThreshold;
            valuefrom: 1
            valueto: 10
            stepsize: 1
            majorstepsize: 1
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacleftbwldistancethresholdsliderId.value = VGrooveLeftBWLRansacParameters.distanceThreshold;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacleftbwldistancethresholdsliderId.state = ransacleftbwldistancethresholdsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftBWLRansacParameters.distanceThreshold = ransacleftbwldistancethresholdsliderId.value;
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
            messagetext: "BWL Iterations: " + VGrooveLeftBWLRansacParameters.iterations
            valuefrom: 20
            valueto: 1000
            stepsize: 1
            majorstepsize: 50
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacleftbwliterationssliderId.value = VGrooveLeftBWLRansacParameters.iterations;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacleftbwliterationssliderId.state = ransacleftbwliterationssliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftBWLRansacParameters.iterations = ransacleftbwliterationssliderId.value;
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
            messagetext: "BWL Min Votes: " + VGrooveLeftBWLRansacParameters.minVotes
            valuefrom: 1
            valueto: 600
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacleftbwlminvotessliderId.value = VGrooveLeftBWLRansacParameters.minVotes;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacleftbwlminvotessliderId.state = ransacleftbwlminvotessliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveLeftBWLRansacParameters.minVotes = ransacleftbwlminvotessliderId.value;
            }
        }
        //--------------------------------------------------


        //right TSL-----------------------------------------
        //right TSL Ideal Angle Slider
        SliderSettingsObject
        {
            id: ransacrighttslidealanglesliderId
            x:ransacsettingsrectId.sliderrightx
            y: ransactextrightId.y+ransactextrightId.height+5
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "Ideal Angle: " + VGrooveRightTSLRansacParameters.idealAngle;
            valuefrom: -90
            valueto: 90
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrighttslidealanglesliderId.value = VGrooveRightTSLRansacParameters.idealAngle;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacrighttslidealanglesliderId.state = ransacrighttslidealanglesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveRightTSLRansacParameters.idealAngle = ransacrighttslidealanglesliderId.value;
            }
        }

        //right TSL Allowed Angle Variance Slider
        SliderSettingsObject
        {
            id: ransacrighttslallowedanglevariancesliderId
            x: ransacsettingsrectId.sliderrightx
            y: ransacrighttslidealanglesliderId.y + ransacrighttslidealanglesliderId.height + ransacsettingsrectId.slidersverticalspacing
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "Angle Variance: " + VGrooveRightTSLRansacParameters.allowedAngleVariance;
            valuefrom: 0
            valueto: 30
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrighttslallowedanglevariancesliderId.value = VGrooveRightTSLRansacParameters.allowedAngleVariance;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacrighttslallowedanglevariancesliderId.state = ransacrighttslallowedanglevariancesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveRightTSLRansacParameters.allowedAngleVariance = ransacrighttslallowedanglevariancesliderId.value;
            }
        }

        //right TSL Distance Threshold
        SliderSettingsObject
        {

            id: ransacrighttsldistancethresholdsliderId
            x:ransacsettingsrectId.sliderrightx
            y: ransacsettingsrectId.slidersverticalspacing + ransacrighttslallowedanglevariancesliderId.y + ransacrighttslallowedanglevariancesliderId.height
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "TSL Distance Threshold: " + VGrooveRightTSLRansacParameters.distanceThreshold;
            valuefrom: 1
            valueto: 10
            stepsize: 1
            majorstepsize: 1
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrighttsldistancethresholdsliderId.value = VGrooveRightTSLRansacParameters.distanceThreshold;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacrighttsldistancethresholdsliderId.state = ransacrighttsldistancethresholdsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveRightTSLRansacParameters.distanceThreshold = ransacrighttsldistancethresholdsliderId.value;
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
            messagetext: "TSL Iterations: " + VGrooveRightTSLRansacParameters.iterations;
            valuefrom: 20
            valueto: 1000
            stepsize: 1
            majorstepsize: 50
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrighttsliterationssliderId.value = VGrooveRightTSLRansacParameters.iterations;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacrighttsliterationssliderId.state = ransacrighttsliterationssliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveRightTSLRansacParameters.iterations = ransacrighttsliterationssliderId.value;
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
            messagetext: "TSL Min Votes: " + VGrooveRightTSLRansacParameters.minVotes;
            valuefrom: 1
            valueto: 600
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrighttslminvotessliderId.value = VGrooveRightTSLRansacParameters.minVotes;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
            }

            onValueChanged:
            {
                VGrooveRightTSLRansacParameters.minVotes = ransacrighttslminvotessliderId.value;
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //--------------------------------------------------
        //right BWL-----------------------------------------
        //right BWL Ideal Angle Slider
        SliderSettingsObject
        {
            id: ransacrightbwlidealanglesliderId
            x:ransacsettingsrectId.sliderrightx
            y: 430
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "BWL Ideal Angle: " + VGrooveRightBWLRansacParameters.idealAngle;
            valuefrom: -90
            valueto: 90
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrightbwlidealanglesliderId.value = VGrooveRightBWLRansacParameters.idealAngle;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacrightbwlidealanglesliderId.state = ransacrightbwlidealanglesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveRightBWLRansacParameters.idealAngle = ransacrightbwlidealanglesliderId.value;
            }
        }

        //right BWL Allowed Angle Variance Slider
        SliderSettingsObject
        {
            id: ransacrightbwlallowedanglevariancesliderId
            x: ransacsettingsrectId.sliderrightx
            y: ransacrightbwlidealanglesliderId.y + ransacrightbwlidealanglesliderId.height + ransacsettingsrectId.slidersverticalspacing
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "Angle Variance: " + VGrooveRightBWLRansacParameters.allowedAngleVariance
            valuefrom: 0
            valueto: 30
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrightbwlallowedanglevariancesliderId.value = VGrooveRightBWLRansacParameters.allowedAngleVariance;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacrightbwlallowedanglevariancesliderId.state = ransacrightbwlallowedanglevariancesliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveRightBWLRansacParameters.allowedAngleVariance = ransacrightbwlallowedanglevariancesliderId.value;
            }
        }

        //right BWL Distance Threshold
        SliderSettingsObject
        {

            id: ransacrightbwldistancethresholdsliderId
            x:ransacsettingsrectId.sliderrightx
            y: ransacsettingsrectId.slidersverticalspacing + ransacrightbwlallowedanglevariancesliderId.y + ransacrightbwlallowedanglevariancesliderId.height
            width: ransacsettingsrectId.sliderwidth
            height: ransacsettingsrectId.sliderheight
            messagetext: "BWL Distance Threshold: " + VGrooveRightBWLRansacParameters.distanceThreshold
            valuefrom: 1
            valueto: 10
            stepsize: 1
            majorstepsize: 1
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrightbwldistancethresholdsliderId.value = VGrooveRightBWLRansacParameters.distanceThreshold;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacrightbwldistancethresholdsliderId.state = ransacrightbwldistancethresholdsliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveRightBWLRansacParameters.distanceThreshold = ransacrightbwldistancethresholdsliderId.value;
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
            messagetext: "BWL Iterations: " + VGrooveRightBWLRansacParameters.iterations;
            valuefrom: 20
            valueto: 1000
            stepsize: 1
            majorstepsize: 50
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrightbwliterationssliderId.value = VGrooveRightBWLRansacParameters.iterations;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacrightbwliterationssliderId.state = ransacrightbwliterationssliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveRightBWLRansacParameters.iterations = ransacrightbwliterationssliderId.value;
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
            messagetext: "BWL Min Votes: " + VGrooveRightBWLRansacParameters.minVotes
            valuefrom: 1
            valueto: 600
            stepsize: 1
            majorstepsize: 5
            controlstickautorepeat: true;
            fontsize: ransacsettingsrectId.slidertextsize
            highlightbordersize: ransacsettingsrectId.sliderborderwidth

            Component.onCompleted:
            {
                ransacrightbwlminvotessliderId.value = VGrooveRightBWLRansacParameters.minVotes;
            }

            onEndFocus:
            {
                ransacsettingsrectId.grabFocus();
                ransacrightbwlminvotessliderId.state = ransacrightbwlminvotessliderId.highlightedstate;
            }

            onValueChanged:
            {
                VGrooveRightBWLRansacParameters.minVotes = ransacrightbwlminvotessliderId.value;
            }
        }
        //--------------------------------------------------

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
                    switch(MicroControllerStatusPacket.controlMode)
                    {
                    case GaryControlMode.TBI_CONTROL_MODE_MANUAL_MODE:
                        Gary.sendStopMovement();
                        operationsettingsrectId.grabFocus();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_FULLAUTO_MODE:
                        Gary.setControlModeToManual();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_HEIGHTONLY:
                        Gary.setControlModeToManual();
                        break;
                    case GaryControlMode.TBI_CONTROL_MODE_MOTORCALIBRATION:
                        break;
                    }
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
                case rootpageId.blurframe:
                    Max.newBlurMatProcessed.disconnect(mainviewdisplayId.recieveCVMat);
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
                case rootpageId.blurframe:
                    Max.newBlurMatProcessed.connect(mainviewdisplayId.recieveCVMat);
                    mainviewrect_privateId.attached_frame = rootpageId.blurframe;
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
