cmake_minimum_required(VERSION 3.12...3.31)
project(service_template CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DownloadUserver)

find_package(userver COMPONENTS core postgresql QUIET)
if(NOT userver_FOUND)
  # Tries TRY_DIR first, falls back to downloading userver from GitHub using CPM.
  download_userver(TRY_DIR third_party/userver)
endif()

userver_setup_environment()


# Common sources
add_library(${PROJECT_NAME}_login OBJECT
    src/login.cpp
    src/login.hpp
)
target_link_libraries(${PROJECT_NAME}_login PUBLIC userver::postgresql)

add_library(${PROJECT_NAME}_registration OBJECT
    src/registration.cpp
    src/registration.hpp
)
target_link_libraries(${PROJECT_NAME}_registration PUBLIC userver::postgresql)

add_library(${PROJECT_NAME}_http_client OBJECT
    src/find.hpp
    src/aviasalesAPI.hpp
    src/chatgptAPI.hpp
)
target_link_libraries(${PROJECT_NAME}_http_client PUBLIC userver::core)

add_library(${PROJECT_NAME}_trips OBJECT
    src/save_trip.hpp
)
target_link_libraries(${PROJECT_NAME}_trips PUBLIC userver::postgresql)

add_library(${PROJECT_NAME}_acc OBJECT
    src/account.hpp
)
target_link_libraries(${PROJECT_NAME}_acc PUBLIC userver::postgresql)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE  ${PROJECT_NAME}_login  ${PROJECT_NAME}_registration ${PROJECT_NAME}_http_client ${PROJECT_NAME}_trips ${PROJECT_NAME}_acc)


# Functional Tests
userver_testsuite_add_simple()


# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
