cmake_minimum_required(VERSION 3.3)

find_package(LLVM REQUIRED CONFIG)

#list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g")
# list( APPEND CMAKE_CXX_FLAGS "-std=c++0x -S ${CMAKE_CXX_FLAGS} -g")
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x -S -emit-llvm ${CMAKE_CXX_FLAGS} -g")
# -S to only emit assembly

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_subdirectory(ShuffleVectorPass)