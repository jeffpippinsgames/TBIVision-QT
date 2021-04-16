import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12


Item
{
    id: rootcomponentId
    visible: false
    focus:rootcomponentId.visible
    anchors.fill: parent

    //Properties
    property string displayedvalue: ""
    property string displayedmessage: "Enter Value:"
    property bool integervalue: false
    property bool negativeallowed: true
    property string returnedvalue: ""
    property real backgroundopacity: 1

    //Signals
    signal valueEntered()
    signal aboutToOpen()
    signal aboutToClose()

    //Misc Functions-------------------------------------------------
    function grabFocus()
    {
        controllerId.focus = true;
    }

    function open()
    {
        displayedvalue = "";
        rootcomponentId.aboutToOpen();
        rootcomponentId.visible = true;
    }

    function close()
    {
        displayedvalue = "";
        rootcomponentId.aboutToClose();
        rootcomponentId.visible = false;
    }

    //Font for UI
    FontLoader
    {
        id: font2Id
        source: "qrc:/Fonts/Typo Draft Demo.otf"
    }

    //Controller Object
    ControllerObject
    {
        id:controllerId
        focus:rootcomponentId.focus

        onGreenButtonPressed:
        {

            if((gridviewmodelId.get(gridviewId.currentIndex).boxstring !== ".") && (gridviewmodelId.get(gridviewId.currentIndex).boxstring !== "+/-"))
            {
                displayedvalue = displayedvalue + gridviewmodelId.get(gridviewId.currentIndex).boxstring;
            }
            else if((gridviewmodelId.get(gridviewId.currentIndex).boxstring === ".") && !integervalue)
            {
                displayedvalue = displayedvalue + gridviewmodelId.get(gridviewId.currentIndex).boxstring;
            }
            else if((gridviewmodelId.get(gridviewId.currentIndex).boxstring === "+/-") && negativeallowed)
            {
                if(displayedvalue.search('-') === -1 && displayedvalue.length >= 1) //There is no Negative Sign So Add it
                {
                    displayedvalue = "-" + displayedvalue;
                }
                else //Negative Sign Exists Take it out
                {
                    displayedvalue = displayedvalue.substring(1)
                }
            }

        }

        onRedButtonPressed:
        {
                displayedvalue = displayedvalue.substring(0, displayedvalue.length - 1);
        }

        onBlackButtonPressed:
        {
            returnedvalue = displayedvalue;
            rootcomponentId.close();
        }

        onUpButtonPressed:
        {
            gridviewId.moveCurrentIndexUp();
        }

        onDownButtonPressed:
        {
            gridviewId.moveCurrentIndexDown();
        }

        onLeftButtonPressed:
        {
            gridviewId.moveCurrentIndexLeft();
        }

        onRightButtonPressed:
        {
            gridviewId.moveCurrentIndexRight();
        }
    }

    //Background Image
    Image
    {
        id: backgroundimageId
        source: "qrc:/Icons/dark_steel_texture.jpg"
        anchors.fill: parent
        opacity: rootcomponentId.backgroundopacity
    }

    ControllerKeyObject
    {
        greenbuttonmessage: "Select"
        redbuttonmessage: "Backspace"
        blackbuttonmessage: "Accept"
    }

    //displayedmessage Text
    Text
    {
        id: textdisplayedmessageId
        text: displayedmessage
        font.family: font2Id.name
        font.pointSize: 30
        width: textdisplayedmessageId.implicitWidth
        height: textdisplayedmessageId.implicitHeight
        //anchors.verticalCenter: keypadrectId.verticalCenter
        anchors.horizontalCenter: keypadrectId.horizontalCenter
        y: keypadrectId.y - textdisplayedmessageId.implicitHeight - 10
        //y: keypadrectId.y + keypadrectId.height/2 - textdisplayedmessageId.implicitHeight/2 - 50
        //x: keypadrectId.x - textdisplayedmessageId.implicitWidth - 50
        color: Qt.rgba(1,1,.95,1)

    }

    //Gridview Container
    Rectangle
    {
        id:keypadrectId
        opacity: .9
        anchors.centerIn: parent
        width:470
        height:700
        focus:false
        visible: rootcomponentId.visible
        color: "transparent"

        FontLoader
        {
            id: fontId
            source: "qrc:/Fonts/MarckScript-Regular.ttf"
        }

        GridView
        {
            id:gridviewId
            visible: parent.visible
            x: 10
            y: 10
            width: parent.width - 20
            height: parent.height - 20
            anchors.centerIn: parent
            cellHeight: 150
            cellWidth: 150


            Connections
            {
                target: rootcomponentId
                function onintegervalueChanged()
                {
                    if(integervalue)
                    {
                        gridviewmodelId.setProperty(9, enabled, false);
                        gridviewmodelId.setProperty(9, "buttoncolor", "#700000");
                    }
                    else
                    {
                        gridviewmodelId.setProperty(9, enabled, true);
                        gridviewmodelId.setProperty(9, "buttoncolor", "#202020");
                    }
                }

                function onnegativeallowedChanged()
                {
                    if(!negativeallowed)
                    {
                        console.log("BOOOM");
                        gridviewmodelId.setProperty(11, enabled, false);
                        gridviewmodelId.setProperty(11, "buttoncolor", "#700000");
                    }
                    else
                    {
                        gridviewmodelId.setProperty(11, enabled, true);
                        gridviewmodelId.setProperty(11, "buttoncolor", "#202020");
                    }
                }

            }

            Rectangle
            {
                id: highlightId
                visible: parent.visible
                width: 143
                height: 143
                color: "green"
                opacity: .4
                x: gridviewId.currentItem.x
                y: gridviewId.currentItem.y
                radius: 20
                z:5
                //Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                //Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
            }

            Component.onCompleted:
            {
                if(integervalue)
                {
                    gridviewmodelId.setProperty(9, enabled, false);
                    gridviewmodelId.setProperty(9, "buttoncolor", "#700000");
                }
                else
                {
                    gridviewmodelId.setProperty(9, enabled, true);
                    gridviewmodelId.setProperty(9, "buttoncolor", "#202020");
                }
                if(!negativeallowed)
                {
                    gridviewmodelId.setProperty(11, enabled, false);
                    gridviewmodelId.setProperty(11, "buttoncolor", "#700000");
                }
                else
                {
                    gridviewmodelId.setProperty(11, enabled, true);
                    gridviewmodelId.setProperty(11, "buttoncolor", "#202020");
                }
            }

            model: ListModel
            {
                id: gridviewmodelId
                ListElement
                {
                    boxstring: "1"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "2"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "3"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "4"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "5"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "6"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "7"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "8"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "9"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "."
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "0"
                    enabled: true
                    buttoncolor: "#202020"
                }
                ListElement
                {
                    boxstring: "+/-"
                    enabled: true
                    buttoncolor: "#202020"
                }

            }

            delegate: ItemDelegate
            {
                id:gridviewdelegateId
                enabled: enabled

                Rectangle
                {
                    color: buttoncolor
                    opacity: 1.0
                    width:143
                    height:143
                    radius: 20

                    Text
                    {
                        text: boxstring
                        font.family: fontId.name
                        font.pointSize: 50
                        color: Qt.rgba(1,1,.95,1)
                        anchors.centerIn: parent
                    }
                }
            }
        }

        Text
        {
            id: textvalueId
            text: displayedvalue
            font.family: fontId.name
            font.pointSize: 50
            width: textvalueId.implicitWidth
            height: textvalueId.implicitHeight
            y: keypadrectId.height - textvalueId.height - 10
            anchors.horizontalCenter: keypadrectId.horizontalCenter
            color: Qt.rgba(1,1,.95,1)
        }
    }

}
