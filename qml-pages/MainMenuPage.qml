import QtQuick 2.0
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "qrc:/qml-components"
import "qrc:/qml-dialogs"


Item {
    //Properties-----------------------------------
    id: rootpageId
    anchors.fill: parent
    opacity: 1

    readonly property string pagename: "MainMenu Page"

    property var dynamicpage: null

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
        aboutToDestroy();
        destroyPage();
    }

    function createImageProcessingPage()
    {
        if(dynamicpage == null)
        {
            var component = Qt.createComponent("/qml-pages/ImageProcessingSettingsPage.qml");
            dynamicpage = component.createObject(rootpageId);
            if(page !== null)
            {
                dynamicpage.grabFocus();
                dynamicpage.destroyPage.connect(destroyDynamicPage);
            }
            else
            {
                console.log("QML: Could Not Create ImageProcessingSettingsPage");
            }

        }
    }

    function destroyDynamicPage()
    {
        if(dynamicpage !== null)
        {
            dynamicpage.destroy();
            dynamicpage = null;
            grabFocus();
        }
    }

    //Event Handlers-------------------------------
    Component.onCompleted:
    {
        rootpageId.completed();
        console.log("QML: MainMenuPage Created.");
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
            rotatemenuId.addMenuItem("qrc:/Icons/image.png", "Image Processsor Settings");
            rotatemenuId.addMenuItem("qrc:/Icons/quit.png", "Quit Application");
        }

        onSelectionMade:
        {
            switch(selection)
            {
            case rotatemenuId.closemenustring:
                signalDestroyPage();
                break;
            case "Image Processsor Settings":
                rootpageId.createImageProcessingPage();
                break;
            case "Quit Application":
                quitdialogId.visible = true;
                quitdialogId.grabFocus();
                break;
            case "Toggle Laser Power":
                Gary.sendToggleLaserPower();
                signalDestroyPage();
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
            switch(selection)
            {
            case "Yes":
                console.log("Exiting Program.")
                Qt.quit();
                break;
            case "No":
                quitdialogId.visible = false;
                rootpageId.grabFocus();
                break;
            }
        }
    }

}
