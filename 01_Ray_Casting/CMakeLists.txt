cmake_minimum_required(VERSION 3.15)
project(01_Ray_Casting)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(./src SRCS)

set(SOURCE_FILES
        ${SRCS}
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

add_executable(raytracer ${SOURCE_FILES})
