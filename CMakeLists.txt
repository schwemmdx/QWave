cmake_minimum_required(VERSION 3.5)

project(QWave VERSION 0.2 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



if(WIN32)
    set(Qt6_DIR C:/Qt/6.8.0/mingw_64/lib/cmake/Qt6)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Gui)

# Define the directories where sources, headers, and UI files are located
set(CHART_SOURCE_DIR customwidgets/chart)
set(DATA_IMPORT_DIR customwidgets/dataimport)
set(DIALOG_DIR customwidgets/dialog)
set(MISC_DIR customwidgets/misc)
set(STACKWIDGET_DIR customwidgets/stackwidgets)
set(UI_DIR customwidgets/ui-files)

# Set AUTOUIC search paths to include the UI_DIR so that uic can find MainWindow.ui
set(CMAKE_AUTOUIC_SEARCH_PATHS ${UI_DIR})

include_directories(
    ${CHART_SOURCE_DIR}
    ${DATA_IMPORT_DIR}
    ${DIALOG_DIR}
    ${MISC_DIR}
    ${STACKWIDGET_DIR}
    ${UI_DIR}
    .
    themes
    third-party
)

# Specify UI sources
set(UI_SOURCES
    ${UI_DIR}/MainWindow.ui
    ${UI_DIR}/DetailsDialog.ui
    ${UI_DIR}/OptionsDialog.ui
)

# Specify project sources
set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    # Chart-related classes
    ${CHART_SOURCE_DIR}/ChartContainer.cpp
    ${CHART_SOURCE_DIR}/CustomSeries.cpp
    ${CHART_SOURCE_DIR}/ChartCrosshair.cpp
    ${CHART_SOURCE_DIR}/ChartMarker.cpp
    ${CHART_SOURCE_DIR}/CustomChart.cpp
    ${CHART_SOURCE_DIR}/XCursor.cpp
    # Import-related classes
    ${DATA_IMPORT_DIR}/HiracData.cpp
    ${DATA_IMPORT_DIR}/CSVData.cpp

    # Dialogs
    ${DIALOG_DIR}/DetailsDialog.cpp
    ${DIALOG_DIR}/OptionsDialog.cpp
    # Misc classes
    ${MISC_DIR}/Message.cpp
    ${MISC_DIR}/MessageQueue.cpp  
    ${MISC_DIR}/ScientificFormatter.cpp 
    ${MISC_DIR}/ThemeManager.cpp
    
    # StackWidget classes
   ${STACKWIDGET_DIR}/SettingsTree.cpp
    # UI files
    ${UI_SOURCES}
    resources.qrc
)

# Add the executable and link the necessary Qt libraries
qt_add_executable(QWave
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    resources.qrc
    
)

# Link Qt libraries
target_link_libraries(QWave PRIVATE Qt6::Widgets Qt6::Charts Qt6::Gui)

# Set executable properties for Windows
set_target_properties(QWave PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Handle Windows-specific Qt DLLs
if(WIN32)
    set(LIBS_REQUIRED 
        Qt6Charts.dll
        Qt6Core.dll
        Qt6OpenGL.dll
        Qt6Gui.dll
        Qt6Svg.dll
        Qt6Widgets.dll
        Qt6OpenGlWidgets.dll
        opengl32sw.dll
        D3DCompiler_47.dll
        libgcc_s_seh-1.dll
        libstdc++-6.dll
        libwinpthread-1.dll
        Qt6Network.dll
    )

    set(QT_LIBS_EXIST TRUE)

    foreach(FILE ${LIBS_REQUIRED})
        if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${FILE})
            message("Error: ${FILE} not found in ${CMAKE_CURRENT_BINARY_DIR}")
            set(QT_LIBS_EXIST FALSE)
        else()
            message("Found: ${FILE} in ${CMAKE_CURRENT_BINARY_DIR}")
        endif()
    endforeach()
    
    if(NOT QT_LIBS_EXIST)
        add_custom_command(
            TARGET QWave POST_BUILD
            COMMAND windeployqt ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Building Dependencies for QWave"
        )
        message("-- Required Qt6 libraries not found, rebuild required")
    else()
        message("-- All required Qt6 libraries present")
        message("-- Ready for deployment")
    endif()
endif()

# Installation details
install(TARGETS QWave
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Finalize executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QWave)
endif()
