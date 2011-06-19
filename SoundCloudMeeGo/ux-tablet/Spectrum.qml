import QtQuick 1.0

Rectangle {
    Rectangle {
        id: played
        color: "#FF0000"
        width: 0
        height: parent.height
    }

    Rectangle {
        id: notplayed
        color: "#666666"
        x : 0
        width:  parent.width
        height: parent.height
    }


    Image {
        id: spectrum
        source: "http://w1.sndcdn.com/fxguEjG4ax6B_m.png"
        width: 1200
    }

    function onUpdateProgress(ratio) {
        played.width = played.parent.width * ratio;
        notplayed.width = notplayed.parent.width * (1.0 - ratio);
        notplayed.x = notplayed.parent.width * ratio;
    }
}
