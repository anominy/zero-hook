cmake_minimum_required(VERSION 4.2)
project(zero-hook CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

remove_definitions(-D_UNICODE -DUNICODE -D_MBCS)

add_compile_options(-m32)
add_link_options(-m32)

file(GLOB SOURCE_FILES src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN WIN32_EXTRA_LEAN)

add_subdirectory(ext/spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
