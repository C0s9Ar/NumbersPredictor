cmake_minimum_required(VERSION 3.20)
project(NumbersPredictor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

find_package(SFML COMPONENTS window graphics system)

file(GLOB_RECURSE SOURCES 
    src/Main.cpp
    src/Application/*.cpp
    src/NumbersClassifier/*.cpp
    src/NumbersClassifier/DataLoader/*.cpp
    src/NumbersClassifier/NeuralNetwork/*.cpp
    src/NumbersClassifier/NeuralNetwork/Matrix/*.cpp
    src/Widget/*.cpp
    src/Widget/Box/*.cpp
    src/Widget/Button/*.cpp
    src/Widget/Canvas/*.cpp
    src/Widget/Field/*.cpp
    src/Widget/Menu/*.cpp
    src/Widget/Slider/*.cpp
    src/Window/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/Application
    ${CMAKE_SOURCE_DIR}/src/NumbersClassifier
    ${CMAKE_SOURCE_DIR}/src/NumbersClassifier/DataLoader
    ${CMAKE_SOURCE_DIR}/src/NumbersClassifier/NeuralNetwork
    ${CMAKE_SOURCE_DIR}/src/NumbersClassifier/NeuralNetwork/Matrix
    ${CMAKE_SOURCE_DIR}/src/Widget
    ${CMAKE_SOURCE_DIR}/src/Widget/Box
    ${CMAKE_SOURCE_DIR}/src/Widget/Button
    ${CMAKE_SOURCE_DIR}/src/Widget/Canvas
    ${CMAKE_SOURCE_DIR}/src/Widget/Field
    ${CMAKE_SOURCE_DIR}/src/Widget/Menu
    ${CMAKE_SOURCE_DIR}/src/Widget/Slider
    ${CMAKE_SOURCE_DIR}/src/Window
)

set(SFML_STATIC_LIBRARIES FALSE)
