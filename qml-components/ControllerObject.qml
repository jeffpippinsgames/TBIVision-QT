import QtQuick 2.12
import tbi.vision.components 1.0


//Container Object to Help Keep Controller Data in One Place
FocusScope {

    id: rootcomponentId
    focus: false

    //Joystick Properties
    readonly property int greenbutton: Qt.Key_Return
    readonly property int redbutton: Qt.Key_Backspace
    readonly property int blackbutton: Qt.Key_Escape
    readonly property int upbutton: Qt.Key_Up
    readonly property int downbutton: Qt.Key_Down
    readonly property int leftbutton: Qt.Key_Left
    readonly property int rightbutton: Qt.Key_Right

    property bool greenpressed : false
    property bool redpressed : false
    property bool blackpressed : false
    property bool uppressed : false
    property bool downpressed : false
    property bool leftpressed : false
    property bool rightpressed : false
    property bool stickpressed: false

    property bool useAutoRepeatonSticks: false
    property bool useAutoRepeatonButtons: false

    signal greenButtonPressed()
    signal greenButtonReleased()

    signal redButtonPressed()
    signal redButtonReleased()

    signal blackButtonPressed()
    signal blackButtonReleased()

    signal upButtonPressed()
    signal upButtonReleased()

    signal downButtonPressed()
    signal downButtonReleased()

    signal leftButtonPressed()
    signal leftButtonReleased()

    signal rightButtonPressed()
    signal rightButtonReleased()




    onFocusChanged:
    {
        if(rootcomponentId.focus)
        {
          Gary.garyControllerFired.connect(rootcomponentId.processGaryControls);
        }
        else
        {
            Gary.garyControllerFired.disconnect(rootcomponentId.processGaryControls);
        }
    }

    function processGaryControls(controlsignal, autorepeat)
    {


        console.log("controlsignal: " + controlsignal + " - autorepeat: " + autorepeat);
        switch(controlsignal)
        {
        case GaryControllerQMLSignals.TBI_CONTROLLER_GREENBTN_PRESSED:
            if(!useAutoRepeatonButtons)
            {
                if(autorepeat) return;
            }
            greenpressed = true;
            greenButtonPressed();
            //console.log("Green Button Pressed");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_REDBTN_PRESSED:
            if(!useAutoRepeatonButtons)
            {
                if(autorepeat) return;
            }
            redpressed = true;
            redButtonPressed();
            //console.log("Red Button Pressed");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_BLACKBTN_PRESSED:
            if(!useAutoRepeatonButtons)
            {
                if(autorepeat) return;
            }
            blackpressed = true;
            blackButtonPressed();
            //console.log("Black Button Pressed");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYUP_PRESSED:
            if(!useAutoRepeatonSticks)
            {
                if(autorepeat) return;
            }
            uppressed = true;
            upButtonPressed();
            //console.log("Joystick Up Pressed");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYDOWN_PRESSED:
            if(!useAutoRepeatonSticks)
            {
                if(autorepeat) return;
            }
            downpressed = true;
            downButtonPressed();
            //console.log("Joystick Down Pressed");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYLEFT_PRESSED:
            if(!useAutoRepeatonSticks)
            {
                if(autorepeat) return;
            }
            leftpressed = true;
            leftButtonPressed();
            //console.log("Joystick Left Pressed");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYRIGHT_PRESSED:
            if(!useAutoRepeatonSticks)
            {
                if(autorepeat) return;
            }
            rightpressed = true;
            rightButtonPressed();
            //console.log("Joystick Right Pressed");
            break;
        //--------------------------------------
        case GaryControllerQMLSignals.TBI_CONTROLLER_GREENBTN_RELEASED:
            greenpressed = false;
            greenButtonReleased();
            //console.log("Green Button Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_REDBTN_RELEASED:
            redpressed = false;
            redButtonReleased();
            //console.log("Red Button Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_BLACKBTN_RELEASED:
            blackpressed = false;
            blackButtonReleased();
            //console.log("Black Button Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYUP_RELEASED:
            uppressed = false;
            upButtonReleased();
            //console.log("Joystick Up Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYDOWN_RELEASED:
            downpressed = false;
            downButtonReleased();
            //console.log("Joystick Down Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYLEFT_RELEASED:
            leftpressed = false;
            leftButtonReleased();
            //console.log("Joystick Left Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYRIGHT_RELEASED:
            rightpressed = false;
            rightButtonReleased();
            //console.log("Joystick Right Released");
            break;



        }
    }

    Keys.onPressed:
    {

        switch(event.key)
        {
        case greenbutton:
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            greenpressed = true;
            greenButtonPressed();
            break;
        case redbutton:
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            redpressed = true;
            redButtonPressed();
            break;
        case blackbutton:
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            blackpressed = true;
            blackButtonPressed();
            break;
        case upbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            uppressed = true;
            upButtonPressed();
            break;
        case downbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            downpressed = true;
            downButtonPressed();
            break;
        case leftbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            leftpressed = true;
            leftButtonPressed();
            break;
        case rightbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            rightpressed = true;
            rightButtonPressed();
            break;
        }

    }

    Keys.onReleased:
    {
        switch(event.key)
        {
        case greenbutton:
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            greenpressed = false;
            greenButtonReleased();
            break;
        case redbutton:
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            redpressed = false;
            redButtonReleased();
            break;
        case blackbutton:
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            blackpressed = false;
            blackButtonReleased();
            break;
        case upbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            uppressed = false;
            upButtonReleased();
            break;
        case downbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            downpressed = false;
            downButtonReleased();
            break;
        case leftbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            leftpressed = false;
            leftButtonReleased();
            break;
        case rightbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            rightpressed = false;
            rightButtonReleased();
            break;
        }
    }
}
