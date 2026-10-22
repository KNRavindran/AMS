import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow
{
    id: mainWindow

    width: 1200
    height: 700
    //color:"cyan"
    visible: true

    title: "Apartment Management System"


    header: ToolBar
    {
        RowLayout
        {
            anchors.fill: parent

            Label
            {
                text: "Apartment Management System"
                color: "#3B3B98"
                font.pixelSize: 24
                font.bold: true

                Layout.leftMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    SplitView
    {
        anchors.fill: parent

        ListView
        {
            id: navigationView

            SplitView.preferredWidth: 220

            model:
            [
                "Flat Master",
                "Meter Reading",
                "Bill Generation",
                "View Bills"
            ]

            delegate: ItemDelegate
            {
                width: parent.width

                text: modelData

                onClicked:
                {
                    stackLayout.currentIndex = index
                }
            }
        }

        StackLayout
        {
            id: stackLayout

            SplitView.fillWidth: true
            SplitView.fillHeight: true

            FlatMasterScreen
            {
            }

            MeterReading
            {
            }

            BillGeneration
            {
            }

            BillListScreen
            {
            }
        }
    }
}