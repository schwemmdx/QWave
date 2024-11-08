cmake_minimum_required(VERSION 3.5)

project(QWave VERSION 0.2 LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6_DIR C:/Qt/6.8.0/mingw_64/lib/cmake/Qt6)

find_package(Qt6 REQUIRED COMPONENTS Widgets )
find_package(Qt6 REQUIRED COMPONENTS Charts)
find_package(Qt6 REQUIRED COMPONENTS Gui)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        chartcontainer.h
        chartcontainer.cpp

        customseries.h
        customseries.cpp

        theme_colors.h

        dataimporter.h
        dataimporter.cpp

        csvdatalayout.h
        csvdatalayout.cpp

        detailsdialog.h
        detailsdialog.cpp
        detailsdialog.ui

	    xydata.h
        xydata.cpp

        chartcrosshair.h
        chartcrosshair.cpp

        chartmarker.h
        chartmarker.cpp

        optionsdialog.h
        optionsdialog.cpp
        optionsdialog.ui

        customchart.cpp
        customchart.h

        third-party/rapidcsv.h
        qwave.qrc

	datawidget.h
	datawidget.cpp

	cursordockwidget.h
	cursordockwidget.cpp
	cursordockwidget.ui

)



qt_add_executable(QWave
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)




target_link_libraries(QWave PRIVATE Qt6::Widgets)
target_link_libraries(QWave PRIVATE Qt6::Charts)
target_link_libraries(QWave PRIVATE Qt6::Gui)


set_target_properties(QWave PROPERTIES
    WIN32_EXECUTABLE TRUE
)
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
           #return()  # <--- Change break() to return()
        else()
            message("Found: ${FILE} in ${CMAKE_CURRENT_BINARY_DIR}")
        endif()
    endforeach()
    
    if(NOT QT_LIBS_EXIST)
        message("Error: Not all required libraries found in ${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    if(NOT QT_LIBS_EXIST)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND windeployqt ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Building Dependencies for ${PROJECT_NAME}"
        )
        message("-- Required QT6 libraries not found, rebuild required")
    else()
        message("-- All required QT6 libraries present")
        message("-- Ready for deployment")
    endif()
endif()

install(TARGETS QWave
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QWave)
endif()
