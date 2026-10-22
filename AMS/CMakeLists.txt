cmake_minimum_required(VERSION 3.16)

project(AMS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appAMS
    main.cpp
)
qt_add_resources(appAMS "AppResources"
    PREFIX "/"
    FILES
        Resources/Images/PaymentQR.png
)

qt_add_qml_module(appAMS
    URI AMS
    QML_FILES
    SOURCES   Entities/AMSFlat.cpp Entities/AMSFlat.h Entities/AMSMeterReading.cpp Entities/AMSMeterReading.h
    SOURCES Entities/AMSBill.cpp Entities/AMSFlat.cpp Entities/AMSMeterReading.cpp
    SOURCES Managers/AMSBillManager.h Managers/AMSFlatManager.h Managers/AMSMeterReadingManager.h
    SOURCES Managers/AMSBillManager.cpp Managers/AMSFlatManager.cpp Managers/AMSMeterReadingManager.cpp
    SOURCES Database/AMSDatabase.h
    SOURCES Database/AMSDatabase.cpp
    SOURCES Export/AMSDatabase.h
    SOURCES Export/AMSDatabase.cpp
    QML_FILES QML/BillGeneration.qml  QML/Main.qml  QML/Screens/FlatMasterScreen.qml
    SOURCES Entities/AMSBill.h Entities/AMSFlat.h Entities/AMSMeterReading.h
    SOURCES Sql/FlatSql.h
    SOURCES Sql/FlatSql.cpp
    SOURCES Entities/AMSTariff.h
    SOURCES Entities/AMSTariff.cpp
    SOURCES Sql/MeterReadingSql.h
    SOURCES Sql/MeterReadingSql.cpp
    SOURCES Sql/BillSql.h
    SOURCES Sql/BillSql.cpp
    SOURCES Entities/AMSBillGenerationResult.h
    SOURCES Entities/AMSBillGenerationResult.cpp
    SOURCES ViewModels/AMSBillGenerationViewModel.h
    SOURCES ViewModels/AMSBillGenerationViewModel.cpp
    SOURCES ViewModels/AMSFlatViewModel.h
    SOURCES ViewModels/AMSFlatViewModel.cpp
    SOURCES Models/AMSFlatListModel.h Models/AMSFlatListModel.cpp
    QML_FILES QML/Screens/FlatMasterScreen.qml QML/Screens/MeterReading.qml
    SOURCES
    SOURCES
    SOURCES Models/AMSMeterReadingListModel.h
    SOURCES Models/AMSMeterReadingListModel.cpp
    SOURCES ViewModels/AMSMeterReadingViewModel.h
    SOURCES ViewModels/AMSMeterReadingViewModel.cpp
    QML_FILES QML/Screens/FlatMasterScreen.qml QML/Screens/MeterReading.qml
    SOURCES Models/AMSBillListModel.h
    SOURCES Models/AMSBillListModel.cpp
    SOURCES ViewModels/AMSBillViewModel.h
    SOURCES ViewModels/AMSBillViewModel.cpp
    QML_FILES QML/BillListPage.qml
    QML_FILES QML/BillListScreen.qml


)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appAMS PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAMS
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appAMS
    PRIVATE Qt6::Quick
    Qt6::Sql
)

install(TARGETS appAMS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
