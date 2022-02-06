cmake_minimum_required(VERSION 3.16)
project(lunar-client-qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(QT_COMPONENTS Core Gui Widgets Svg)


find_package(Qt6 COMPONENTS
        ${QT_COMPONENTS}
        )
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS
            ${QT_COMPONENTS}
            REQUIRED
            )
    set(QT_VER Qt5)
else()
set(QT_VER Qt6)
endif()

set(SOURCE_FILES
        src/main.cpp
        src/gui/mainwindow.cpp
        src/gui/mainwindow.h
        src/launch/offlinelauncher.cpp
        src/launch/offlinelauncher.h
        src/gui/pages/generalpage.cpp
        src/gui/pages/generalpage.h
        src/gui/pages/configurationpage.h
        src/gui/pages/configurationpage.cpp
        src/gui/pages/agentspage.cpp
        src/gui/pages/agentspage.h
        src/gui/pages/helperspage.cpp
        src/gui/pages/helperspage.h
        src/launch/launcher.h
        src/launch/launcher.cpp
        src/config/config.cpp
        src/config/config.h
        src/gui/pages/minecraftpage.cpp
        src/gui/pages/minecraftpage.h
        src/gui/widgets/filechooser.cpp
        src/gui/widgets/filechooser.h 
        src/gui/widgets/widgetutils.cpp 
        src/gui/widgets/widgetutils.h src/utils.cpp src/utils.h)
        
set(LCQT_VERSION UNKNOWN)
find_package(Git)
if(Git_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tag
            OUTPUT_VARIABLE tag
    )

    if(tag MATCHES "^v([0-9]+)\\.([0-9]+)\\.([0-9]+)")
        set(LCQT_VERSION ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3})
    endif()
endif()

configure_file(${CMAKE_SOURCE_DIR}/version.cpp.in ${CMAKE_BINARY_DIR}/version.cpp @ONLY)
set(VERSION_FILE ${CMAKE_BINARY_DIR}/version.cpp)

add_executable(lunar-client-qt WIN32 MACOSX_BUNDLE ${SOURCE_FILES} ${VERSION_FILE} res.qrc)

if(UNIX AND APPLE)
    set(JAR_DEST_DIR ${CMAKE_BINARY_DIR}/lunar-client-qt.app/Contents/MacOS)
else()
    set(JAR_DEST_DIR ${CMAKE_BINARY_DIR})
endif()

function(COPY_JARS DIR OUTPUT_LIST)
    add_custom_command(
            OUTPUT "${JAR_DEST_DIR}/${DIR}/"
            COMMAND ${CMAKE_COMMAND} -E make_directory ${JAR_DEST_DIR}/${DIR}/
    )

    set(COPIED_FILES "${JAR_DEST_DIR}/${DIR}/")

    file(GLOB JARS ${CMAKE_SOURCE_DIR}/java/${DIR}/*jar)

    foreach(JAR ${JARS})
        get_filename_component(FILE_NAME ${JAR} NAME)
        add_custom_command(
                OUTPUT "${JAR_DEST_DIR}/${DIR}/${FILE_NAME}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${JAR} ${JAR_DEST_DIR}/${DIR}/
                DEPENDS ${JAR}
                COMMENT "Copying ${FILE_NAME}"
        )
        list(APPEND COPIED_FILES ${JAR_DEST_DIR}/${DIR}/${FILE_NAME})
    endforeach()

    set(${OUTPUT_LIST} ${COPIED_FILES} PARENT_SCOPE)
endfunction()

copy_jars(agents COPIED_AGENTS)
copy_jars(libs COPIED_LIBRARIES)

add_custom_target(CopyJars ALL COMMAND DEPENDS ${COPIED_AGENTS} ${COPIED_LIBRARIES})
add_dependencies(lunar-client-qt CopyJars)

list(TRANSFORM QT_COMPONENTS PREPEND ${QT_VER}:: OUTPUT_VARIABLE QT_LIBS)

target_link_libraries(lunar-client-qt ${QT_LIBS})

target_include_directories(lunar-client-qt PRIVATE src)