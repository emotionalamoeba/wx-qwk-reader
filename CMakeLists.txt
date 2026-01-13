cmake_minimum_required(VERSION 3.20)

# Project
project(WXQWKReader)

find_package(wxWidgets REQUIRED COMPONENTS base core)
include(${wxWidgets_USE_FILE})

set(SOURCES README.md src/App.cpp src/QWK.cpp src/MessageTree.cpp src/ConferenceList.cpp)

# Options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libzip package
find_package(libzip REQUIRED)

# Target
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCES})
target_include_directories(${PROJECT_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} libzip::zip)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "wxCore/HelloWorlds")