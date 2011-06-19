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
    Image {
        source:"working-files/SoundCloudMeegoUXLoginScreen_images/login_editbox.png"
        id:login_editbox
        x:268 ; y:113
        width:714
        height:66
        smooth: true
    }
    TextInput {
        id:login_editbox_text
        x:278 ; y:123
        width:694
        height:46
        font.family: "Helvetica"
        font.pointSize: 20
        color: "black"
        focus: true
    }
    Image {
        source:"working-files/SoundCloudMeegoUXLoginScreen_images/password_editbox.png"
        id:password_editbox
        x:268 ; y:264
        width:714
        height:66
        smooth: true
    }
    TextInput {
        id:password_editbox_text
        x:278 ; y:274
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
        x:264 ; y:399
        width:338
        height:102
        smooth: true
        MouseArea
        {
          anchors.fill: parent
          id: mouseArea
          onClicked: login(login_editbox_text.text,password_editbox_text.text)
        }
    }
}
