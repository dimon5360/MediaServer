cmake_minimum_required(VERSION 3.8)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(ABSL_ENABLE_INSTALL ON)

FetchContent_Declare(
  absl
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp
  GIT_TAG        20230802.1
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(absl)

FetchContent_Declare(
  Protobuf
  GIT_REPOSITORY https://github.com/protocolbuffers/protobuf
  GIT_TAG        v25.1
  SOURCE_SUBDIR  cmake
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(Protobuf)

FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.60.0
)
FetchContent_MakeAvailable(gRPC)

find_package(absl REQUIRED)
FetchContent_GetProperties(Protobuf SOURCE_DIR Protobuf_SOURCE_DIR)
include(${Protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)

add_library(proto OBJECT "${CMAKE_CURRENT_LIST_DIR}/storage.proto")
target_link_libraries(proto PUBLIC protobuf::libprotobuf grpc++ grpc
  absl::algorithm
  absl::base
  absl::debugging
  absl::flat_hash_map
  absl::flags
  absl::flags_parse
  absl::memory
  absl::meta
  absl::numeric
  absl::random_random
  absl::strings
  absl::synchronization
  absl::time
  absl::utility
  grpc++_reflection
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

message(STATUS " - ${_REFLECTION}")
message(STATUS " - ${_GRPC_GRPCPP}")
message(STATUS " - ${_PROTOBUF_LIBPROTOBUF}")

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

target_include_directories(proto PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")

protobuf_generate(
    TARGET proto
    IMPORT_DIRS ${PROTO_IMPORT_DIRS}
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")

protobuf_generate(
    TARGET proto
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=$<TARGET_FILE:grpc_cpp_plugin>"
    IMPORT_DIRS ${PROTO_IMPORT_DIRS}
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")