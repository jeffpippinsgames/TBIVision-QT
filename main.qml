import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "qrc:/qml-pages"
import "qrc:/qml-components"

/*
  main.qml is a page management component.
  only 1 page is ever in memory at a time.
  pages must be built accordingly.
  */
Window {
    id: rootId
    visible: true
    //visibility: Window.FullScreen
    width: 1920
    height: 1080
    title: qsTr("TBI Laser Seam Tracker")
    flags: Qt.FramelessWindowHint

    property var page: null
    readonly property bool showdebug: false

    function openPage(_page)
    {
        var component = null;
        switch(_page)
        {
        case pagesId.operationspage:
            component = Qt.createComponent("/qml-pages/OperationsPage.qml");
            break;
        case pagesId.mainmenupage:
            component = Qt.createComponent("/qml-pages/MainMenuPage.qml");
            break;
        case pagesId.vgroovepipelinesettingspage:
            component = Qt.createComponent("/qml-pages/VGrooveImageProcessingSettingsPage.qml");
            break;
        case pagesId.buttjointpipelinesettingspage:
            component = Qt.createComponent("/qml-pages/ButtJointImageProcessingSettingsPage.qml");
            break;
        }

        if(component === null) return;

        if(page == null)
        {
            page = component.createObject(rootId);
            if(page !== null)
            {
                page.grabFocus(); //Send Focus
                page.destroyPage.connect(destroyPage); //Connect The Destruction
                if(rootId.showdebug) console.log(page.pagename + " is Being Created.");
            }
            else
            {
                if(rootId.showdebug) console.log("main.qml: failed to create page.");
            }

        }
    }

    function destroyPage(_transition_page)
    {
        if(rootId.showdebug) console.log(page.pagename + " is Being Destroyed.")
        page.destroy();
        page = null;
        openPage(_transition_page);
    }

    PagesDescriptionObject
    {
        id:pagesId
    }

    Rectangle
    {
        id: backgroundrectId
        anchors.fill: parent
        color: "black"
    }

    //This is where main operating page is created
    //and where any signal/slot wiring is done.
    Component.onCompleted:
    {
        openPage(pagesId.operationspage);
    }
}
