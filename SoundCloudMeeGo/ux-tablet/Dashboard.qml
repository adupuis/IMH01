import Qt 4.7
Rectangle {
    id: dashboard
    width:1326 
    height:638
    Image {
        source:"working-files/SoundCloudMeegoUXDashboard_images/background.png"
        id:background
        x:0 ; y:0
        width:1326
        height:638
    }
    Image {
        source:"working-files/SoundCloudMeegoUXDashboard_images/updates.png"
        id:updates
        x:862 ; y:54
        width:400
        height:163
    }
    Image {
        source:"working-files/SoundCloudMeegoUXDashboard_images/following.png"
        id:following
        x:862 ; y:273
        width:400
        height:310
    }
    Image {
        source:"working-files/SoundCloudMeegoUXDashboard_images/activities.png"
        id:activities
        x:75 ; y:53
        width:729
        height:530
    }
}
