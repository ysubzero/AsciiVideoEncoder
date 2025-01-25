cmake_minimum_required (VERSION 3.28)

project ("AsciiVideoEncoder")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++23")

add_executable (AsciiVideoEncoder "src/filesys.cpp" "src/bmp.cpp" "src/functions.cpp" "src/main.cpp")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)