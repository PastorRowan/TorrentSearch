
cmake_minimum_required(VERSION 3.16)

project(TorrentSearch VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    Qml
    QuickControls2
)

qt_standard_project_setup(REQUIRES 6.5)

set(HEADERS

)

set(SOURCES
    src/main.cpp
)

qt_add_executable(TorrentSearch
    ${HEADERS}
    ${SOURCES}
)

qt_add_qml_module(TorrentSearch

    URI TorrentSearch
    VERSION 1.0

    QML_FILES
        resources/qml/components/TorrentSearchResult.qml
        resources/qml/pages/SearchPage.qml
        resources/qml/Main.qml

    # RESOURCES
        # img/world.png

)

target_link_libraries(TorrentSearch
    PRIVATE
        Qt6::Quick
        Qt6::Qml
        Qt6::QuickControls2
)

message(STATUS "SOURCE DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CURRENT SOURCE DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "BINARY DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CURRENT BINARY DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "")
