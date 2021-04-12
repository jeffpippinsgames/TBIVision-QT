import QtQuick 2.0
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"
import "qrc:/qml-dialogs"


Item {
    //Properties-----------------------------------
    id: mainmenuId
    anchors.fill: parent

    readonly property string pagename: "MainMenu Page"

    //Signals--------------------------------------
    signal destroyPage()
    signal aboutToDestroy()
    signal completed()

    //Functions------------------------------------
    function grabFocus()
    {
        rotatemenuId.grabFocus();
    }

    function signalDestroyPage()
    {
        console.log("MainMenuPage.qml Firing aboutToDestroy() Signal");
        aboutToDestroy();
        console.log("MainMenuPage.qml Firing aboutToDestroy() Signal");
        destroyPage();
    }

    //Event Handlers-------------------------------
    Component.onCompleted:
    {
        mainmenuId.completed();
        console.log("mainmenuId Created.");
    }

    //QML Components-------------------------------
    RotateMenu
    {
        id: rotatemenuId
        menutitle: "Main Menu"

        Component.onCompleted:
        {
            rotatemenuId.addMenuItem("qrc:/Icons/save.png", "Save Profile");
            rotatemenuId.addMenuItem("qrc:/Icons/gear.png", "System Settings");
            rotatemenuId.addMenuItem("qrc:/Icons/laser3.png", "Toggle Laser Power");
            rotatemenuId.addMenuItem("qrc:/Icons/quit.png", "Quit Application");
        }

        onSelectionMade:
        {
            switch(selection)
            {
            case rotatemenuId.closemenustring:
                signalDestroyPage();
                break;
            case "Quit Application":
                quitdialogId.visible = true;
                quitdialogId.grabFocus();
                break;
            }
        }
    }

    //Quit Program Verification--------------------
    TwoButtonOptionDialog
    {
        id: quitdialogId
        visible: false
        option1: "Yes"
        option2: "No"

        messagetext: "Are You Sure You Want To Quit?"
        onSelectionMade:
        {
            console.log("Ding");
            switch(selection)
            {
            case "Yes":
                console.log("Exiting Program.")
                Qt.quit();
                break;
            case "No":
                quitdialogId.visible = false;
                mainmenuId.grabFocus();
                break;
            }
        }
    }
}
