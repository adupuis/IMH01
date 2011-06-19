import QtQuick 1.0

Rectangle {
    id : spect
    visible: false;
    MouseArea {
        id: mousearea
        anchors.fill: parent
        onPressed: { playerSeek(mouse.x) }
    }

    Image {
        source:"working-files/SoundCloudMeegoUX_Tracks_images/start.png"
        id:start_button
        x:862 ; y:100
        width:400
        height:163
        smooth: true
        MouseArea
        {
          anchors.fill: parent
          id: mouseArea
          onClicked: startSpectrum()
        }
        states:[
            State
            {
                name: "hovered";
                when: mouseArea.pressed;
                PropertyChanges { target: start_button; opacity: 0.7;}
            },
            State
            {
                name: "normal"
                when: mouseArea.pressed == false;
                PropertyChanges { target: start_button; opacity: 1; }
            }
        ]
    }
    Image {
        source:"working-files/SoundCloudMeegoUX_Tracks_images/stop.png"
        id:stop_button
        x:862 ; y:100
        width:400
        height:163
        smooth: true
        MouseArea
        {
          anchors.fill: parent
          id: mouseArea
          onClicked: stopSpectrum()
        }
        states:[
            State
            {
                name: "hovered";
                when: mouseArea.pressed;
                PropertyChanges { target: stop_button; opacity: 0.7;}
            },
            State
            {
                name: "normal"
                when: mouseArea.pressed == false;
                PropertyChanges { target: stop_button; opacity: 1; }
            }
        ]
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
