cmake_minimum_required(VERSION 2.8...3.13)
project(boids)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(TGUI 0.10 REQUIRED)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "Could not find SFML")
endif()

if(NOT TGUI_FOUND)
    message(FATAL_ERROR "Could not find TGUI")
endif()
set(CMAKE_VERBOSE_MAKEFILE ON)
file(GLOB SRCS *.cpp )

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} tgui sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/arrow.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})