import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item
{
    anchors.fill: parent
    MessageDialog
    {
        id: messageDialog

        title: "Apartment Management System"

        buttons: MessageDialog.Ok
    }
    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 20

        spacing: 15

        Label
        {
            text:
                "Generated Bills (" +
                billViewModel.billCount +
                ")"

            font.pixelSize: 20
            color: "#3B3B98"
            font.bold: true
        }

        GridLayout
        {
            columns: 2

            rowSpacing: 10
            columnSpacing: 20

            Label
            {
                text: "Bill Month"
            }

            ComboBox
            {
                id: cmbMonth

                model:
                [
                    1,2,3,4,5,6,
                    7,8,9,10,11,12
                ]

                Layout.preferredWidth: 150
            }

            Label
            {
                text: "Bill Year"
            }

            SpinBox
            {
                id: spnYear

                from: 2020
                to: 2100

                value: 2026

                Layout.preferredWidth: 150
            }

            Label
            {
                text: "Rate Per Unit"
            }

            TextField
            {
                id: txtRate

                text: "8.50"

                Layout.preferredWidth: 150
            }

            Label
            {
                text: "Maintenance (1BHK)"
            }

            TextField
            {
                id: txt1

                text: "1500"

                Layout.preferredWidth: 150
            }

            Label
            {
                text: "Maintenance (2BHK)"
            }

            TextField
            {
                id: txt2

                text: "2000"

                Layout.preferredWidth: 150
            }

            Label
            {
                text: "Maintenance (3BHK)"
            }

            TextField
            {
                id: txt3

                text: "2500"

                Layout.preferredWidth: 150
            }
        }

        Button
        {
            text: "Generate Bills"

            Layout.preferredWidth: 180
            onClicked:
            {
                billViewModel.generateBills(
                            cmbMonth.model[cmbMonth.currentIndex],
                            spnYear.value,
                            Number(txtRate.text),
                            Number(txt1.text),
                            Number(txt2.text),
                            Number(txt3.text))
            }
        }
        Label
        {
            text: "Generated Bills"

            font.bold: true
        }

        Rectangle
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            border.width: 1
            color: "white"

            Column
            {
                anchors.fill: parent

                spacing: 0

                //--------------------------------------------------
                // Header
                //--------------------------------------------------

                Rectangle
                {
                    width: parent.width
                    height: 40

                    color: "#D6D6D6"

                    RowLayout
                    {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10

                        Label
                        {
                            text: "Flat"

                            font.bold: true

                            Layout.preferredWidth: 70
                        }

                        Label
                        {
                            text: "Month"

                            font.bold: true

                            Layout.preferredWidth: 60
                        }

                        Label
                        {
                            text: "Year"

                            font.bold: true

                            Layout.preferredWidth: 60
                        }

                        Label
                        {
                            text: "Prev"

                            font.bold: true

                            Layout.preferredWidth: 70
                        }

                        Label
                        {
                            text: "Current"

                            font.bold: true

                            Layout.preferredWidth: 70
                        }

                        Label
                        {
                            text: "Units"

                            font.bold: true

                            Layout.preferredWidth: 60
                        }

                        Label
                        {
                            text: "Rate"

                            font.bold: true

                            Layout.preferredWidth: 80
                        }

                        Label
                        {
                            text: "Electricity"

                            font.bold: true

                            Layout.preferredWidth: 110
                        }

                        Label
                        {
                            text: "Maintenance"

                            font.bold: true

                            Layout.preferredWidth: 120
                        }

                        Label
                        {
                            text: "Total"

                            font.bold: true

                            Layout.fillWidth: true
                        }
                    }
                }

                //--------------------------------------------------
                // Scrollable List
                //--------------------------------------------------

                ScrollView
                {
                    width: parent.width
                    height: parent.height - 40

                    clip: true

                    ScrollBar.vertical.policy: ScrollBar.AsNeeded
                    ScrollBar.horizontal.policy: ScrollBar.AsNeeded

                    ListView
                    {
                        id: billList

                        width: parent.width

                        model: billViewModel.generatedBillModel

                        spacing: 0

                        delegate: Rectangle
                        {
                            width: billList.width
                            height: 38

                            border.width: 1

                            color:
                                index % 2 === 0
                                ? "#F8F8F8"
                                : "white"

                            RowLayout
                            {
                                anchors.fill: parent

                                anchors.leftMargin: 10
                                anchors.rightMargin: 10

                                Text
                                {
                                    text: flatNo

                                    font.bold: true

                                    Layout.preferredWidth: 70
                                }

                                Text
                                {
                                    text: billMonth

                                    Layout.preferredWidth: 60
                                }

                                Text
                                {
                                    text: billYear

                                    Layout.preferredWidth: 60
                                }

                                Text
                                {
                                    text: previousReading

                                    Layout.preferredWidth: 70
                                }

                                Text
                                {
                                    text: currentReading

                                    Layout.preferredWidth: 70
                                }

                                Text
                                {
                                    text: unitsConsumed

                                    Layout.preferredWidth: 60
                                }

                                Text
                                {
                                    text: Number(ratePerUnit).toFixed(2)

                                    Layout.preferredWidth: 80
                                }

                                Text
                                {
                                    text: Number(electricityAmount).toFixed(2)

                                    Layout.preferredWidth: 110
                                }

                                Text
                                {
                                    text: Number(maintenanceAmount).toFixed(2)

                                    Layout.preferredWidth: 120
                                }

                                Text
                                {
                                    text: Number(totalAmount).toFixed(2)

                                    font.bold: true

                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Connections
    {
        target: billViewModel

        function onOperationCompleted(success, message)
        {
            messageDialog.text = message
            messageDialog.open()
        }
    }
}