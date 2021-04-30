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
    readonly property string pagename: ""

    property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string backgroundimage: "qrc:/Icons/dark_steel_texture.jpg"
    property real backgroundopacity: 1
    property real controlsopacity: 1
    property color textcolor: Qt.rgba(1,1,.95,1)


    //Signals--------------------------------------------------------
    signal destroyPage()
    signal aboutToDestroy()
    signal completed()

    //Misc Functions-------------------------------------------------
    function grabFocus()
    {
        controllerId.focus = true;
    }

    //Slots
    Component.onCompleted:
    {
        //Connect the Application Singletons to the QML Objects.
        rootpageId.completed();
    }

    //OML Components------------------------------------------------
    ControllerObject
    {

        id:controllerId
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

    //Font for UI
    FontLoader
    {
        id: fontId
        source: rootpageId.fontsource
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

    //Slider Template
    SliderSettingsObject
    {
        id: templatesliderId
        controlname: "MinThresholdSlider"
        x:20
        y:60
        messagetext: "Text Message";
        valuefrom: 0
        valueto: 100
        stepsize: 1
        majorstepsize: 5


        Component.onCompleted:
        {
            // templatesliderId.value = Some Init Function

        onEndFocus:
        {
            // grabFocus of Root Rectangle
            templatesliderId.state = templatesliderId.highlightedstate;
        }

        onValueChanged:
        {
            //Update Mary With Value
        }
    }



}


}
