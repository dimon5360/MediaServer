cmake_minimum_required(VERSION 3.2)
project(app LANGUAGES CXX VERSION 2.0.0)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-15")
set(CMAKE_C_COMPILER "/usr/bin/clang-15")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(Boost_USE_STATIC_LIBS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread")

find_package(Boost COMPONENTS program_options chrono filesystem thread system date_time context REQUIRED)
find_package(spdlog REQUIRED)
if(Boost_NOT_FOUND)
    message(FATAL "Boost not found!")
endif()

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} ${BOOST_DIR})

set(INCLUDES
    ${workspaceFolder}
    ${workspaceFolder}core/
    ${workspaceFolder}net/
    ${workspaceFolder}net/handler/
    ${workspaceFolder}net/router/
    ${workspaceFolder}config/
)

FILE(GLOB SOURCES
    ${workspaceFolder}*.cpp
    ${workspaceFolder}core/*.cpp
    ${workspaceFolder}net/*.cpp
    ${workspaceFolder}net/handler/*.cpp
    ${workspaceFolder}net/router/*.cpp
    ${workspaceFolder}config/*.cpp
)

include_directories (
    ${INCLUDES}
    ${Boost_INCLUDE_DIR}
)

set(BoostLibs ${Boost_LIBRARIES} ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})

message(STATUS "system: ${CMAKE_SYSTEM}")
message(STATUS "build_type: ${CMAKE_BUILD_TYPE}")
message(STATUS "paths:")
message(STATUS " - ${CMAKE_INSTALL_PREFIX}")
message(STATUS " - ${PROJECT_SOURCE_DIR}")
message(STATUS " - ${PROJECT_BINARY_DIR}")

message(STATUS " - ${Boost_DIR}")
message(STATUS " - ${Boost_INCLUDE_DIR}")
message(STATUS " - ${BoostLibs}")

message(STATUS)
message(STATUS "compiler:")
message(STATUS " - ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " - ${CMAKE_CXX_COMPILER}")
message(STATUS " - ${CMAKE_C_COMPILER}")
message(STATUS " - ${CMAKE_CXX_FLAGS}")
message(STATUS)
message(STATUS "cmake:")
message(STATUS " - ${CMAKE_VERSION}")
message(STATUS " - ${CMAKE_COMMAND}")
message(STATUS " - ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS " - ${CMAKE_GENERATOR}")
message(STATUS " - ${CMAKE_BUILD_TOOL}")
message(STATUS)

add_executable(app main.cpp ${SOURCES} ${INCLUDES})
target_link_libraries(app ${BoostLibs} spdlog::spdlog)