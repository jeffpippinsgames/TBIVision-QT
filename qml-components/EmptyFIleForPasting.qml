import QtQuick 2.0


//Post Blur Settings Rectangle---------------------------------------
Rectangle
{
    id: postblursettingsrectId
    visible: false
    width: rootpageId.settingsrectwidth
    height: rootpageId.settingsrectheight
    x: rootpageId.settingsrectx
    y: rootpageId.settingsrecty
    color: "transparent"

    function grabFocus()
    {
        postblursettingscontrollerId.focus = true;

    }

    states:
        [
        State
        {
            name: "NonFocused"
            PropertyChanges{target: postblursettingsrectId; border.color:rootpageId.nonfocuscolor;}
            PropertyChanges{target: postblursliderId; state: postblursliderId.nothighlightedstate;}

        },
        State
        {
            name: "BlurState"
            PropertyChanges{target: postblursettingsrectId; border.color:rootpageId.focuscolor;}
            PropertyChanges{target: postblursliderId; state: postblursliderId.highlightedstate;}
        }
    ]

    ControllerObject
    {

        id:postblursettingscontrollerId
        focus: false

        onFocusChanged:
        {
            if(focus)
            {
                controlkeyId.enableblackbutton = false;
            }
        }

        onBlackButtonpostssed:
        {

        }

        onGreenButtonpostssed:
        {
            switch(postblursettingsrectId.state)
            {
            case "BlurState":
                postblursliderId.grabFocus();
                break;
            }
        }

        onRedButtonpostssed:
        {

        }

        onUpButtonpostssed:
        {
            postblursettingscontrollerId.state = "NonFocused"
            rootpageId.state = rootpageId.postblurstateframeactive;
        }

        onDownButtonpostssed:
        {

        }

        onLeftButtonpostssed:
        {

        }

        onRightButtonpostssed:
        {

        }

        onUpButtonReleased:
        {

        }

        onDownButtonReleased:
        {

        }

        onLeftButtonReleased:
        {

        }

        onRightButtonReleased:
        {

        }
    }

    Rectangle
    {
        anchors.fill: postblursettingsrectId
        color: "black"
        opacity: .8
        radius: 10
    }

    Rectangle
    {
        anchors.fill: postblursettingsrectId
        color: "darkred"
        opacity: .05
        radius: 10
    }

    Rectangle
    {
        id: postblursettingsborderrectId
        anchors.fill: postblursettingsrectId
        color: "transparent"
        border.color: rootpageId.nonfocuscolor
        border.width: rootpageId.rectborderwidths
        radius: 10
    }

    //Settings Text
    Text
    {
        id: postblurtextId
        font.family: fontId.name
        y: 10
        x:10
        text: "Post Edge Blur Frame Settings"
        font.pixelSize: 25
        color: rootpageId.textcolor
    }

    SliderSettingsObject
    {
        id: postblursliderId
        controlname: "PostBlurSliderControl"
        x:20
        y:60
        messagetext: "Blur: " + VGrooveGausianDistroParameters.postBlurValue;
        valuefrom: 1
        valueto: 49
        stepsize: 2
        majorstepsize: 4
        controlstickautorepeat: true

        Component.onCompleted:
        {
            postblursliderId.value = VGrooveGausianDistroParameters.postBlurValue;
        }

        onEndFocus:
        {
            postblursettingsrectId.grabFocus();
            postblursliderId.state = postblursliderId.highlightedstate;
        }

        onValueChanged:
        {
            VGrooveGausianDistroParameters.postBlurValue = postblursliderId.value;
        }


    }

}
