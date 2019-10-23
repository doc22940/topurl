cmake_minimum_required(VERSION 3.15)
project(topurl)

set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src DIR_SRC)
add_executable(${PROJECT_NAME} ${DIR_SRC})

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/test DIR_TEST)
add_executable("${PROJECT_NAME}_test" ${DIR_TEST})