
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

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HEADERS
    "${INCLUDE_DIR}/networking/NetworkManager.h"
    "${INCLUDE_DIR}/providers/EZTVProvider.h"
    "${INCLUDE_DIR}/providers/PirateBayProvider.h"
    "${INCLUDE_DIR}/providers/TorrentProvider.h"
    "${INCLUDE_DIR}/providers/TorrentProviderManager.h"
    "${INCLUDE_DIR}/search/TorrentSearchResult.h"
    "${INCLUDE_DIR}/search/TorrentSearchResults.h"
)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES
    "${SRC_DIR}/networking/NetworkManager.cpp"
    "${SRC_DIR}/providers/EZTVProvider.cpp"
    "${SRC_DIR}/providers/PirateBayProvider.cpp"
    "${INCLUDE_DIR}/providers/TorrentProvider.cpp"
    "${SRC_DIR}/providers/TorrentProviderManager.cpp"
    "${SRC_DIR}/search/TorrentSearchResult.cpp"
    "${SRC_DIR}/search/TorrentSearchResults.cpp"
    "${SRC_DIR}/main.cpp"
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

target_include_directories(TorrentSearch
    PRIVATE
        "${INCLUDE_DIR}"
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
