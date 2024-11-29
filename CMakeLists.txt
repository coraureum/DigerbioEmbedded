cmake_minimum_required(VERSION 3.31)
project (DIGERBIO_EMBEDDED LANGUAGES CXX)

set (CMAKE_CXX_FLAGS {"${CMAKE_CXX_FLAGS} -Wall -Werror")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")
file (GLOB source_files"${source_dir}/*.cpp")
