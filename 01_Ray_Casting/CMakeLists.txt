cmake_minimum_required(VERSION 3.15)
project(01_Ray_Casting)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(./include INCLUDE_SRCS)

set(SOURCE_FILES
        src/main.cpp
        src/ifs.h
        ${INCLUDE_SRCS}
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

add_executable(ifs ${SOURCE_FILES})


add_executable(01_Ray_Casting main.cpp)