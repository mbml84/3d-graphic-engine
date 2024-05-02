cmake_minimum_required(VERSION 3.14)
project(3DGraphicsEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(SOURCE_FILES src/main.cpp
        src/engine/GameEngine.cpp
        src/engine/GameEngine.h
        src/engine/shapes/Triangle3D.cpp
        src/engine/shapes/Triangle3D.h
        src/engine/shapes/Mesh.cpp
        src/engine/shapes/Mesh.h
        src/engine/shapes/Matrix.cpp
        src/engine/shapes/Matrix.h
)

file(COPY ${CMAKE_SOURCE_DIR}/objects DESTINATION ${CMAKE_BINARY_DIR})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)