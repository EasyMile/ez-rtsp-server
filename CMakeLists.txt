cmake_minimum_required(VERSION 3.2.2)
project(SimpleRtspServer)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wextra -Werror)

# Library

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_GST_RTSP_SERVER REQUIRED gstreamer-rtsp-server-1.0)

add_library(_SimpleRtspServer STATIC lib/Server.cpp)

target_include_directories(_SimpleRtspServer SYSTEM PRIVATE
  ${PC_GST_RTSP_SERVER_INCLUDE_DIRS}
)
target_include_directories(_SimpleRtspServer PUBLIC lib)

target_link_libraries(_SimpleRtspServer PRIVATE
  ${PC_GST_RTSP_SERVER_LIBRARIES}
  Threads::Threads
)

# CLI Executable

find_package(Boost REQUIRED COMPONENTS program_options)
add_executable(SimpleRtspServer main.cpp sources.cpp)
target_link_libraries(SimpleRtspServer PRIVATE
  _SimpleRtspServer
  Boost::program_options)

# Tests

find_path(CATCH_INCLUDE_DIR catch.hpp PATH_SUFFIXES catch)
add_executable(smoketest test/test.cpp)
target_link_libraries(smoketest PRIVATE _SimpleRtspServer)
target_include_directories(smoketest PRIVATE ${CATCH_INCLUDE_DIR})

include(CTest)
add_test(NAME smoke COMMAND smoketest)
