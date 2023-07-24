cmake_minimum_required(VERSION 3.2)
project(app LANGUAGES CXX VERSION 2.0.0)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(Boost_USE_STATIC_LIBS ON)

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -stdlib=libc++ -pthread -fcoroutines-ts")
endif (APPLE)

if (LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -pthread -fcoroutines -spdlog")
endif (LINUX)

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
    ${workspaceFolder}config/
)

FILE(GLOB SOURCES
    ${workspaceFolder}*.cpp
    ${workspaceFolder}core/*.cpp
    ${workspaceFolder}net/*.cpp
    ${workspaceFolder}config/*.cpp
)

include_directories (
    ${INCLUDES}
    ${Boost_INCLUDE_DIR}
)

if (APPLE)
    link_directories(/usr/local/lib /opt/homebrew/Cellar/boost/1.81.0_1/lib)
endif (APPLE)

message(STATUS "system: ${CMAKE_SYSTEM}")
message(STATUS "build_type: ${CMAKE_BUILD_TYPE}")
message(STATUS "paths:")
message(STATUS " - ${CMAKE_INSTALL_PREFIX}")
message(STATUS " - ${PROJECT_SOURCE_DIR}")
message(STATUS " - ${PROJECT_BINARY_DIR}")

message(STATUS " - ${Boost_DIR}")
message(STATUS " - ${Boost_INCLUDE_DIR}")
message(STATUS " - ${Boost_LIBRARIES}")

message(STATUS)
message(STATUS "compiler:")
message(STATUS " - ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " - ${CMAKE_CXX_COMPILER}")
message(STATUS)
message(STATUS "cmake:")
message(STATUS " - ${CMAKE_VERSION}")
message(STATUS " - ${CMAKE_COMMAND}")
message(STATUS " - ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS " - ${CMAKE_GENERATOR}")
message(STATUS " - ${CMAKE_BUILD_TOOL}")
message(STATUS)

set(BoostLibs ${Boost_LIBRARIES} ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(app main.cpp ${SOURCES} ${INCLUDES})
target_link_libraries(app ${BoostLibs} spdlog::spdlog)