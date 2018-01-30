cmake_minimum_required(VERSION 2.8.4)
set(TARGET_NAME HW3)
SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
project(${TARGET_NAME})

# Set build flags and directories
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/deploy)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib)

# Find source files
file(GLOB SOURCE_FILES ${SOURCE_DIR}/*.cpp)

# Find libraries
find_library(COCOA Cocoa)
find_library(IOKIT IOKit)
find_library(CORE_VIDEO CoreVideo)
find_library(OPENGL OpenGL)
find_library(GLFW glfw3 PATHS ${LIBRARY_DIR})
find_library(APPKIT AppKit)
find_library(ANT AntTweakBar PATHS ${LIBRARY_DIR})
set(ALL_LIBS ${COCOA} ${IOKIT} ${CORE_VIDEO} ${OPENGL} ${GLFW} ${APPKIT} ${ANT})

# Create build rules
include_directories(${SOURCE_DIR} ${LIBRARY_DIR})
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} ${ALL_LIBS})

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION deploy)