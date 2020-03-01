cmake_minimum_required(VERSION 3.15)
project(05_Voxel_Redndering)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRCS)
set(SOURCE_FILES
        ${SRCS}
        src/marchingInfo.h src/marchingInfo.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)
add_executable(raytracer ${SOURCE_FILES})

