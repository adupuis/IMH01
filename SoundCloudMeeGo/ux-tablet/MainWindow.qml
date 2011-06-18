import QtQuick 1.0
import Qt 4.7

Rectangle {
    width:1366
    height:768
    Image {
        source:"working-files/SoundCloudMeegoUX_images/background.png"
        id:background
        x:0 ; y:0
        width:1366
        height:768
    }
    Image {
        source:"working-files/SoundCloudMeegoUX_images/home_button.png"
        id:home_button
        x:0 ; y:0
        width:151
        height:96
        MouseArea
        {
          anchors.fill: parent
          id: mouseArea
          onClicked: console.log("Home button clicked")
        }
    }
    WindowButton {
        id: quit_button
        x: 200
        y:20
        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text:  "Quit"
        }

        function callback(){
            window.quit();
        }
    }

    Image {
        source:"working-files/SoundCloudMeegoUX_images/main_display_area.png"
        id:main_display_area
        x:0 ; y:0
        width:1366
        height:768
    }
}
