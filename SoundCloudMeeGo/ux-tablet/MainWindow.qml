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
            buttonSelected: true
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:  "Dashboard"
                font.pointSize: 13
            }
            function callback(){
                console.log("Menu button clicked: Dashboard")
                menu_dashboard.buttonSelected = true
                menu_groups.buttonSelected = false
                menu_people.buttonSelected = false
                menu_you.buttonSelected = false
                menu_tracks.buttonSelected = false
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
                menu_dashboard.buttonSelected = false
                menu_groups.buttonSelected = false
                menu_people.buttonSelected = false
                menu_you.buttonSelected = true
                menu_tracks.buttonSelected = false
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
                menu_dashboard.buttonSelected = false
                menu_groups.buttonSelected = false
                menu_people.buttonSelected = false
                menu_you.buttonSelected = false
                menu_tracks.buttonSelected = true
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
                menu_dashboard.buttonSelected = false
                menu_groups.buttonSelected = false
                menu_people.buttonSelected = true
                menu_you.buttonSelected = false
                menu_tracks.buttonSelected = false
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
                menu_dashboard.buttonSelected = false
                menu_groups.buttonSelected = true
                menu_people.buttonSelected = false
                menu_you.buttonSelected = false
                menu_tracks.buttonSelected = false
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
    Rectangle {
        x:160
        y: 80
        width: 1356-160
        Image {
            id: indicator
            x: 50
            y: 2
            source: "images/indicator.png"
        }
        states:[
            State
            {
                name: "indicator-dashboard";
                when: menu_dashboard.buttonSelected
                PropertyChanges { target: indicator; x: 50;}
            },
            State
            {
                name: "indicator-you"
                when: menu_you.buttonSelected ;
                PropertyChanges { target: indicator; x: 170; }
            },
            State
            {
                name: "indicator-tracks"
                when: menu_tracks.buttonSelected ;
                PropertyChanges { target: indicator; x: 290; }
            },
            State
            {
                name: "indicator-people"
                when: menu_people.buttonSelected ;
                PropertyChanges { target: indicator; x: 410; }
            },
            State
            {
                name: "indicator-groups"
                when: menu_groups.buttonSelected ;
                PropertyChanges { target: indicator; x: 530; }
            }
        ]
        transitions: [
            Transition {
                from: "*"; to: "indicator-you"; reversible: true
                NumberAnimation { properties: "x"; duration: 200; easing.type: Easing.Linear }
            },
            Transition {
                from: "*"; to: "indicator-tracks"; reversible: true
                NumberAnimation { properties: "x"; duration: 200; easing.type: Easing.Linear }
            },
            Transition {
                from: "*"; to: "indicator-groups"; reversible: true
                NumberAnimation { properties: "x"; duration: 200; easing.type: Easing.Linear }
            },
            Transition {
                from: "*"; to: "indicator-people"; reversible: true
                NumberAnimation { properties: "x"; duration: 200; easing.type: Easing.Linear }
            },
            Transition {
                NumberAnimation { properties: "x"; duration: 200; easing.type: Easing.Linear }
            }
        ]
    }
    Rectangle {
        id: mainarea
        x: 20
        y: 110
        LoginScreen {
            id: login
            x:0
            y:0

            function login(login,password) {
                window.login(login,password);
            }
        }
        Spectrum {
            id: spectrum
            x: 40
            y: 90
            height: 200
            width:1200
        }
        function onUpdateProgress(ratio) {
            spectrum.onUpdateProgress(ratio);
        }
        function spectrumVisible() {
            login.visible = false;
            spectrum.setVisibile();
        }

        Dashboard {
            id: dashboard
            x:0
            y:0
        }
        function dashboardVisible() {
            login.visible = false;
            dashboard.visible = true;
        }

        function onSetWaveForm(img) {
            spectrum.spectrum.source = img;
        }
        function playerSeek(param) {
            window.playerSeek(param, spectrum.width);

        }
    }
}
