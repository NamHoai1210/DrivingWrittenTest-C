cmake_minimum_required(VERSION 3.5)

project(DrivingWrittenTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES DrivingWrittenTest_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Auth/auth.cpp
        Auth/auth.h
        Auth/auth.ui
        Practice/practice.cpp
        Practice/practice.h
        Practice/practice.ui
        Exam/exam.cpp
        Exam/exam.h
        Exam/exam.ui
        Question/questionframe.ui
        Question/questionframe.h
        Question/questionframe.cpp
        Entity/Question/question.h
        Entity/Question/question.cpp
        Entity/History/history.h
        Entity/History/history.cpp
        Entity/Room/roomentity.cpp
        Entity/Room/roomentity.h
        Entity/User/user.cpp
        Entity/User/user.h
        Utils/utils.cpp
        Utils/utils.h
        Online/onlinewindow.cpp
        Online/onlinewindow.h
        Online/onlinewindow.ui
        Room/room.cpp
        Room/room.h
        Room/room.ui
        connect/connect.cpp
        connect/connect.h
        session/session.cpp
        session/session.h
        CreateRoomForm/createroom.cpp
        CreateRoomForm/createroom.h
        CreateRoomForm/createroom.ui
        WaitingScreen/waiting.cpp
        WaitingScreen/waiting.h
        WaitingScreen/waiting.ui
        RoomResult/roomresult.cpp
        RoomResult/roomresult.h
        RoomResult/roomresult.ui
        resource.qrc
        ${TS_FILES}
)

include_directories(rapidjson)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DrivingWrittenTest
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DrivingWrittenTest APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(DrivingWrittenTest SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(DrivingWrittenTest
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(DrivingWrittenTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(DrivingWrittenTest PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS DrivingWrittenTest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DrivingWrittenTest)
endif()
