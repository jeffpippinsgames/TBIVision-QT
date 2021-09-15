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
    readonly property int nobutton: 0

    readonly property bool showdebug: true

    property int currentlyheldbutton: 0

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

    function isAnyOtherKeyPressed(qtKey)
    {

        switch(qtKey)
        {
        case greenbutton:
            if(redpressed || blackpressed || uppressed || downpressed || leftpressed || rightpressed)
            {
                return true;
            }
            break;
        case redbutton:
            if(greenpressed || blackpressed || uppressed || downpressed || leftpressed || rightpressed)
            {
                return true;
            }
            break;
        case blackbutton:
            if(greenpressed || redpressed || uppressed || downpressed || leftpressed || rightpressed)
            {
                return true;
            }
            break;
        case upbutton:
            if(greenpressed || redpressed || blackpressed || downpressed || leftpressed || rightpressed)
            {
                return true;
            }
            break;
        case downbutton:
            if(greenpressed || redpressed || blackpressed || uppressed || leftpressed || rightpressed)
            {
                return true;
            }
            break;
        case leftbutton:
            if(greenpressed || redpressed || blackpressed || uppressed || downpressed || rightpressed)
            {
                return true;
            }
            break;
        case rightbutton:
            if(greenpressed || redpressed || blackpressed || uppressed || downpressed || leftpressed)
            {
                return true;
            }
            break;
        }

        return false;
    }


    onFocusChanged:
    {
        currentlyheldbutton = nobutton;
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
        if(showdebug) console.log("ControllerObject.qml: controlsignal: " + controlsignal + " - autorepeat: " + autorepeat);
        switch(controlsignal)
        {
        case GaryControllerQMLSignals.TBI_CONTROLLER_GREENBTN_PRESSED:
            if(!useAutoRepeatonButtons)
            {
                if(autorepeat) return;
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===greenbutton))
            {
                greenpressed = true;
                currentlyheldbutton = greenbutton;
                greenButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Green Button Pressed");
            }
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_REDBTN_PRESSED:
            if(isAnyOtherKeyPressed(redbutton)) return;
            if(!useAutoRepeatonButtons)
            {
                if(autorepeat) return;
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===redbutton))
            {
                redpressed = true;
                currentlyheldbutton = redbutton;
                redButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Red Button Pressed");
            }
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_BLACKBTN_PRESSED:
            if(isAnyOtherKeyPressed(greenbutton)) return;
            if(!useAutoRepeatonButtons)
            {
                if(autorepeat) return;
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===blackbutton))
            {
                blackpressed = true;
                currentlyheldbutton = blackbutton;
                blackButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Black Button Pressed");
            }
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYUP_PRESSED:
            if(!useAutoRepeatonSticks)
            {
                if(autorepeat) return;
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===upbutton))
            {
                uppressed = true;
                currentlyheldbutton = upbutton;
                upButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Joystick Up Pressed");
            }
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYDOWN_PRESSED:
            if(!useAutoRepeatonSticks)
            {
                if(autorepeat) return;
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===downbutton))
            {
                downpressed = true;
                currentlyheldbutton = downbutton;
                downButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Joystick Down Pressed");
            }
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYLEFT_PRESSED:
            if(!useAutoRepeatonSticks)
            {
                if(autorepeat) return;
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===leftbutton))
            {
                leftpressed = true;
                currentlyheldbutton = leftbutton;
                leftButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Joystick Left Pressed");
            }
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYRIGHT_PRESSED:
            if(!useAutoRepeatonSticks)
            {
                if(autorepeat) return;
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===rightbutton))
            {
                rightpressed = true;
                currentlyheldbutton = rightbutton;
                rightButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Joystick Right Pressed");
            }
            break;
        //--------------------------------------
        case GaryControllerQMLSignals.TBI_CONTROLLER_GREENBTN_RELEASED:
            if(currentlyheldbutton != greenbutton) return;
            greenpressed = false;
            currentlyheldbutton = nobutton;
            greenButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Green Button Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_REDBTN_RELEASED:
            if(currentlyheldbutton != redbutton) return;
            redpressed = false;
            currentlyheldbutton = nobutton;
            redButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Red Button Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_BLACKBTN_RELEASED:
            if(currentlyheldbutton != blackbutton) return;
            blackpressed = false;
            currentlyheldbutton = nobutton;
            blackButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Black Button Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYUP_RELEASED:
            if(currentlyheldbutton != upbutton) return;
            uppressed = false;
            currentlyheldbutton = nobutton;
            upButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Joystick Up Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYDOWN_RELEASED:
            if(currentlyheldbutton != downbutton) return;
            downpressed = false;
            downButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Joystick Down Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYLEFT_RELEASED:
            if(currentlyheldbutton != leftbutton) return;
            leftpressed = false;
            currentlyheldbutton = nobutton;
            leftButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Joystick Left Released");
            break;
        case GaryControllerQMLSignals.TBI_CONTROLLER_JOYRIGHT_RELEASED:
            if(currentlyheldbutton != rightbutton) return;
            rightpressed = false;
            currentlyheldbutton = nobutton;
            rightButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Joystick Right Released");
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
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===greenbutton))
            {
                greenpressed = true;
                currentlyheldbutton = greenbutton;
                greenButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Green Button Pressed");
            }
            break;
        case redbutton:
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===redbutton))
            {
                redpressed = true;
                currentlyheldbutton = redbutton;
                redButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Red Button Pressed");
            }
            break;
        case blackbutton:
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===blackbutton))
            {
                blackpressed = true;
                currentlyheldbutton = blackbutton;
                blackButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Black Button Pressed");
            }
            break;
        case upbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===upbutton))
            {
                uppressed = true;
                currentlyheldbutton = upbutton;
                upButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Up Button Pressed");
            }
            break;
        case downbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===downbutton))
            {
                downpressed = true;
                currentlyheldbutton = downbutton;
                downButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Down Button Pressed");
            }

            break;
        case leftbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===leftbutton))
            {
                leftpressed = true;
                currentlyheldbutton = leftbutton;
                leftButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Left Button Pressed");
            }
            break;
        case rightbutton:
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===rightbutton))
            {
                rightpressed = true;
                currentlyheldbutton = rightbutton;
                rightButtonPressed();
                if(showdebug) console.log("ControllerObject.qml: Right Button Pressed");
            }
            break;
        }

    }

    Keys.onReleased:
    {
        switch(event.key)
        {
        case greenbutton:
            if(currentlyheldbutton != greenbutton) return;
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            greenpressed = false;
            currentlyheldbutton = nobutton;
            greenButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Green Button Released");
            break;
        case redbutton:
            if(currentlyheldbutton != redbutton) return;
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            redpressed = false;
            currentlyheldbutton = nobutton;
            redButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Red Button Released");
            break;
        case blackbutton:
            if(currentlyheldbutton != blackbutton) return;
            if(!useAutoRepeatonButtons)
            {
                if(event.isAutoRepeat) return
            }
            blackpressed = false;
            currentlyheldbutton = nobutton;
            blackButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Black Button Released");
            break;
        case upbutton:
            if(currentlyheldbutton != upbutton) return;
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            uppressed = false;
            currentlyheldbutton = nobutton;
            upButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Up Button Released");
            break;
        case downbutton:
            if(currentlyheldbutton != downbutton) return;
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            downpressed = false;
            currentlyheldbutton = nobutton;
            downButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Down Button Released");
            break;
        case leftbutton:
            if(currentlyheldbutton != leftbutton) return;
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            leftpressed = false;
            currentlyheldbutton = nobutton;
            leftButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Left Button Released");
            break;
        case rightbutton:
            if(currentlyheldbutton != rightbutton) return;
            if(!useAutoRepeatonSticks)
            {
                if(event.isAutoRepeat) return
            }
            rightpressed = false;
            currentlyheldbutton = nobutton;
            rightButtonReleased();
            if(showdebug) console.log("ControllerObject.qml: Right Button Released");
            break;
        }
    }
}
