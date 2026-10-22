import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page
{
    title: "Bills"

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 10

        Label
        {
            text: "Generated Bills"
            font.pixelSize: 22
            font.bold: true
        }

        Rectangle
        {
            Layout.fillWidth: true
            height: 40
            color: "#dcdcdc"

            Row
            {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 25

                Text { text: "Flat"; width: 60; font.bold: true }
                Text { text: "Month"; width: 60; font.bold: true }
                Text { text: "Year"; width: 60; font.bold: true }
                Text { text: "Units"; width: 60; font.bold: true }
                Text { text: "Electricity"; width: 90; font.bold: true }
                Text { text: "Maintenance"; width: 100; font.bold: true }
                Text { text: "Total"; width: 90; font.bold: true }
            }
        }

        ListView
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true

            model: billViewModel.billListModel

            delegate: Rectangle
            {
                width: ListView.view.width
                height: 40

                color: index % 2 ? "#f8f8f8" : "white"

                Row
                {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 25

                    Text { text: flatNo; width: 60 }
                    Text { text: billMonth; width: 60 }
                    Text { text: billYear; width: 60 }
                    Text { text: unitsConsumed; width: 60 }
                    Text { text: electricityAmount.toFixed(2); width: 90 }
                    Text { text: maintenanceAmount.toFixed(2); width: 100 }
                    Text { text: totalAmount.toFixed(2); width: 90 }
                }
            }
        }
    }
}