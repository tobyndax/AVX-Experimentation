cmake_minimum_required(VERSION 3.6.2)

project (SSEGuide)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
 
add_executable(SSEGuide ${SOURCES})


