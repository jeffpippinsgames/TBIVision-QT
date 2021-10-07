import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"
import tbi.vision.components 1.0


Rectangle {

    id: rootId
    width: 360
    height: 300
    color: rootId.nonhighlightcolor
    border.width: rootId.sliderborderwidth
    border.color: rootId.nonfocuscolor


    property string titletext: "Ransac Parameters"
    property color titletextcolor: Qt.rgba(224/255,252/255,13/255,1)
    property int titletextsize: 20


    property real idealangle_fromvalue: -90
    property real idealangle_tovalue: 90
    property real idealangle_stepsize: 1
    property real idealangle_majorstepsize: 5
    property bool idealangle_controlstickautorepeat: true

    property real anglevariance_fromvalue: 0
    property real anglevariance_tovalue: 30
    property real anglevariance_stepsize: 1
    property real anglevariance_majorstepsize: 5
    property bool anglevariance_controlstickautorepeat: true

    property real distance_fromvalue: 1
    property real distance_tovalue: 50
    property real distance_stepsize: 1
    property real distance_majorstepsize: 1
    property bool distance_controlstickautorepeat: true

    property real iterations_fromvalue: 20
    property real iterations_tovalue: 1000
    property real iterations_stepsize: 1
    property real iterations_majorstepsize: 50
    property bool iterations_controlstickautorepeat: true

    property real minvotes_fromvalue: 20
    property real minvotes_tovalue: 1000
    property real minvotes_stepsize: 1
    property real minvotes_majorstepsize: 50
    property bool minvotes_controlstickautorepeat: true

    property TBIRansacParameter ransacparameter

    readonly property color focuscolor: Qt.rgba(0,1,0,.6)
    readonly property color nonfocuscolor: Qt.rgba(1,1,1,.1)
    readonly property color highlightcolor: Qt.rgba(0,.2,0,.5)
    readonly property color nonhighlightcolor: "transparent"
    readonly property int sliderwidth: 340
    readonly property int sliderheight: 45
    readonly property int sliderborderwidth: 3
    readonly property int slidersverticalspacing: 5
    readonly property int sliderleftx: 10
    readonly property int sliderrightx: 410
    readonly property int slidertextsize: 15

    signal endFocus()

    function grabFocus()
    {
        controllerId.focus = true;
        rootId.state = "idealangle";
    }


    states:
        [
        State
        {
            name: "NonFocused"
            PropertyChanges{target:  rootId; border.color: rootId.focuscolor;}
            PropertyChanges{target:  rootId; color: rootId.nonhighlightcolor;}
            PropertyChanges{target:  idealanglesliderId; state:  idealanglesliderId.nothighlightedstate;}
            PropertyChanges{target:  allowedanglevariancesliderId; state:  allowedanglevariancesliderId.nothighlightedstate;}
            PropertyChanges{target:  minvotessliderId; state:  minvotessliderId.nothighlightedstate;}
            PropertyChanges{target:  distancethresholdsliderId; state:  distancethresholdsliderId.nothighlightedstate;}
            PropertyChanges{target:  iterationssliderId; state:  iterationssliderId.nothighlightedstate;}
        },
        State
        {
            name: "idealangle"
            PropertyChanges{target:  rootId; border.color: rootId.focuscolor;}
            PropertyChanges{target:  rootId; color: rootId.highlightcolor;}
            PropertyChanges{target:  idealanglesliderId; state:  idealanglesliderId.highlightedstate;}
            PropertyChanges{target:  allowedanglevariancesliderId; state:  allowedanglevariancesliderId.nothighlightedstate;}
            PropertyChanges{target:  minvotessliderId; state:  minvotessliderId.nothighlightedstate;}
            PropertyChanges{target:  distancethresholdsliderId; state:  distancethresholdsliderId.nothighlightedstate;}
            PropertyChanges{target:  iterationssliderId; state:  iterationssliderId.nothighlightedstate;}
        },
        State
        {
            name: "allowedanglevariance"

            PropertyChanges{target:  rootId; border.color: rootId.focuscolor;}
            PropertyChanges{target:  rootId; color: rootId.highlightcolor;}
            PropertyChanges{target:  idealanglesliderId; state:  idealanglesliderId.nothighlightedstate;}
            PropertyChanges{target:  allowedanglevariancesliderId; state:  allowedanglevariancesliderId.highlightedstate;}
            PropertyChanges{target:  minvotessliderId; state:  minvotessliderId.nothighlightedstate;}
            PropertyChanges{target:  distancethresholdsliderId; state:  distancethresholdsliderId.nothighlightedstate;}
            PropertyChanges{target:  iterationssliderId; state:  iterationssliderId.nothighlightedstate;}
        },
        State
        {
            name: "MinVotes"

            PropertyChanges{target:  rootId; border.color: rootId.focuscolor;}
            PropertyChanges{target:  rootId; color: rootId.highlightcolor;}
            PropertyChanges{target:  idealanglesliderId; state:  idealanglesliderId.nothighlightedstate;}
            PropertyChanges{target:  allowedanglevariancesliderId; state:  allowedanglevariancesliderId.nothighlightedstate;}
            PropertyChanges{target:  minvotessliderId; state:  minvotessliderId.highlightedstate;}
            PropertyChanges{target:  distancethresholdsliderId; state:  distancethresholdsliderId.nothighlightedstate;}
            PropertyChanges{target:  iterationssliderId; state:  iterationssliderId.nothighlightedstate;}
        },
        State
        {
            name: "DistanceThreshold"

            PropertyChanges{target:  rootId; border.color: rootId.focuscolor;}
            PropertyChanges{target:  rootId; color: rootId.highlightcolor;}
            PropertyChanges{target:  idealanglesliderId; state:  idealanglesliderId.nothighlightedstate;}
            PropertyChanges{target:  allowedanglevariancesliderId; state:  allowedanglevariancesliderId.nothighlightedstate;}
            PropertyChanges{target:  minvotessliderId; state:  minvotessliderId.nothighlightedstate;}
            PropertyChanges{target:  distancethresholdsliderId; state:  distancethresholdsliderId.highlightedstate;}
            PropertyChanges{target:  iterationssliderId; state:  iterationssliderId.nothighlightedstate;}
        },
        State
        {
            name: "Iterations"

            PropertyChanges{target:  rootId; border.color: rootId.focuscolor;}
            PropertyChanges{target:  rootId; color: rootId.highlightcolor;}
            PropertyChanges{target:  idealanglesliderId; state:  idealanglesliderId.nothighlightedstate;}
            PropertyChanges{target:  allowedanglevariancesliderId; state:  allowedanglevariancesliderId.nothighlightedstate;}
            PropertyChanges{target:  minvotessliderId; state:  minvotessliderId.nothighlightedstate;}
            PropertyChanges{target:  distancethresholdsliderId; state:  distancethresholdsliderId.nothighlightedstate;}
            PropertyChanges{target:  iterationssliderId; state:  iterationssliderId.highlightedstate;}
        }
    ]


    ControllerObject
    {

        id: controllerId
        focus: false

        onFocusChanged:
        {

        }

        onBlackButtonPressed:
        {
            rootId.state = "NonFocused";
            rootId.endFocus();
        }

        onGreenButtonPressed:
        {
            switch( rootId.state)
            {
            case "idealangle":
                 idealanglesliderId.grabFocus();
                break;
            case "allowedanglevariance":
                 allowedanglevariancesliderId.grabFocus();
                break;
            case "MinVotes":
                 minvotessliderId.grabFocus();
                break;
            case "DistanceThreshold":
                 distancethresholdsliderId.grabFocus();
                break;
            case "Iterations":
                 iterationssliderId.grabFocus();
                break;
            }

        }

        onRedButtonPressed:
        {
            rootId.state = "NonFocused";
            rootId.endFocus();
        }

        onUpButtonPressed:
        {

            switch( rootId.state)
            {
            case "idealangle":
                 rootId.state = "MinVotes";
                break;
            case "allowedanglevariance":
                 rootId.state = "idealangle";
                break;
            case "MinVotes":
                 rootId.state = "Iterations";
                break;
            case "DistanceThreshold":
                 rootId.state = "allowedanglevariance";
                break;
            case "Iterations":
                 rootId.state = "DistanceThreshold"
                break;
            }
        }

        onDownButtonPressed:
        {
            switch( rootId.state)
            {
            case "idealangle":
                 rootId.state = "allowedanglevariance";
                break;
            case "allowedanglevariance":
                 rootId.state = "DistanceThreshold";
                break;
            case "MinVotes":
                 rootId.state = "idealangle";
                break;
            case "DistanceThreshold":
                 rootId.state = "Iterations";
                break;
            case "Iterations":
                 rootId.state = "MinVotes";
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

    //Title Text
    Text
    {
        id: titletextId
        font.family: fontId.name
        y: 10
        x:10
        text: rootId.titletext
        font.pixelSize: rootId.titletextsize
        width: titletextId.implicitWidth
        height: titletextId.implicitHeight
        color: rootId.titletextcolor;
    }

    //Ideal Angle Slider
    SliderSettingsObject
    {
        id:  idealanglesliderId
        x:titletextId.x
        y: titletextId.y + titletextId.height + rootId.slidersverticalspacing
        width:  rootId.sliderwidth
        height:   rootId.sliderheight
        messagetext: "Ideal Angle: " + rootId.ransacparameter.idealAngle
        valuefrom: rootId.idealangle_fromvalue
        valueto: rootId.idealangle_tovalue
        stepsize: rootId.idealangle_stepsize
        majorstepsize: rootId.idealangle_majorstepsize
        controlstickautorepeat: rootId.idealangle_controlstickautorepeat
        fontsize:   rootId.slidertextsize
        highlightbordersize:   rootId.sliderborderwidth

        Component.onCompleted:
        {
             idealanglesliderId.value = ransacparameter.idealAngle;
        }

        onEndFocus:
        {
             rootId.grabFocus();
             rootId.state = "idealangle";
        }

        onValueChanged:
        {
            ransacparameter.idealAngle =  idealanglesliderId.value;
        }
    }

    //Allowed Angle Variance Slider
    SliderSettingsObject
    {
        id:  allowedanglevariancesliderId
        x:   rootId.sliderleftx
        y:  idealanglesliderId.y +  idealanglesliderId.height + rootId.slidersverticalspacing
        width: rootId.sliderwidth
        height: rootId.sliderheight
        messagetext: "Angle Variance: " + ransacparameter.allowedAngleVariance
        valuefrom: anglevariance_fromvalue
        valueto: anglevariance_tovalue
        stepsize: anglevariance_stepsize
        majorstepsize: anglevariance_majorstepsize
        controlstickautorepeat: anglevariance_controlstickautorepeat
        fontsize: rootId.slidertextsize
        highlightbordersize: rootId.sliderborderwidth

        Component.onCompleted:
        {

             allowedanglevariancesliderId.value = ransacparameter.allowedAngleVariance;
        }

        onEndFocus:
        {
              rootId.grabFocus();
             rootId.state = "allowedanglevariance";
        }

        onValueChanged:
        {
            ransacparameter.allowedAngleVariance =  allowedanglevariancesliderId.value;
        }
    }

    //Distance Threshold
    SliderSettingsObject
    {

        id:  distancethresholdsliderId
        x:  rootId.sliderleftx
        y:   rootId.slidersverticalspacing +  allowedanglevariancesliderId.y +  allowedanglevariancesliderId.height
        width:   rootId.sliderwidth
        height:   rootId.sliderheight
        messagetext: "Distance Threshold: " + rootId.ransacparameter.distanceThreshold
        valuefrom: rootId.distance_fromvalue
        valueto: rootId.distance_tovalue
        stepsize: rootId.distance_stepsize
        majorstepsize: rootId.anglevariance_majorstepsize
        controlstickautorepeat: rootId.distance_controlstickautorepeat
        fontsize: rootId.slidertextsize
        highlightbordersize: rootId.sliderborderwidth

        Component.onCompleted:
        {
             distancethresholdsliderId.value = ransacparameter.distanceThreshold;
        }

        onEndFocus:
        {
             rootId.grabFocus();
             rootId.state = "DistanceThreshold";
        }

        onValueChanged:
        {
            ransacparameter.distanceThreshold =  distancethresholdsliderId.value;
        }
    }

    //Iterations
    SliderSettingsObject
    {

        id:  iterationssliderId
        x:  rootId.sliderleftx
        y:   rootId.slidersverticalspacing +  distancethresholdsliderId.y + distancethresholdsliderId.height
        width:   rootId.sliderwidth
        height:   rootId.sliderheight
        messagetext: "Iterations: " + rootId.ransacparameter.iterations
        valuefrom: rootId.iterations_fromvalue
        valueto: rootId.iterations_tovalue
        stepsize: rootId.iterations_stepsize
        majorstepsize: rootId.iterations_majorstepsize
        controlstickautorepeat: rootId.iterations_controlstickautorepeat
        fontsize: rootId.slidertextsize
        highlightbordersize: rootId.sliderborderwidth

        Component.onCompleted:
        {
             iterationssliderId.value = rootId.ransacparameter.iterations;
        }

        onEndFocus:
        {
             rootId.grabFocus();
             rootId.state = "Iterations";
        }

        onValueChanged:
        {
            rootId.ransacparameter.iterations =  iterationssliderId.value;
        }
    }

    //Min Votes
    SliderSettingsObject
    {

        id:  minvotessliderId
        x:  rootId.sliderleftx
        y:   rootId.slidersverticalspacing +  iterationssliderId.y +  iterationssliderId.height
        width:   rootId.sliderwidth
        height:   rootId.sliderheight
        messagetext: "Min Votes: " + rootId.ransacparameter.minVotes
        valuefrom: rootId.minvotes_fromvalue
        valueto: rootId.minvotes_tovalue
        stepsize: rootId.minvotes_stepsize
        majorstepsize: rootId.minvotes_majorstepsize
        controlstickautorepeat: rootId.minvotes_controlstickautorepeat
        fontsize: rootId.slidertextsize
        highlightbordersize: rootId.sliderborderwidth

        Component.onCompleted:
        {
             minvotessliderId.value = rootId.ransacparameter.minVotes
        }

        onEndFocus:
        {
             rootId.grabFocus();
             rootId.state = "MinVotes"
        }

        onValueChanged:
        {
            rootId.ransacparameter.minVotes =  minvotessliderId.value;
        }
    }
}
