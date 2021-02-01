cmake_minimum_required(VERSION 3.12)

project(XENONLED VERSION 0.1)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCE ${CMAKE_SOURCE_DIR}/src/*.cc)

file(GLOB HEADER
	${CMAKE_SOURCE_DIR}/include/xenonled/*.hh
	${CMAKE_SOURCE_DIR}/include/glm/*.hpp
	${CMAKE_SOURCE_DIR}/include/GL/*.h
)

add_executable(xenonled ${SOURCE} ${HEADER})

find_package(OpenGL REQUIRED)

target_include_directories(xenonled PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_directories(xenonled PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(xenonled glfw3 opengl32 glew32s)