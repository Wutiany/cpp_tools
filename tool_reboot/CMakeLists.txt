cmake_minimum_required(VERSION 3.5)

project(reboot)

add_executable(reboot_win
        src/main.cpp
        src/reboot_win.cpp)

set(SOURCE
        src/main.cpp
        src/reboot_win.cpp)

target_include_directories(reboot_win
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include)

target_compile_features(reboot_win
        PRIVATE cxx_std_11)
