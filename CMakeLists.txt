cmake_minimum_required(VERSION 3.6.2)

project (SSEGuide)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")

set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT  "${SSEGuide}"  )
set (CMAKE_CXX_STANDARD 14)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
 
add_executable(SSEGuide ${SOURCES})
add_definitions(/arch:AVX)

