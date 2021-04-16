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


    //For Rectangle Placements
    readonly property color focuscolor: Qt.rgba(0,1,0,.6)
    readonly property color nonfocuscolor: Qt.rgba(1,1,1,.1)
    readonly property int rectborderwidths: 5
    readonly property int rectpadding: 20

    readonly property int frameselectrectx: 20
    readonly property int frameselectrecty: 60
    readonly property int frameselectrectheight: 248
    readonly property int frameselectrectwidth: rootpageId.width-2*rootpageId.rectpadding


    readonly property int settingsrectheight: 730
    readonly property int settingsrectwidth: 1000
    readonly property int settingsrectx: rootpageId.width - rootpageId.settingsrectwidth - rootpageId.rectpadding
    readonly property int settingsrecty: rootpageId.height - rootpageId.settingsrectheight - rootpageId.rectpadding

    readonly property int mainviewheight: 540 + rootpageId.rectpadding
    readonly property int mainviewwidth: 720 + rootpageId.rectpadding
    readonly property int mainviewrectx: rootpageId.frameselectrectx
    readonly property int mainviewrecty: rootpageId.settingsrecty


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
        rootpageId.completed();
        console.log("QML: ImageProcessingSettingPage Created.");
    }

    //OML Components------------------------------------------------

    //Font for UI
    FontLoader
    {
        id: fontId
        source: rootpageId.fontsource
    }

    //Background Solid
    Rectangle
    {
        anchors.fill: rootpageId
        color: "black"
    }

    //Background Image
    Image
    {
        focus: false
        id: backgroundimageId
        source: rootpageId.backgroundimage
        anchors.fill: parent
        opacity: rootpageId.backgroundopacity
    }

    //Top Text
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

    //Frame Selection Rectangle
    Rectangle
    {
        id: frameselectionrectId
        x:rootpageId.frameselectrectx
        y:rootpageId.frameselectrecty
        height:rootpageId.frameselectrectheight
        width: rootpageId.frameselectrectwidth
        color: "transparent"

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
            id: frameselectwindowrectId
            anchors.fill: frameselectionrectId
            color: "transparent"
            border.width: rootpageId.rectborderwidths
            border.color: rootpageId.nonfocuscolor
            radius: 10
        }


        ControllerObject
        {

            id:frameselectcontrollerId
            focus: false

            onFocusChanged:
            {
                switch(frameselectcontrollerId.focus)
                {
                case true:
                    frameselectwindowrectId.border.color = rootpageId.focuscolor;
                    break;
                case false:
                    frameselectwindowrectId.border.color = rootpageId.nonfocuscolor;
                    break;
                }
            }

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
                camerasettingscontrollerId.focus = true;
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

        //ListModel
        ListModel
        {
            id: modelId

            ListElement
            {
                frame: "Camera"
            }

            ListElement
            {
                frame:"Blur"
            }

            ListElement
            {
                frame: "Threshold"
            }

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
                    height:200

                    //Handle Max Signal/Slot Connections
                    Component.onCompleted:
                    {
                        switch(frame)
                        {
                        case "Camera":
                            Max.newRawMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case "Blur":
                            Max.newBlurMatProcessed.connect(delegateframeId.recieveCVMat);
                            break;
                        case "Threshold":
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

        }
    }

    //Camera Settings Rectangle
    Rectangle
    {
        id: camerasettingsrectId
        width: rootpageId.settingsrectwidth
        height: rootpageId.settingsrectheight
        x: rootpageId.settingsrectx
        y: rootpageId.settingsrecty
        color: "transparent"

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

        ControllerObject
        {

            id:camerasettingscontrollerId
            focus: false

            onFocusChanged:
            {
                switch(camerasettingscontrollerId.focus)
                {
                case true:
                    camerasettingsborderrectId.border.color = rootpageId.focuscolor;
                    break;
                case false:
                    camerasettingsborderrectId.border.color = rootpageId.nonfocuscolor;
                }
            }

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
                frameselectcontrollerId.focus = true;
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

    }

    //Main View Rectangle
    Rectangle
    {
        id: mainviewrectId
        width: rootpageId.mainviewwidth
        height: rootpageId.mainviewheight
        x: rootpageId.mainviewrectx
        y: rootpageId.mainviewrecty

        property string current_frame
        function changeFrame ()
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
            width: mainviewrectId.width - rootpageId.rectpadding
            height: mainviewrectId.height - rootpageId.rectpadding
        }
    }


    //Graphical Key
    ControllerKeyObject
    {
        id: controlkeyId
        greenbuttonmessage: "Select"
        redbuttonmessage: "Back"
        enableblackbutton: false
    }


}
