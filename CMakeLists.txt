cmake_minimum_required(VERSION 3.0.0)
project(OimoDemo VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
add_executable(oimo_demo ${SOURCES})
target_link_libraries(oimo_demo 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/liboimo.a
    yaml-cpp
    jsoncpp
)
