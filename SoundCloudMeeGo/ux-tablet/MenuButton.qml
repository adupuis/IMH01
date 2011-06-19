import QtQuick 1.0

BorderImage {
    property alias buttonPressed: mouseArea.pressed
    property bool buttonSelected : false
    id: borderImage
    source: "images/base-button.png"
    border { left: 15; top: 15; right: 15; bottom: 15 }
    horizontalTileMode: BorderImage.Round
    verticalTileMode: BorderImage.Round
    MouseArea
    {
        anchors.fill: parent
        id: mouseArea
        onClicked: callback()
    }
    states:[
        State
        {
            name: "hovered";
            when: mouseArea.pressed == false;
            PropertyChanges { target: borderImage; opacity: 1;}
        },
        State
        {
            name: "normal"
            when: mouseArea.pressed ;
            PropertyChanges { target: borderImage; source: "images/base-button-shadow.png"; }
        }
    ]
}
