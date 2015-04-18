cmake_minimum_required(VERSION 2.8.4)
project(GeneticAlgorithm)

# CXX flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# CMake directories
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_SRC_DIR                  "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CMAKE_INCLUDE_DIR              "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(CMAKE_TEST_DIR                 "${CMAKE_CURRENT_SOURCE_DIR}/test")

include_directories("${CMAKE_INCLUDE_DIR}")

set(MAIN_TESTS_FILE ${CMAKE_TEST_DIR}/mainTests.cpp)
aux_source_directory(${CMAKE_SRC_DIR} SOURCE_FILES)
aux_source_directory(${CMAKE_TEST_DIR} TESTS_FILES)

add_executable(GeneticAlgorithmTests ${MAIN_TESTS_FILE} ${SOURCE_FILES} ${TESTS_FILES})

# Command used to execute unit tests after build
#add_custom_command(
#    TARGET GeneticAlgorithmTests
#    POST_BUILD
#    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GeneticAlgorithmTests
#)

enable_testing(true)
add_test(
    NAME all
    COMMAND GeneticAlgorithmTests
)