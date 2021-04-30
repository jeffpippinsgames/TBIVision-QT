import QtQuick 2.12


//Container Object to Help Keep Controller Data in One Place
FocusScope {

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
    property bool stickpressed: false;

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
