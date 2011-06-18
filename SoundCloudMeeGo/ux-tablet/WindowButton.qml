import QtQuick 1.0

Rectangle {
    id: button
    width: 60
    height: 20
    radius: 5.0
    smooth:  true
    border.color: "#bbb5b5"
    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#e5e5e5";
        }
        GradientStop {
            position: 0.5;
            color: "#ffffff";
        }
        GradientStop {
            position: 1.00;
            color: "#e5e5e5";
        }
    }
    MouseArea
    {
        anchors.fill: parent
        id: mouseArea
        onClicked: callback()
    }
}
