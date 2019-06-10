cmake_minimum_required(VERSION 3.10)

set(NAME stracer)

project(${NAME})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include_directories(external)
include_directories(external/glm)
include_directories(external/gli)
include_directories(external/assimp)
include_directories(external/imgui)
include(source/CMakeLists.txt)
include(source/Geometry/CMakeLists.txt)
include(source/Math/CMakeLists.txt)
include(source/Vulkan/CMakeLists.txt)
include(source/json/CMakeLists.txt)

# Add shaders
set(SHADER_DIR shaders)
file(GLOB SHADERS "${SHADER_DIR}/*.vert" "${SHADER_DIR}/*.frag" "${SHADER_DIR}/*.glsl" "${SHADER_DIR}/*.tesc" "${SHADER_DIR}/*.tese")
source_group("Shaders" FILES ${SHADERS})

set(SOURCE_FILES ${STRACER_SOURCES} ${VULKAN_SOURCES} ${GEOMETRY_SOURCES} ${MATH_SOURCES} ${APP_SOURCES} ${JSON_SOURCES})

file(GLOB ADD_SOURCE external/imgui/*.cpp)
SET(SOURCE ${SOURCE_FILES} ${ADD_SOURCE})

file(GLOB IMGUI_SRC "external/imgui/imgui.cpp" "external/imgui/imgui_draw.cpp")

OPTION(USE_D2D_WSI "Build the project using Direct to Display swapchain" OFF)
OPTION(USE_WAYLAND_WSI "Build the project using Wayland swapchain" OFF)

set(RESOURCE_INSTALL_DIR "" CACHE PATH "Path to install resources to (leave empty for running uninstalled)")
find_package(Vulkan)
#find_library(Vulkan_LIBRARY ${CMAKE_SOURCE_DIR}/libs/vulkan/libvulkan.so)

#find_package(Vulkan)
IF (Vulkan_FOUND)
    message(STATUS ${Vulkan_LIBRARY})
ELSE()
    message(FATAL_ERROR "Could not find the vulkan library")
ENDIF()

# Set preprocessor defines
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -D_USE_MATH_DEFINES")

# Clang specific stuff
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-enum")
endif()

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wundef")
endif(CMAKE_COMPILER_IS_GNUCXX)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-std=c++17)

# Compiler specific stuff
IF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
ENDIF(MSVC)

add_executable(stracer ${SHADERS} ${SOURCE_FILES})
add_library(imguisrc STATIC ${IMGUI_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE external/glfw-3.2.1/include Vulkan::Vulkan)
target_link_libraries(stracer imguisrc glfw Vulkan::Vulkan ${ASSIMP_LIBRARIES})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

add_subdirectory(external/glfw-3.2.1)
