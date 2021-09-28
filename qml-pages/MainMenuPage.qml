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

    //Signals--------------------------------------
    signal destroyPage(string _transition_page)

    //Functions------------------------------------
    function grabFocus()
    {
        rotatemenuId.grabFocus();
    }

    function cleanupForDestruction()
    {
        //Kill All Bindings For The Destruction

        //Disconnect All Signals


    }

    //Event Handlers-------------------------------
    Component.onCompleted:
    {

    }

    //QML Components-------------------------------

    //Required For Every Page
    PagesDescriptionObject
    {
        id:pagesId
    }

    RotateMenu
    {
        id: rotatemenuId
        menutitle: "Main Menu"

        Component.onCompleted:
        {
            rotatemenuId.addMenuItem("qrc:/Icons/save.png", "Save Profile");
            rotatemenuId.addMenuItem("qrc:/Icons/gear.png", "System Settings");
            rotatemenuId.addMenuItem("qrc:/Icons/laser3.png", "Toggle Laser Power");
            rotatemenuId.addMenuItem("qrc:/Icons/processingpipeline.png", "VGroove Joint Pipeline Settings");
            rotatemenuId.addMenuItem("qrc:/Icons/processingpipeline.png", "Butt Joint Pipeline Settings");
            rotatemenuId.addMenuItem("qrc:/Icons/quit.png", "Quit Application");
            rotatemenuId.addMenuItem("qrc:/Icons/image.png", "Process Still Image");
            rotatemenuId.addMenuItem("qrc:/Icons/Camera.png", "Process Camera");

        }

        onSelectionMade:
        {
            switch(selection)
            {
            case rotatemenuId.closemenustring:
                rootpageId.cleanupForDestruction();
                rootpageId.destroyPage(pagesId.operationspage);
                break;
            case "VGroove Joint Pipeline Settings":
                rootpageId.cleanupForDestruction();
                rootpageId.destroyPage(pagesId.vgroovepipelinesettingspage)
                break;
            case "Butt Joint Pipeline Settings":
                rootpageId.cleanupForDestruction();
                rootpageId.destroyPage(pagesId.buttjointpipelinesettingspage)
                break;
            case "Quit Application":
                quitdialogId.visible = true;
                quitdialogId.grabFocus();
                break;
            case "Toggle Laser Power":
                Gary.toggleLaserPower();
                rootpageId.cleanupForDestruction();
                rootpageId.destroyPage(pagesId.operationspage);
                break;
            case "Process Still Image":
                Toby.updateStillImageFileList();
                Toby.stopCamera();
                stillimagefiledialogId.visible = true;
                break;
            case "Process Camera":
                Toby.closeStillImagetoProcess();
                rootpageId.cleanupForDestruction();
                rootpageId.destroyPage(pagesId.operationspage);
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

    //Still Image File Open Dialog
    StillImageFileOpenDialog
    {
        id: stillimagefiledialogId
        visible: false

        onVisibleChanged:
        {
            if(stillimagefiledialogId.visible == true)
            {
                stillimagefiledialogId.grabFocus();
            }
            else
            {
                rootpageId.cleanupForDestruction();
                rootpageId.destroyPage(pagesId.operationspage);
            }

        }
    }

}
