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
        states:[
            State
            {
                name: "hovered";
                when: mouseArea.pressed;
                PropertyChanges { target: home_button; opacity: 0.7;}
            },
            State
            {
                name: "normal"
                when: mouseArea.pressed == false;
                PropertyChanges { target: home_button; opacity: 1; }
            }
        ]
    }
    Rectangle {
        id: quit_button
        x: 1292
        y:20
        width: 64
        height: 64
        Image {
            source: "images/application-exit.png"
            states:[
                State
                {
                    name: "hovered";
                    when: quitMouseArea.pressed;
                    PropertyChanges { target: quit_button; opacity: 1;}
                },
                State
                {
                    name: "normal"
                    when: quitMouseArea.pressed == false;
                    PropertyChanges { target: quit_button; opacity: 0.7; }
                }
            ]
            MouseArea
            {
                id: quitMouseArea
                anchors.fill: parent
                onClicked: window.quit()
            }
        }
    }
    Row {
        x: 160
        y: 10
        MenuButton {
            id: menu_dashboard
            width: 120
            height: 70
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:  "Dashboard"
                font.family: "Liberation Sans Narrow"
                font.pointSize: 13
            }
            function callback(){
                console.log("Menu button clicked: Dashboard")
            }
        }
        MenuButton {
            id: menu_you
            width: 120
            height: 70
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:  "You"
                font.pointSize: 13
            }
            function callback(){
                console.log("Menu button clicked: You")
            }
        }
        MenuButton {
            id: menu_tracks
            width: 120
            height: 70
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:  "Tracks"
                font.pointSize: 13
            }
            function callback(){
                console.log("Menu button clicked: Tracks")
            }
        }
        MenuButton {
            id: menu_people
            width: 120
            height: 70
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:  "People"
                font.pointSize: 13
            }
            function callback(){
                console.log("Menu button clicked: People")
            }
        }
        MenuButton {
            id: menu_groups
            width: 120
            height: 70
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:  "Groups"
                font.pointSize: 13
            }
            function callback(){
                console.log("Menu button clicked: Groups")
            }
        }
    }

    Image {
        source:"working-files/SoundCloudMeegoUX_images/main_display_area.png"
        id:main_display_area
        x:0 ; y:0
        width:1366
        height:768
    }
    Image {
        id: indicator
        x: 210
        y: 82
        source: "images/indicator.png"
    }
}
