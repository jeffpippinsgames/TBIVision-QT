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
    property bool useOneButtonAtaTime: true

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

    Connections
    {
        target: TBIJoystick

        onTBIJoystick_Up_Pressed:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickUp_Pressed(_autorepeat);
        }
        onTBIJoystick_Down_Pressed:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickDown_Pressed(_autorepeat);
        }
        onTBIJoystick_Left_Pressed:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickLeft_Pressed(_autorepeat);
        }
        onTBIJoystick_Right_Pressed:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickRight_Pressed(_autorepeat);
        }
        onTBIJoystick_Green_Pressed:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickGreen_Pressed(_autorepeat);
        }
        onTBIJoystick_Red_Pressed:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickRed_Pressed(_autorepeat);
        }
        onTBIJoystick_Black_Pressed:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickBlack_Pressed(_autorepeat);
        }

        onTBIJoystick_Up_Released:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickUp_Released();
        }
        onTBIJoystick_Down_Released:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickDown_Released();
        }
        onTBIJoystick_Left_Released:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickLeft_Released();
        }
        onTBIJoystick_Right_Released:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickRight_Released();
        }
        onTBIJoystick_Green_Released:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickGreen_Released();
        }
        onTBIJoystick_Red_Released:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickRed_Released();
        }
        onTBIJoystick_Black_Released:
        {
            if(rootcomponentId.focus) rootcomponentId.onTBIJoystickBlack_Released();
        }
    }

    //TBI Pressed Handlers---------------------------------
    function onTBIJoystickUp_Pressed(_autorepeat)
    {
        if(!useAutoRepeatonSticks)
        {
            if(_autorepeat===true) return
        }
        if((currentlyheldbutton===nobutton) || (currentlyheldbutton===upbutton) || (!rootcomponentId.useOneButtonAtaTime))
        {
            uppressed = true;
            currentlyheldbutton = upbutton;
            upButtonPressed();
            if(showdebug) console.log("ControllerObject.qml: TBIJoystick Up Button Pressed");
        }
    }

    function onTBIJoystickDown_Pressed(_autorepeat)
    {
        if(!useAutoRepeatonSticks)
        {
            if(_autorepeat) return
        }
        if((currentlyheldbutton===nobutton) || (currentlyheldbutton===downbutton) || (!rootcomponentId.useOneButtonAtaTime))
        {
            downpressed = true;
            currentlyheldbutton = downbutton;
            downButtonPressed();
            if(showdebug) console.log("ControllerObject.qml: TBIJoystick Down Button Pressed");
        }
    }

    function onTBIJoystickLeft_Pressed(_autorepeat)
    {
        if(!useAutoRepeatonSticks)
        {
            if(_autorepeat) return
        }
        if((currentlyheldbutton===nobutton) || (currentlyheldbutton===leftbutton) || (!rootcomponentId.useOneButtonAtaTime))
        {
            leftpressed = true;
            currentlyheldbutton = leftbutton;
            leftButtonPressed();
            if(showdebug) console.log("ControllerObject.qml: TBIJoystick Left Button Pressed");
        }
    }

    function onTBIJoystickRight_Pressed(_autorepeat)
    {
        if(!useAutoRepeatonSticks)
        {
            if(_autorepeat) return
        }
        if((currentlyheldbutton===nobutton) || (currentlyheldbutton===rightbutton) || (!rootcomponentId.useOneButtonAtaTime))
        {
            rightpressed = true;
            currentlyheldbutton = rightbutton;
            rightButtonPressed();
            if(showdebug) console.log("ControllerObject.qml: TBIJoystick Right Button Pressed");
        }
    }

    function onTBIJoystickGreen_Pressed(_autorepeat)
    {
        if(!useAutoRepeatonButtons)
        {
            if(_autorepeat) return
        }
        if((currentlyheldbutton===nobutton) || (currentlyheldbutton===greenbutton) || (!rootcomponentId.useOneButtonAtaTime))
        {
            greenpressed = true;
            currentlyheldbutton = greenbutton;
            greenButtonPressed();
            if(showdebug) console.log("ControllerObject.qml: TBIJoystick Green Button Pressed");
        }
    }

    function onTBIJoystickRed_Pressed(_autorepeat)
    {
        if(!useAutoRepeatonButtons)
        {
            if(_autorepeat) return
        }
        if((currentlyheldbutton===nobutton) || (currentlyheldbutton===redbutton) || (!rootcomponentId.useOneButtonAtaTime))
        {
            redpressed = true;
            currentlyheldbutton = redbutton;
            redButtonPressed();
            if(showdebug) console.log("ControllerObject.qml: Red Button Pressed");
        }
    }

    function onTBIJoystickBlack_Pressed(_autorepeat)
    {
        if(!useAutoRepeatonButtons)
        {
            if(_autorepeat) return
        }
        if((currentlyheldbutton===nobutton) || (currentlyheldbutton===blackbutton) || (!rootcomponentId.useOneButtonAtaTime))
        {
            blackpressed = true;
            currentlyheldbutton = blackbutton;
            blackButtonPressed();
            if(showdebug) console.log("ControllerObject.qml: Black Button Pressed");
        }
    }

    //TBIJoystick Release Handlers------------------------
    function onTBIJoystickUp_Released()
    {
        if((currentlyheldbutton != upbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
        uppressed = false;
        currentlyheldbutton = nobutton;
        upButtonReleased();
        if(showdebug) console.log("ControllerObject.qml: TBIJoystick Up Button Released");
    }

    function onTBIJoystickDown_Released()
    {
        if((currentlyheldbutton != downbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
        downpressed = false;
        currentlyheldbutton = nobutton;
        downButtonReleased();
        if(showdebug) console.log("ControllerObject.qml: TBIJoystick Down Button Released");
    }

    function onTBIJoystickLeft_Released()
    {
        if((currentlyheldbutton != leftbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
        leftpressed = false;
        currentlyheldbutton = nobutton;
        leftButtonReleased();
        if(showdebug) console.log("ControllerObject.qml: TBIJoystick Left Button Released");
    }

    function onTBIJoystickRight_Released()
    {
        if((currentlyheldbutton != rightbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
        rightpressed = false;
        currentlyheldbutton = nobutton;
        rightButtonReleased();
        if(showdebug) console.log("ControllerObject.qml: TBIJoystick Right Button Released");
    }

    function onTBIJoystickGreen_Released()
    {
        if((currentlyheldbutton != greenbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
        greenpressed = false;
        currentlyheldbutton = nobutton;
        greenButtonReleased();
        if(showdebug) console.log("ControllerObject.qml: TBIJoystick Green Button Released");
    }

    function onTBIJoystickRed_Released()
    {
        if((currentlyheldbutton != redbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
        redpressed = false;
        currentlyheldbutton = nobutton;
        redButtonReleased();
        if(showdebug) console.log("ControllerObject.qml: TBIJoystick Red Button Released");
    }

    function onTBIJoystickBlack_Released()
    {
        if((currentlyheldbutton != blackbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
        blackpressed = false;
        currentlyheldbutton = nobutton;
        blackButtonReleased();
        if(showdebug) console.log("ControllerObject.qml: TBIJoystick Black Button Released");
    }

    //on Focused Changed.
    onFocusChanged:
    {
        currentlyheldbutton = nobutton;
        if(rootcomponentId.focus)
        {
            //Make TBIJoystick Connections
            //TBIJoystick.TBIJoystickEventSignal.connect(rootcomponentId.onTBIJoystickEvent);
        }
        else
        {
            //Disconnect From TBIJoystick
            //TBIJoystick.TBIJoystickEventSignal.disconnect(rootcomponentId.onTBIJoystickEvent);
        }
    }

    Connections
    {
        target: TBIJoystick



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
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===greenbutton) || (!rootcomponentId.useOneButtonAtaTime))
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
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===redbutton) || (!rootcomponentId.useOneButtonAtaTime))
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
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===blackbutton) || (!rootcomponentId.useOneButtonAtaTime))
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
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===upbutton) || (!rootcomponentId.useOneButtonAtaTime))
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
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===downbutton) || (!rootcomponentId.useOneButtonAtaTime))
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
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===leftbutton) || (!rootcomponentId.useOneButtonAtaTime))
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
            if((currentlyheldbutton===nobutton) || (currentlyheldbutton===rightbutton) || (!rootcomponentId.useOneButtonAtaTime))
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
            if((currentlyheldbutton != greenbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
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
            if((currentlyheldbutton != redbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
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
            if((currentlyheldbutton != blackbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
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
            if((currentlyheldbutton != upbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
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
            if((currentlyheldbutton != downbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
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
            if((currentlyheldbutton != leftbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
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
            if((currentlyheldbutton != rightbutton) && (rootcomponentId.useOneButtonAtaTime)) return;
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
