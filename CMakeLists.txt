cmake_minimum_required(VERSION 4.11)
project(GLCube)

link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}GLCube/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}GLCube/src/*.h)

add_executable(GLCube, ${HEADER_FILES} ${SOURCE_FILES})