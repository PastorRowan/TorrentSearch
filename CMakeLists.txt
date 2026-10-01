
cmake_minimum_required(VERSION 3.16)

project(TorrentSearch VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    Qml
    QuickControls2
    Network
)

qt_standard_project_setup(REQUIRES 6.5)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HEADERS
    "${INCLUDE_DIR}/Aria2c/Aria2c.h"
    "${INCLUDE_DIR}/download/TorrentDownload.h"
    "${INCLUDE_DIR}/download/TorrentDownloadData.h"
    "${INCLUDE_DIR}/download/TorrentDownloadManager.h"
    "${INCLUDE_DIR}/download/TorrentDownloads.h"
    "${INCLUDE_DIR}/search/EZTVSearch.h"
    "${INCLUDE_DIR}/search/PirateBaySearch.h"
    "${INCLUDE_DIR}/search/TorrentSearch.h"
    "${INCLUDE_DIR}/search/TorrentSearchManager.h"
    "${INCLUDE_DIR}/search/TorrentSearchResult.h"
    "${INCLUDE_DIR}/search/TorrentSearchResults.h"
    "${INCLUDE_DIR}/search/TorrentSearchResultsModel.h"
)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES
    "${SRC_DIR}/Aria2c/Aria2c.cpp"
    "${SRC_DIR}/download/TorrentDownload.cpp"
    "${SRC_DIR}/download/TorrentDownloadData.cpp"
    "${SRC_DIR}/download/TorrentDownloadManager.cpp"
    "${SRC_DIR}/download/TorrentDownloads.cpp"
    "${SRC_DIR}/search/EZTVSearch.cpp"
    "${SRC_DIR}/search/PirateBaySearch.cpp"
    "${SRC_DIR}/search/TorrentSearch.cpp"
    "${SRC_DIR}/search/TorrentSearchManager.cpp"
    "${SRC_DIR}/search/TorrentSearchResult.cpp"
    "${SRC_DIR}/search/TorrentSearchResults.cpp"
    "${SRC_DIR}/search/TorrentSearchResultsModel.cpp"
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
        resources/qml/components/TorrentSearchResultsView.qml
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
        Qt6::Network
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/bin/windows/aria2c.exe"
    "${CMAKE_CURRENT_BINARY_DIR}/windows/aria2c.exe"
    COPYONLY
)

message(STATUS "SOURCE DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CURRENT SOURCE DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "BINARY DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CURRENT BINARY DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "")
