cmake_minimum_required(VERSION 3.1)

SET(PROJECT_NAME_STR AstarPaganitzu)
PROJECT (${PROJECT_NAME_STR})

INCLUDE_DIRECTORIES( "${CMAKE_SOURCE_DIR}/include" )

FILE(GLOB HEADERS "include/*.hpp")
source_group(include FILES ${HEADERS}})

SET(CMAKE_SUPPRESS_REGENERATION TRUE)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

SET(CMAKE_CXX_FLAGS  " ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE} --std=c++17 -Werror -Wall -O3")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")


ADD_SUBDIRECTORY(src)


