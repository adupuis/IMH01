import QtQuick 1.0

Rectangle {
    id : spect
    visible: false;
    MouseArea {
        id: mousearea
        anchors.fill: parent
        onPressed: { playerSeek(mouse.x) }
    }

    Rectangle {
        id: played
        color: "#FF0000"
        width: 0
        height: spectrum.height
    }

    Rectangle {
        id: notplayed
        color: "#666666"
        x : 0
        width:  parent.width
        height: spectrum.height
    }


    Image {
        id: spectrum
        source : "local.png" //FAKE !!
        width: 1200
    }

    function onUpdateProgress(ratio) {
        played.width = played.parent.width * ratio;
        notplayed.width = notplayed.parent.width * (1.0 - ratio);
        notplayed.x = notplayed.parent.width * ratio;
    }

    function setVisibile() {
        visible = true;
    }
}
