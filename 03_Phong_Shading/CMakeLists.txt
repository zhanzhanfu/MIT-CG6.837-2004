cmake_minimum_required(VERSION 3.15)
project(03_Phong_Shading)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRCS)
set(SOURCE_FILES
        ${SRCS}
        )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)
add_executable(raytracer ${SOURCE_FILES})
target_link_libraries(raytracer ${PROJECT_SOURCE_DIR}/lib/libglfw3.a)
