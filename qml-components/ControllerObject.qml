import QtQuick 2.12


//Container Object to Help Keep Controller Data in One Place
FocusScope {

    //id: controllerrootId
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
        if(event.isAutoRepeat) return
        switch(event.key)
        {
        case greenbutton:
            greenpressed = true;
            greenButtonPressed();
            break;
        case redbutton:
            redpressed = true;
            redButtonPressed();
            break;
        case blackbutton:
            blackpressed = true;
            blackButtonPressed();
            break;
        case upbutton:
            uppressed = true;
            upButtonPressed();
            break;
        case downbutton:
            downpressed = true;
            downButtonPressed();
            break;
        case leftbutton:
            leftpressed = true;
            leftButtonPressed();
            break;
        case rightbutton:
            rightpressed = true;
            rightButtonPressed();
            break;
        }

    }

    Keys.onReleased:
    {
        if(event.isAutoRepeat) return
        switch(event.key)
        {
        case greenbutton:
            greenpressed = false;
            greenButtonReleased();
            break;
        case redbutton:
            redpressed = false;
            redButtonReleased();
            break;
        case blackbutton:
            blackpressed = false;
            blackButtonReleased();
            break;
        case upbutton:
            uppressed = false;
            upButtonReleased();
            break;
        case downbutton:
            downpressed = false;
            downButtonReleased();
            break;
        case leftbutton:
            leftpressed = false;
            leftButtonReleased();
            break;
        case rightbutton:
            rightpressed = false;
            rightButtonReleased();
            break;
        }
    }

}
