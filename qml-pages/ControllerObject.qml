import QtQuick 2.0


//Container Object to Help Keep Controller Data in One Place
Item {
    focus: true
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


    Keys.onPressed:
    {
        switch(event.key)
        {
        case greenbutton:
            greenpressed = true;
            break;
        case redbutton:
            redpressed = true;
            break;
        case blackbutton:
            blackpressed = true;
            break;
        case upbutton:
            uppressed = true;
            break;
        case downbutton:
            downpressed = true;
            break;
        case leftbutton:
            leftpressed = true;
            break;
        case rightbutton:
            rightpressed = true;
            break;
        }
    }
    Keys.onReleased:
    {
        switch(event.key)
        {
        case greenbutton:
            greenpressed = false;
            break;
        case redbutton:
            redpressed = false;
            break;
        case blackbutton:
            blackpressed = false;
            break;
        case upbutton:
            uppressed = false;
            break;
        case downbutton:
            downpressed = false;
            break;
        case leftbutton:
            leftpressed = false;
            break;
        case rightbutton:
            rightpressed = false;
            break;
        }
    }

}
