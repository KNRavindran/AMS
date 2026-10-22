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

        text:
            "Delete Flat No. " +
            txtFlatNo.text +
            " ?"

        buttons:
            MessageDialog.Yes |
            MessageDialog.No

        onButtonClicked: function(button)
        {
            if (button === MessageDialog.Yes)
            {
                flatViewModel.deleteFlat(
                            txtFlatNo.text)
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
                "Flat Master (" +
                flatViewModel.flatCount +
                ")"

            font.pixelSize: 20

            font.bold: true
            color: "#3B3B98"
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

            TextField
            {
                id: txtFlatNo

                Layout.preferredWidth: 150
            }

            Label
            {
                text: "Flat Type"
            }

            ComboBox
            {
                id: cmbFlatType

                Layout.preferredWidth: 150

                model:
                [
                    "1BHK",
                    "2BHK",
                    "3BHK"
                ]
            }

            Label
            {
                text: "Status"
            }

            ComboBox
            {
                id: cmbStatus

                Layout.preferredWidth: 150

                model:
                [
                    "Vacant",
                    "Occupied"
                ]
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
                    flatViewModel.addFlat(
                        txtFlatNo.text,
                        cmbFlatType.currentIndex,
                        cmbStatus.currentIndex)
                }
            }
            */
            Button
            {
                text: "Add"

                onClicked:
                {
                    var success =
                            flatViewModel.addFlat(
                                txtFlatNo.text,
                                cmbFlatType.currentIndex,
                                cmbStatus.currentIndex)

                    if (success)
                    {
                        console.log("Flat Added")

                        txtFlatNo.text = ""

                        txtFlatNo.forceActiveFocus()
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
                    flatViewModel.updateFlat(
                        txtFlatNo.text,
                        cmbFlatType.currentIndex,
                        cmbStatus.currentIndex)
                }
            }

            Button
            {
                text: "Delete"

                Layout.preferredWidth: 100

                onClicked:
                {
                    if (txtFlatNo.text === "")
                    {
                        messageDialog.text =
                                "Please select a flat."

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
                    flatViewModel.clearSelection()
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
                            text: "Flat Type"

                            font.bold: true

                            Layout.preferredWidth: 150
                        }

                        Label
                        {
                            text: "Status"

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
                        id: flatListView

                        width: parent.width

                        model: flatViewModel.flatModel

                        spacing: 0

                        delegate: Rectangle
                        {
                            width: flatListView.width
                            height: 36

                            border.width: 1

                            color:
                                index === flatViewModel.selectedRow
                                ? "#A7D8FF"
                                : (index % 2 === 0
                                   ? "#F8F8F8"
                                   : "white")

                            MouseArea
                            {
                                anchors.fill: parent

                                onClicked:
                                {
                                    flatViewModel.selectFlat(index)
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
                                    text: flatType

                                    Layout.preferredWidth: 150
                                }

                                Text
                                {
                                    text: status

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
        target: flatViewModel

        function onOperationCompleted(success, message)
        {
            messageDialog.text = message
            messageDialog.open()
        }

        function onSelectedFlatChanged()
        {
            txtFlatNo.text = flatViewModel.selectedFlatNo

            cmbFlatType.currentIndex =
                    flatViewModel.selectedFlatType

            cmbStatus.currentIndex =
                    flatViewModel.selectedStatus
        }
    }
}