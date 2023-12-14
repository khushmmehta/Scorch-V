cmake_minimum_required(VERSION 3.27)
project(ScorchV)

set(CMAKE_CXX_STANDARD 23)
set(ignoreMe "${FOO}${BAZ}${BAR}")
set(SOURCE_FILES src/main.cpp
        src/ScorchVkEngine/ScorchV.cpp
        src/ScorchVkEngine/ScorchV.h

        src/ScorchVkEngine/Abstractions/ValidationLayers.cpp
        src/ScorchVkEngine/Abstractions/ValidationLayers.h

        src/ScorchVkEngine/Abstractions/VulkanMemoryAllocator.cpp
        src/ScorchVkEngine/Abstractions/VulkanMemoryAllocator.h

        src/ScorchVkEngine/Abstractions/PresentationManager.cpp
        src/ScorchVkEngine/Abstractions/PresentationManager.h

        src/ScorchVkEngine/Abstractions/Rendering/Shader.cpp
        src/ScorchVkEngine/Abstractions/Rendering/Shader.h

        src/ScorchVkEngine/Abstractions/Rendering/BufferManager.cpp
        src/ScorchVkEngine/Abstractions/Rendering/BufferManager.h

        src/ScorchVkEngine/Abstractions/Rendering/Objects/MeshObject.cpp
        src/ScorchVkEngine/Abstractions/Rendering/Objects/MeshObject.h

        src/vendor/imgui/imgui.cpp
        src/vendor/imgui/imgui.h

        src/vendor/imgui/imgui_demo.cpp
        src/vendor/imgui/imgui_draw.cpp
        src/vendor/imgui/imgui_tables.cpp
        src/vendor/imgui/imgui_widgets.cpp

        src/vendor/imgui/backends/imgui_impl_glfw.cpp
        src/vendor/imgui/backends/imgui_impl_glfw.h
        src/vendor/imgui/backends/imgui_impl_vulkan.cpp
        src/vendor/imgui/backends/imgui_impl_vulkan.h
        src/ScorchVkEngine/Abstractions/GuiManager.cpp
        src/ScorchVkEngine/Abstractions/GuiManager.h)

include_directories(GLFW)
include_directories(GLFW/include)
include_directories(src/ScorchVkEngine)
include_directories(src/vendor)
include_directories(src/vendor/imgui)
include_directories(src/vendor/imgui/backends)
link_directories(GLFW/lib)

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw3)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_compile_options(${PROJECT_NAME} PRIVATE /permissive-)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif ()
