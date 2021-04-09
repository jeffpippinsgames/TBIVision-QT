import QtQuick 2.0
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"


Item {
    //Properties-----------------------------------
    id: mainmenuId
    anchors.fill: parent

    //Signals--------------------------------------
    signal destroyPage()
    signal aboutToDestroy()
    signal completed()

    //Functions------------------------------------
    function grabFocus()
    {
        rotatemenuId.grabFocus();
    }

    function onSelectionMade(_selection)
    {
        switch(_selection)
        {
        case "Quit Application":
            Qt.quit();
            break;

        }
        signalDestroyPage();
    }

    function signalDestroyPage()
    {

        aboutToDestroy();
        destroyPage();
    }

    //Event Handlers-------------------------------
    Component.onCompleted:
    {
        rotatemenuId.addMenuItem("qrc:/Icons/save.png", "Save Profile");
        rotatemenuId.addMenuItem("qrc:/Icons/gear.png", "System Settings");
        rotatemenuId.addMenuItem("qrc:/Icons/laser3.png", "Laser");
        rotatemenuId.addMenuItem("qrc:/Icons/quit.png", "Quit Application");
        rotatemenuId.selectionMade.connect(onSelectionMade);
        rotatemenuId.destroyPage.connect(signalDestroyPage);
        mainmenuId.completed();
        console.log("mainmenuId Created.");
    }

    //QML Components-------------------------------
    RotateMenu
    {
        id: rotatemenuId
        menutitle: "Main Menu"
    }





}
