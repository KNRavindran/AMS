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

        buttons:
            MessageDialog.Yes |
            MessageDialog.No

        text:
            "Delete meter reading for Flat " +
            cmbFlatNo.currentText +
            " ?"

        onButtonClicked: function(button)
        {
            if (button === MessageDialog.Yes)
            {
                meterReadingViewModel.deleteMeterReading(
                            cmbFlatNo.currentText,
                            cmbMonth.model[cmbMonth.currentIndex],
                            spnYear.value)
            }
        }
    }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 20

        spacing: 15

        Label
        {
            text:
                "Meter Reading (" +
                meterReadingViewModel.meterReadingCount +
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
                text: "Flat Number"
            }

            ComboBox
            {
                id: cmbFlatNo

                Layout.preferredWidth: 150

                model: flatViewModel.flatModel

                textRole: "flatNo"
            }

            Label
            {
                text: "Month"
            }

            ComboBox
            {
                id: cmbMonth

                Layout.preferredWidth: 150

                model:
                [
                    1,2,3,4,5,6,
                    7,8,9,10,11,12
                ]
            }

            Label
            {
                text: "Year"
            }

            SpinBox
            {
                id: spnYear

                Layout.preferredWidth: 150

                from: 2020
                to: 2100

                value: 2026
            }

            Label
            {
                text: "Meter Reading"
            }

            TextField
            {
                id: txtReading

                Layout.preferredWidth: 150
            }
        }

        RowLayout
        {
            spacing: 10
            /*
            Button
            {
                text: "Add"

                Layout.preferredWidth: 100

                onClicked:
                {
                    meterReadingViewModel.addMeterReading(
                                cmbFlatNo.currentText,
                                cmbMonth.model[cmbMonth.currentIndex],
                                spnYear.value,
                                Number(txtReading.text))
                }
            }
            */
            Button
            {
                text: "Add"

                onClicked:
                {
                    var success =
                            meterReadingViewModel.addMeterReading(
                                cmbFlatNo.currentText,
                                cmbMonth.model[cmbMonth.currentIndex],
                                spnYear.value,
                                Number(txtReading.text))

                    if (success)
                    {
                        console.log("Meter Reading Added")

                        txtReading.text = ""

                        txtReading.forceActiveFocus()
                    }
                    else
                    {
                        console.log("Failed")
                    }
                }
            }
            Button
            {
                text: "Update"

                Layout.preferredWidth: 100

                onClicked:
                {
                    meterReadingViewModel.updateMeterReading(
                                cmbFlatNo.currentText,
                                cmbMonth.model[cmbMonth.currentIndex],
                                spnYear.value,
                                Number(txtReading.text))
                }
            }

            Button
            {
                text: "Delete"

                Layout.preferredWidth: 100

                onClicked:
                {
                    if (cmbFlatNo.currentText === "")
                    {
                        messageDialog.text =
                                "Please select a meter reading."

                        messageDialog.open()

                        return
                    }

                    deleteConfirmDialog.open()
                }
            }

            Button
            {
                text: "Clear"

                Layout.preferredWidth: 100

                onClicked:
                {
                    meterReadingViewModel.clearSelection()
                }
            }
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

                //------------------------------------------------------
                // Header
                //------------------------------------------------------

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
                            text: "Flat No"

                            font.bold: true

                            Layout.preferredWidth: 120
                        }

                        Label
                        {
                            text: "Month"

                            font.bold: true

                            Layout.preferredWidth: 80
                        }

                        Label
                        {
                            text: "Year"

                            font.bold: true

                            Layout.preferredWidth: 80
                        }

                        Label
                        {
                            text: "Reading"

                            font.bold: true

                            Layout.fillWidth: true
                        }
                    }
                }

                //------------------------------------------------------
                // Scrollable List
                //------------------------------------------------------

                ScrollView
                {
                    width: parent.width
                    height: parent.height - 40

                    clip: true

                    ScrollBar.vertical.policy: ScrollBar.AsNeeded
                    ScrollBar.horizontal.policy: ScrollBar.AsNeeded

                    ListView
                    {
                        id: meterReadingList

                        width: parent.width

                        model: meterReadingViewModel.meterReadingModel

                        spacing: 0

                        delegate: Rectangle
                        {
                            width: meterReadingList.width
                            height: 36

                            border.width: 1

                            color:
                                index === meterReadingViewModel.selectedRow
                                ? "#A7D8FF"
                                : (index % 2 === 0
                                   ? "#F8F8F8"
                                   : "white")

                            MouseArea
                            {
                                anchors.fill: parent

                                onClicked:
                                {
                                    meterReadingViewModel.selectMeterReading(index)
                                }
                            }

                            RowLayout
                            {
                                anchors.fill: parent

                                anchors.leftMargin: 10
                                anchors.rightMargin: 10

                                Text
                                {
                                    text: flatNo

                                    Layout.preferredWidth: 120
                                }

                                Text
                                {
                                    text: readingMonth

                                    Layout.preferredWidth: 80
                                }

                                Text
                                {
                                    text: readingYear

                                    Layout.preferredWidth: 80
                                }

                                Text
                                {
                                    text: meterReading

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
        target: meterReadingViewModel

        function onOperationCompleted(success, message)
        {
            messageDialog.text = message
            messageDialog.open()
        }

        function onSelectedMeterReadingChanged()
        {
            // Flat Number
            if (meterReadingViewModel.selectedFlatNo === "")
            {
                cmbFlatNo.currentIndex = 0
            }
            else
            {
                for (var i = 0; i < cmbFlatNo.count; ++i)
                {
                    if (cmbFlatNo.textAt(i) === meterReadingViewModel.selectedFlatNo)
                    {
                        cmbFlatNo.currentIndex = i
                        break
                    }
                }
            }

            // Month
            if (meterReadingViewModel.selectedMonth > 0)
                cmbMonth.currentIndex =
                        meterReadingViewModel.selectedMonth - 1
            else
                cmbMonth.currentIndex = 0

            // Year
            if (meterReadingViewModel.selectedYear > 0)
                spnYear.value =
                        meterReadingViewModel.selectedYear
            else
                spnYear.value = 2026

            // Reading
            if (meterReadingViewModel.selectedReading > 0)
                txtReading.text =
                        meterReadingViewModel.selectedReading.toString()
            else
                txtReading.text = ""
        }
    }
}