import QtQuick 1.0
import Qt 4.7
Rectangle {
    width:1326
    height:638
    Image {
        source:"working-files/SoundCloudMeegoUX_You_images/bg.png"
        id:bg
        x:0 ; y:0
        width:1326
        height:638
    }
    Text {
        text:'Tracks'
        font.pixelSize:30
        color: "#0066cc"
        id:tracks
        x:118 ; y:272
        width:108
        height:36
    }
    Text {
        text:'Sets'
        font.pixelSize:30
        color: "#0066cc"
        id:sets
        x:456 ; y:272
        width:74
        height:36
    }
    Text {
        text:'Comments'
        font.pixelSize:30
        color: "#0066cc"
        id:comments
        x:739 ; y:272
        width:178
        height:36
    }
    Text {
        text:'Favorites'
        font.pixelSize:30
        color: "#0066cc"
        id:favorites
        x:1086 ; y:272
        width:157
        height:36
    }
    Image {
        source:"working-files/SoundCloudMeegoUX_You_images/trackslist.png"
        id:trackslist
        x:0 ; y:0
        width:1326
        height:638
        ListModel {
            id: tracksModel

            ListElement {
                img: "images/tracks01.png"
            }
            ListElement {
                img: "images/tracks02.png"
            }
        }
        ListView {
            x: 26
            y: 335
            width: 286
            height: 250
            model: tracksModel
            delegate: Row {
                Image {
                    source: img
                    width: 282
                }
            }
        }
    }
    Image {
        source:"working-files/SoundCloudMeegoUX_You_images/setslist.png"
        id:setslist
        x:0 ; y:0
        width:1326
        height:638
        ListModel {
            id: setsModel

            ListElement {
                img: "images/tracks03.png"
            }
        }
        ListView {
            x: 358
            y: 335
            width: 286
            height: 250
            model: setsModel
            delegate: Row {
                Image {
                    source: img
                    width: 282
                }
            }
        }
    }
    Image {
        source:"working-files/SoundCloudMeegoUX_You_images/commentslist.png"
        id:commentslist
        x:0 ; y:0
        width:1326
        height:638
        ListModel {
            id: commentModel

            ListElement {
                img: "images/comment.png"
            }
            ListElement {
                img: "images/comment.png"
            }
        }
        ListView {
            x: 687
            y: 330
            width: 288
            height: 250
            model: commentModel
            delegate: Row {
                Image {
                    source: img
                    width: 285
                }
            }
        }
    }
    Image {
        source:"working-files/SoundCloudMeegoUX_You_images/favoriteslist.png"
        id:favoriteslist
        x:0 ; y:0
        width:1326
        height:638
        ListModel {
            id: favModel

            ListElement {
                img: "images/tracks03.png"
            }
            ListElement {
                img: "images/tracks01.png"
            }
        }
        ListView {
            x: 1017
            y: 335
            width: 286
            height: 250
            model: favModel
            delegate: Row {
                Image {
                    source: img
                    width: 282
                }
            }
        }
    }
    Image {
        source:"working-files/SoundCloudMeegoUX_You_images/profileinfo.png"
        id:profileinfo
        x:35 ; y:80
        width:353
        height:107
    }
    Text {
        text:'Profile'
        font.pixelSize:44
        id:profile
        x:29 ; y:14
        width:162
        height:52
        color: "#0066cc"
    }
    Image {
        source:"working-files/SoundCloudMeegoUX_You_images/uprec.png"
        id:uprec
        x:0 ; y:0
        width:1326
        height:638
    }
}



