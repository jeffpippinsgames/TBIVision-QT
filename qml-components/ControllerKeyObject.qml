import QtQuick 2.12


//Bottom Button Key
Item {
    id: buttonkeyimagesId
    anchors.fill: parent
    focus:false

    property string greenbuttonmessage: "Green Button"
    property bool enablegreenbutton: true
    property string redbuttonmessage: "Red Button"
    property bool enableredbutton: true
    property string blackbuttonmessage: "Black Button"
    property bool enableblackbutton: true

    readonly property int buttonsize: 100
    readonly property int xmargin: 20
    readonly property int ymargin: 20
    readonly property int buttonspacing: 50

    //Font for UI
    FontLoader
    {
        id: fontId
        source: "qrc:/Fonts/Typo Draft Demo.otf"
    }

    //Green Button Hugs the Left Side of the Screen
    Item
    {
        id: greenbuttonitemId
        x: parent.x + xmargin
        y: parent.height - greenbuttonitemId.height - ymargin
        width: greenbuttontextId.x + greenbuttontextId.implicitWidth
        height: greenbuttonimageId.height
        Image
        {
            id: greenbuttonimageId
            x:0
            y:0
            width: buttonsize
            height: buttonsize
            visible: enablegreenbutton
            source: "qrc:/Icons/GreenButton2.png"
            opacity: .60
        }

        Text
        {
            id: greenbuttontextId
            x: greenbuttonimageId.x + greenbuttonimageId.width + xmargin
            anchors.verticalCenter: greenbuttonimageId.verticalCenter
            text: greenbuttonmessage
            visible: enablegreenbutton
            font.family: fontId.name
            font.pointSize: 15
            width: greenbuttontextId.implicitWidth
            height: greenbuttontextId.implicitHeight
            color: Qt.rgba(.5, .5, .5, 1)
        }
    }


    //Red Button Sits in the Middle of the Screen
    Item
    {
        id: redbuttonitemId
        x: greenbuttonitemId.x + greenbuttonitemId.width + buttonspacing
        //x: parent.width/2 - redbuttonimageId.width/2
        y: parent.height - redbuttonitemId.height - ymargin
        width: redbuttontextId.x  + redbuttontextId.implicitWidth
        height: redbuttonimageId.height
        Image
        {
            id: redbuttonimageId
            x:0
            y:0
            width:buttonsize
            height:buttonsize
            visible: enableredbutton
            source: "qrc:/Icons/RedButton2.png"
            opacity: .60
        }

        Text
        {
            id: redbuttontextId
            x: redbuttonimageId.x + redbuttonimageId.width + xmargin
            anchors.verticalCenter: redbuttonimageId.verticalCenter
            visible: enableredbutton
            text: redbuttonmessage
            font.family: fontId.name
            font.pointSize: 15
            width: redbuttontextId.implicitWidth
            height: redbuttontextId.implicitHeight
            color: Qt.rgba(.5, .5, .5, 1)
        }
    }


    //Black Button Hugs the Right Screen
    Item
    {
        id: blackbuttonitemId
        x: redbuttonitemId.x + redbuttonitemId.width + buttonspacing
        //x: parent.width - blackbuttonitemId.width - imagemargin
        y: parent.height - blackbuttonitemId.height - ymargin
        width: blackbuttontextId.x + blackbuttontextId.implicitWidth
        height: blackbuttonimageId.height
        Image
        {
            id: blackbuttonimageId
            x:0
            y:0
            width:buttonsize
            height:buttonsize
            visible: enableblackbutton
            source: "qrc:/Icons/BlackButton2.png"
            opacity: .90
        }

        Text
        {
            id: blackbuttontextId
            x: blackbuttonimageId.x + blackbuttonimageId.width + xmargin
            anchors.verticalCenter: blackbuttonimageId.verticalCenter
            visible: enableblackbutton
            text: blackbuttonmessage
            font.family: fontId.name
            font.pointSize: 15
            width: blackbuttontextId.implicitWidth
            height: blackbuttontextId.implicitHeight
            color: Qt.rgba(.5, .5, .5, 1)
        }
    }

}
