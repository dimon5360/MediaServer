cmake_minimum_required(VERSION 3.22)
project(app LANGUAGES CXX VERSION 2.0.0)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++-15")
set(CMAKE_C_COMPILER "/usr/bin/clang-15")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

set(ABSL_PROPAGATE_CXX_STD ON)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

# application build config
set(GRPC_ENABLED false)

# application dependencies
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

set(BOOST_HINTS)

if(USE_STAGED_BOOST)
    set(BOOST_HINTS HINTS ../../../../stage)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/BoostConfig.cmake)

find_package(Boost ${BOOST_VERSION} CONFIG REQUIRED program_options chrono filesystem thread system date_time context)
if(Boost_NOT_FOUND)
    message(FATAL "Boost not found!")
endif()

find_package(spdlog REQUIRED)
if(spdlog_NOT_FOUND)
    message(FATAL "spdlog not found!")
endif()

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

# include(${CMAKE_CURRENT_SOURCE_DIR}/proto/CMakeLists.txt)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} ${BOOST_DIR})

set(INCLUDES
    ${workspaceFolder}source/core/
    ${workspaceFolder}source/net/
    ${workspaceFolder}source/net/http/
    ${workspaceFolder}source/net/http/api/
    ${workspaceFolder}source/net/router/
    ${workspaceFolder}source/net/session/
    ${workspaceFolder}source/config/
    ${workspaceFolder}source/service/

    # ${workspaceFolder}source/net/grpc/

    # generated grpc files
    # ${PROTO_BINARY_DIR}/source/proto/
)

FILE(GLOB SOURCES
    ${workspaceFolder}source/core/*.cc
    ${workspaceFolder}source/net/*.cc
    ${workspaceFolder}source/net/http/*.cc
    ${workspaceFolder}source/net/http/api/*.cc
    ${workspaceFolder}source/net/router/*.cc
    ${workspaceFolder}source/net/session/*.cc
    ${workspaceFolder}source/config/*.cc
    ${workspaceFolder}source/service/*.cc

    # ${workspaceFolder}net/grpc/*.cc
)

include_directories (
    ${INCLUDES}
    ${Boost_INCLUDE_DIR}
)

set(BoostLibs ${Boost_LIBRARIES} ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})

# display application build status

message(STATUS "system: ${CMAKE_SYSTEM}")
message(STATUS "build_type: ${CMAKE_BUILD_TYPE}")
message(STATUS "current binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "paths:")
message(STATUS " - ${CMAKE_INSTALL_PREFIX}")
message(STATUS " - ${PROJECT_SOURCE_DIR}")
message(STATUS " - ${PROJECT_BINARY_DIR}")
message(STATUS)
message(STATUS "boost lib info:")
message(STATUS " - ${Boost_DIR}")
message(STATUS " - ${Boost_INCLUDE_DIR}")
message(STATUS " - ${BoostLibs}")
message(STATUS)
message(STATUS "pqxx lib info:")
message(STATUS " - ${PQXX_LIB}")
message(STATUS " - ${PQ_LIB}")
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

# executable application config

# add_executable(grpc_server test_grpc_server.cc ${INCLUDES})
add_executable(app cmd/main.cc ${SOURCES} ${INCLUDES})

# linker application config

target_link_libraries(app PRIVATE ${BoostLibs} spdlog::spdlog ${PQXX_LIB} ${PQXX_LIBRARIES} )#proto)
# target_link_libraries(grpc_server PRIVATE ${BoostLibs} spdlog::spdlog ${PQXX_LIB} ${PQXX_LIBRARIES} proto)