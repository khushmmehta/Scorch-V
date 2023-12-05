cmake_minimum_required(VERSION 3.27)
project(ScorchV)

set(CMAKE_CXX_STANDARD 23)
set(ignoreMe "${FOO}${BAZ}${BAR}")
set(SOURCE_FILES src/main.cpp
        src/ScorchVkEngine/ScorchV.cpp
        src/ScorchVkEngine/ScorchV.cpp
        src/ScorchVkEngine/ScorchV.h)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4099")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4099")

include_directories(GLFW)
include_directories(GLFW/include)
include_directories(src/vendor)
link_directories(GLFW/lib)

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw3 User32 Gdi32 Shell32 ucrtd)
    target_link_options(${PROJECT_NAME} PRIVATE /NODEFAULTLIB:UCRTD)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw3 User32 Gdi32 Shell32 ucrt)
    target_link_options(${PROJECT_NAME} PRIVATE /NODEFAULTLIB:UCRT)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw3 User32 Gdi32 Shell32 ucrt)
    target_link_options(${PROJECT_NAME} PRIVATE /NODEFAULTLIB:UCRT)
endif ()
