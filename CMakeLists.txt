cmake_minimum_required(VERSION 3.20)

project(QsLog)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_DEBUG_POSTFIX "_d")

set(QT_MSVC_DIR "D:/SoftWare/Library/QT/6.8.2/msvc2022_64")
set(CMAKE_PREFIX_PATH ${QT_MSVC_DIR}/lib/cmake)
find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
)

file(GLOB SRC_FILES *.cpp)
file(GLOB INCLUDE_FILES *.h)

set(PROJECT_SOURCES
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt6_add_library(${PROJECT_NAME} STATIC
        ${PROJECT_SOURCES}
    )
elseif(QT_VERSION_MAJOR EQUAL 5)
    qt5_add_library(${PROJECT_NAME} STATIC
        ${PROJECT_SOURCES}
    )
else()
    message(FATAL_ERROR "Unsupported Qt version")
endif()

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PROJECT_LIBRARIES 
    Qt${QT_VERSION_MAJOR}::Core
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    ${PROJECT_LIBRARIES}
)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(IMPORTED_RUNTIME_ARTIFACTS ${PROJECT_LIBRARIES})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ DESTINATION cmake)
install(FILES ${INCLUDE_FILES}
    DESTINATION include/QsLog
)
install(CODE [[
    if(EXISTS README.md)
        install(FILES README.md DESTINATION .)
    endif()
    if(EXISTS LICENSE.txt)
        install(FILES LICENSE.txt DESTINATION .)
    endif()
    if(EXISTS LICENSE)
        install(FILES LICENSE.txt DESTINATION .)
    endif()
]])



