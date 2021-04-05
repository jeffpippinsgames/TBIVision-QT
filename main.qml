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

        Keys.onPressed:
        {
            //console.log("StackView in main.qml- Keys.OnPressed Event Handler Fired.")

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
