cmake_minimum_required(VERSION 3.10)

project(vfs CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB lib_files src/vfs/**.cpp)
add_library(vfs_lib SHARED ${lib_files})
target_include_directories(vfs_lib PUBLIC include)
target_compile_definitions(vfs_lib PUBLIC _BUILD_VFS=1)

file(GLOB test_files src/test/**.cpp)
add_executable(vfs_test ${test_files})
target_include_directories(vfs_test PUBLIC include)
target_link_libraries(vfs_test vfs_lib)
