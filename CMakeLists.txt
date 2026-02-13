# Ubuntu 25.10
cmake_minimum_required(VERSION 3.31)

project(my_great_project VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IWYU
find_program(IWYU_PATH NAMES include-what-you-use iwyu)
if(NOT IWYU_PATH)
  message(FATAL_ERROR
    "Could not find the program include-what-you-use (apt install iwyu)")
endif()
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})

add_compile_options(-Wall -Werror -Wpedantic -Wextra)

# Server
add_executable(server server.cpp)
target_link_libraries(server PRIVATE shared)

# Client
add_executable(client client.cpp)
target_link_libraries(client PRIVATE shared)

# Shared Lib
add_library(shared SHARED)
target_sources(
  shared

  PUBLIC
  FILE_SET CXX_MODULES
  FILES
    shared.cpp
)
