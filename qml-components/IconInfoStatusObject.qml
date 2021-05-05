import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12
import "qrc:/qml-components"
import "qrc:/qml-pages"
import tbi.vision.components 1.0

Rectangle
{
    id: rootcomponentId
    width: 300
    height: 300
    color: "transparent"
    visible: true
    property string imagesource: "qrc:/Icons/microcontroller(white).png"
    property string messagetext: "Ok"
    property color glowcolor: Qt.rgba(0,1,0,1)
    property color textcolor: Qt.rgba(1,1,.95,1)
    property int glowradius: 10
    property int glowsamples: 10
    //property string fontsource: "qrc:/Fonts/Blueprint BoldItalic.ttf"
    //property string fontsource: "qrc:/Fonts/EurostileBold.ttf"
    //property string fontsource: "qrc:/Fonts/Measurements.ttf"
    //property string fontsource: "qrc:/Fonts/Typo Draft Demo.otf"
    property string fontsource: "qrc:/Fonts/PermanentMarker-Regular.ttf"
    property int textsize: 30


    //Font for UI
    FontLoader
    {
        id: fontId
        source: rootcomponentId.fontsource
    }

    Image
    {
       id: imageId
       source: rootcomponentId.imagesource
       fillMode: Image.PreserveAspectFit
       y:5
       anchors.horizontalCenter: rootcomponentId.horizontalCenter
       width: rootcomponentId.width - 30
       height: rootcomponentId.height - 30
       smooth: true
       visible: rootcomponentId.visible
    }

    Glow
    {
        anchors.fill: imageId
        visible: rootcomponentId.visible
        color: rootcomponentId.glowcolor
        samples: rootcomponentId.glowsamples
        radius: rootcomponentId.glowradius
    }

    Text
    {
        id: textId
        font.family: rootcomponentId.fontsource
        font.pixelSize: rootcomponentId.textsize
        text: rootcomponentId.messagetext
        visible: rootcomponentId.visible
        anchors.horizontalCenter: rootcomponentId.horizontalCenter
        width: textId.implicitWidth
        height: textId.implicitHeight
        y: rootcomponentId.height - textId.height
    }

    Glow
    {
        anchors.fill: textId
        visible: rootcomponentId.visible
        color: rootcomponentId.glowcolor
        samples: rootcomponentId.glowsamples
        radius: rootcomponentId.glowradius
    }

}
