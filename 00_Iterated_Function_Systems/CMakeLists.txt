cmake_minimum_required(VERSION 3.15)
project(00_Iterated_Function_Systems)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(./src SRCS)

set(SOURCE_FILES
        ${SRCS}
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

add_executable(ifs ${SOURCE_FILES})