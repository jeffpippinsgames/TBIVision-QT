import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "qrc:/qml-pages" as MyQmlPages
import "qrc:/qml-components" as MyQmlComponents


Window {
    id: rootId
    visible: true
    width: 1920
    height: 1080
    title: qsTr("TBI Laser Seam Tracker")
    flags: Qt.FramelessWindowHint


    property var mainoperationpage: null

    function openMainOperationPage()
    {
        if(mainoperationpage == null)
        {
            var component = Qt.createComponent("/qml-pages/MainPage.qml");
            mainoperationpage = component.createObject(rootId);
            if(mainoperationpage !== null)
            {
                mainoperationpage.grabFocus();
                console.log("mainoperationpage created.")
            }
            else
            {
                console.log("mainoperationpage failed to Create.");
            }

        }
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
        openMainOperationPage();
    }


}
