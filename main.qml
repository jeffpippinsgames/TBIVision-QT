import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "qrc:/qml-pages" as MyQmlPages
import "qrc:/qml-components" as MyQmlComponents
import tbi.vision.components 1.0

Window {
    id: rootId
    visible: true
    width: 1920
    height: 1080
    title: qsTr("TBI Laser Seam Tracker")
    flags: Qt.FramelessWindowHint

    property MyQmlPages.MainPage mainpage: MyQmlPages.MainPage{width: rootId.width; height: rootId.height;}
    property MyQmlPages.MainMenuPage mainmenupage: MyQmlPages.MainMenuPage{width: rootId.width; height: rootId.height}

    readonly property int greenbutton: Qt.Key_Return
    readonly property int redbutton: Qt.Key_Backspace
    readonly property int blackbutton: Qt.Key_Escape
    readonly property int upbutton: Qt.Key_Up
    readonly property int downbutton: Qt.Key_Down
    readonly property int leftbutton: Qt.Key_Left
    readonly property int rightbutton: Qt.Key_Right


    Rectangle
    {
        id: backgroundrectId
        anchors.fill: parent
        color:"black"
    }

    StackView
    {
        focus: true
        id: stackview
        anchors.fill: parent
        initialItem: mainpage

        replaceEnter: Transition {
            PropertyAnimation{
                property: "opacity"
                from: 0
                to: 1
                duration: 600
            }
        }

        replaceExit: Transition {
            PropertyAnimation{
                property: "opacity"
                from: 1
                to: .5
                duration: 600
            }

        }


        Component.onCompleted:
        {

        }
    }


    //This is Where all signal/slot wiring is done
    Component.onCompleted:
    {

    }


}
