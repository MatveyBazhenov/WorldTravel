cmake_minimum_required(VERSION 3.10)
project(WorldTravel)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "*.cpp")

add_executable(MyExecutable ${SOURCES})

find_package(wxWidgets REQUIRED COMPONENTS core base net)
include(${wxWidgets_USE_FILE})

find_package(CURL REQUIRED)

find_package(nlohmann_json 3.0.0 REQUIRED)

find_package(wxWidgets REQUIRED COMPONENTS core base net html adv)

target_link_libraries(MyExecutable 
    PRIVATE 
    ${wxWidgets_LIBRARIES} 
    CURL::libcurl
)

target_compile_definitions(MyExecutable PRIVATE wxUSE_SOCKETS=1)

target_link_libraries(MyExecutable PRIVATE nlohmann_json::nlohmann_json)
