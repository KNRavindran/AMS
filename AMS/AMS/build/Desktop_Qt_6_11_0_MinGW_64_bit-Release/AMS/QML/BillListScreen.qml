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

    MessageDialog
    {
        id: deleteConfirmDialog

        title: "Confirm Delete"
        /*
        text:
            "Delete bill for Flat " +
            billViewModel.selectedFlatNo +
            " ?"
        */
        text:
            "Delete " +
            billViewModel.selectedCount +
            " selected bill(s)?\n\nThis action cannot be undone."
        buttons:
            MessageDialog.Yes |
            MessageDialog.No

        onButtonClicked: function(button)
        {
            if (button === MessageDialog.Yes)
            {
                billViewModel.deleteSelectedBills()
            }
        }
    }
    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 0

        Label
        {
            text:
                "Generated Bills (" +
                billViewModel.billCount +
                ")"
            color: "#3B3B98"
            font.pixelSize: 20
            font.bold: true
        }
        RowLayout
        {
            Layout.fillWidth: true
            spacing: 10
            Button
            {
                text: "Delete Selected Bills"

                enabled: billViewModel.selectedCount > 0

                onClicked:
                {
                    deleteConfirmDialog.open()
                }
            }
            Button
            {
                text: "Export Selected PDFs"

                enabled: billViewModel.selectedCount > 0

                onClicked:
                {
                    billViewModel.exportSelectedBills()
                }
            }
            //
            RowLayout
            {
                spacing: 10

                TextField
                {
                    width: 100

                    placeholderText: "Flat"

                    text: billViewModel.filterFlatNo

                    onTextChanged:
                        billViewModel.filterFlatNo = text
                }

                SpinBox
                {
                    from: 0
                    to: 12

                    value: billViewModel.filterMonth

                    onValueChanged:
                        billViewModel.filterMonth = value
                }

                SpinBox
                {
                    from: 2020
                    to: 2100

                    value: billViewModel.filterYear

                    onValueChanged:
                        billViewModel.filterYear = value
                }

                Button
                {
                    text: "Search"

                    onClicked:
                        billViewModel.applyFilter()
                }

                Button
                {
                    text: "Clear Filters"

                    onClicked:
                        billViewModel.clearFilter()
                }
            }
            //
        }
        Rectangle
        {
            Layout.fillWidth: true

            height: 40

            color: "#D6D6D6"

            border.width: 1

            RowLayout
            {
                Layout.preferredWidth: 100

                spacing: 6

                CheckBox
                {
                    id: headerCheck

                    enabled: billViewModel.billCount > 0

                    onClicked:
                    {
                        if (checked)
                            billViewModel.selectAllBills()
                        else
                            billViewModel.clearAllSelections()
                    }

                    Connections
                    {
                        target: billViewModel

                        function onSelectedRowsChanged()
                        {
                            headerCheck.checked = billViewModel.allSelected
                        }
                    }

                    Component.onCompleted:
                    {
                        headerCheck.checked = billViewModel.allSelected
                    }
                }
                Label
                {
                    text: "Flat"

                    font.bold: true
                }

                Item
                {
                    width: 25    // Adjust between 8 and 16 until it looks right
                    height: 1
                }

                Label
                {
                    text: "Month"

                    font.bold: true

                    Layout.preferredWidth: 60
                }
                Item
                {
                    width: 9    // Adjust between 8 and 16 until it looks right
                    height: 1
                }
                Label
                {
                    text: "Year"

                    font.bold: true

                    Layout.preferredWidth: 60
                }
                Item
                {
                    //width: 1    // Adjust between 8 and 16 until it looks right
                    height: 1
                }
                Label
                {
                    text: "Previous"

                    font.bold: true

                    Layout.preferredWidth: 90
                }
                Item
                {
                    width: 1    // Adjust between 8 and 16 until it looks right
                    height: 1
                }
                Label
                {
                    text: "Current"

                    font.bold: true

                    Layout.preferredWidth: 90
                }
                Item
                {
                    //width: 1    // Adjust between 8 and 16 until it looks right
                    height: 1
                }
                Label
                {
                    text: "Units"

                    font.bold: true

                    Layout.preferredWidth: 70
                }
                Item
                {
                    //width: 12    // Adjust between 8 and 16 until it looks right
                    height: 1
                }
                Label
                {
                    text: "Rate"

                    font.bold: true

                    Layout.preferredWidth: 70
                }
                Item
                {
                    //width: 12    // Adjust between 8 and 16 until it looks right
                    height: 1
                }
                Label
                {
                    text: "Electricity"

                    font.bold: true

                    Layout.preferredWidth: 110
                }
                Item
                {
                    //width: 12    // Adjust between 8 and 16 until it looks right
                    height: 1
                }
                Label
                {
                    text: "Maintenance"

                    font.bold: true

                    Layout.preferredWidth: 120
                }
                Item
                {
                    //width: 12    // Adjust between 8 and 16 until it looks right
                    height: 1
                }
                Label
                {
                    text: "Total"

                    font.bold: true

                    Layout.fillWidth: true
                }
            }
        }

        ScrollView
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true

            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AsNeeded

            ListView
            {
                id: billList

                width: parent.width

                model: billViewModel.billModel

                spacing: 0

                delegate: Rectangle
                {
                    width: billList.width
                    height: 38

                    border.width: 1

                    color:
                        selected
                        ? "#A7D8FF"
                        : (index % 2 === 0
                           ? "#FFFFFF"
                           : "#F8F8F8")



                    RowLayout
                    {
                        Layout.preferredWidth: 100

                        spacing: 6


                        CheckBox
                        {
                            checked: selected

                            onClicked:
                            {
                                billViewModel.toggleSelection(index)
                            }
                        }
                        Label
                        {
                            text: flatNo

                            Layout.preferredWidth: 70
                        }

                        Label
                        {
                            text: billMonth
                            Layout.preferredWidth: 60
                        }

                        Label
                        {
                            text: billYear
                            Layout.preferredWidth: 60
                        }

                        Label
                        {
                            text: previousReading
                            Layout.preferredWidth: 100
                        }

                        Label
                        {
                            text: currentReading
                            Layout.preferredWidth: 100
                        }

                        Label
                        {
                            text: unitsConsumed
                            Layout.preferredWidth: 75
                        }

                        Label
                        {
                            text: Number(ratePerUnit).toFixed(2)
                            Layout.preferredWidth: 80
                        }

                        Label
                        {
                            text: Number(electricityAmount).toFixed(2)
                            Layout.preferredWidth: 110
                        }

                        Label
                        {
                            text: Number(maintenanceAmount).toFixed(2)
                            Layout.preferredWidth: 130
                        }

                        Label
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