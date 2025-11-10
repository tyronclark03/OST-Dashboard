cmake_minimum_required(VERSION 3.16)
project(OST_Dashboard VERSION 0.5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# put binaries in /bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

# 👇 explicitly list all .cpp files we have right now
add_executable(ost_dashboard
    src/scanner/scanner.cpp
    src/scanner/scanner_test.cpp
    src/logger/logger.cpp
    src/config/config.cpp
)
# warnings like before
if (MSVC)
    target_compile_options(ost_dashboard PRIVATE /W4 /WX)
else()
    target_compile_options(ost_dashboard PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
