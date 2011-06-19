import Qt 4.7
Rectangle {
    width:1326 
    height:638
    Image {
        source:"working-files/SoundCloudMeegoUXLoginScreen_images/background.png"
        id:background
        x:0 ; y:0
        width:1326
        height:638
        smooth: true
    }
    Text {
        id: login_label
        text: "login"
        x:278 ; y:47
        width:200
        height:40
        font.family: "Helvetica"
        font.pointSize: 20
        color: "black"
    }
    Image {
        source:"working-files/SoundCloudMeegoUXLoginScreen_images/login_editbox.png"
        id:login_editbox
        x:268 ; y:87
        width:714
        height:66
        smooth: true
    }
    TextInput {
        id:login_editbox_text
        text:"ouroz"
        x:278 ; y:97
        width:694
        height:46
        font.family: "Helvetica"
        font.pointSize: 20
        color: "black"
        focus: true
    }
    Text {
        id: password_label
        text: "password"
        x:278 ; y:159
        width:200
        height:40
        font.family: "Helvetica"
        font.pointSize: 20
        color: "black"
    }
    Image {
        source:"working-files/SoundCloudMeegoUXLoginScreen_images/password_editbox.png"
        id:password_editbox
        x:268 ; y:199
        width:714
        height:66
        smooth: true
    }
    TextInput {
        id:password_editbox_text
        text:"qwerty"
        x:278 ; y:209
        width:694
        height:46
        font.family: "Helvetica"
        font.pointSize: 20
        color: "black"
        echoMode: TextInput.Password
    }
    Image {
        source:"working-files/SoundCloudMeegoUXLoginScreen_images/login_button.png"
        id:login_button
        x:264 ; y:307
        width:338
        height:102
        smooth: true
        MouseArea
        {
          anchors.fill: parent
          id: mouseArea
          onClicked: login(login_editbox_text.text,password_editbox_text.text)
        }
        states:[
            State
            {
                name: "hovered";
                when: mouseArea.pressed;
                PropertyChanges { target: login_button; opacity: 0.7;}
            },
            State
            {
                name: "normal"
                when: mouseArea.pressed == false;
                PropertyChanges { target: login_button; opacity: 1; }
            }
        ]
    }
}
