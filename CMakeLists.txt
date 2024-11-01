CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

# Name of the project
PROJECT(TBDEngine)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Get the list of all files.
FILE(GLOB_RECURSE SOURCES "src/*.cpp")
FILE(GLOB_RECURSE HEADERS "include/*.h")

message(STATUS "${SOURCES}")
message(STATUS "${HEADERS}")

# Set the executable.
message(STATUS "Including ${CMAKE_SOURCE_DIR}/include")
include_directories(${CMAKE_SOURCE_DIR}/include)
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLSL})

# Use FetchContent to download and build GLM, GLFW, and Vulkan SDK

include(FetchContent)

# Fetch GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8  # Stable release
)
FetchContent_MakeAvailable(glm)
set(GLM_INCLUDE_DIRS ${glm_SOURCE_DIR})
message(STATUS "GLM include directory: ${GLM_INCLUDE_DIRS}")
include_directories(${GLM_INCLUDE_DIRS})

# Fetch GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8  # Stable release
)
FetchContent_MakeAvailable(glfw)
set(GLFW_INCLUDE_DIRS ${glfw_SOURCE_DIR}/include)
message(STATUS "GLFW include directory: ${GLFW_INCLUDE_DIRS}")
include_directories(${GLFW_INCLUDE_DIRS})

# Fetch GLEW
FetchContent_Declare(
        glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG glew-cmake-2.2.0  # Specify the version you want to use (e.g., "glew-2.2.0")
)

FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
    FetchContent_Populate(glew)

    add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
FetchContent_MakeAvailable(glew)
set(GLEW_INCLUDE_DIRS ${glew_SOURCE_DIR}/include)
message(STATUS "GLEW include directory: ${GLEW_INCLUDE_DIRS}")
target_link_libraries(${CMAKE_PROJECT_NAME} libglew_static)
#include_directories(${GLEW_INCLUDE_DIRS})

# Fetch fastgltf
FetchContent_Declare(
        fastgltf
        GIT_REPOSITORY https://github.com/spnda/fastgltf.git
        GIT_TAG v0.7.2  # Stable release
)
FetchContent_MakeAvailable(fastgltf)
# set(FASTGLTF_INCLUDE_DIRS ${fastgltf_SOURCE_DIR}/include)
# message(STATUS "fastgltf include directory: ${FASTGLTF_INCLUDE_DIRS}")
# include_directories(${FASTGLTF_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} fastgltf::fastgltf)

# Fetch fmt
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.0.2  # Stable release
)
FetchContent_MakeAvailable(fmt)
# set(FMT_INCLUDE_DIRS ${fmt_SOURCE_DIR}/include)
# message(STATUS "fmt include directory: ${FMT_INCLUDE_DIRS}")
# include_directories(${FMT_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} fmt::fmt)

# Fetch imgui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.3  # Stable release
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_INCLUDE_DIRS ${imgui_SOURCE_DIR})
message(STATUS "imgui include directory: ${IMGUI_INCLUDE_DIRS}")

# Add ImGui sources manually
set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)


# Create the imgui library target
add_library(imgui STATIC ${IMGUI_SOURCES})


# Fetch stb
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
)
FetchContent_MakeAvailable(stb)
set(STB_INCLUDE_DIRS ${stb_SOURCE_DIR})
message(STATUS "stb include directory: ${STB_INCLUDE_DIRS}")
include_directories(${STB_INCLUDE_DIRS})

# # Fetch vk_bootstrap
# FetchContent_Declare(
#         vk_bootstrap
#         GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
#         GIT_TAG v1.3.296
# )
# FetchContent_MakeAvailable(vk_bootstrap)
# target_link_libraries(${CMAKE_PROJECT_NAME} vk-bootstrap::vk-bootstrap)
#
# # Fetch VulkanMemoryAllocator
# FetchContent_Declare(
#         vma
#         GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
#         GIT_TAG v3.1.0  # Stable release
# )
# FetchContent_MakeAvailable(vma)
# set(VMA_INCLUDE_DIRS ${vma_SOURCE_DIR}/include)
# message(STATUS "vma include directory: ${VMA_INCLUDE_DIRS}")
# include_directories(${VMA_INCLUDE_DIRS})
#
# # Fetch volk
# FetchContent_Declare(
#         volk
#         GIT_REPOSITORY https://github.com/zeux/volk.git
#         GIT_TAG vulkan-sdk-1.3.296.0  # Stable release
# )
# FetchContent_MakeAvailable(volk)
# set(VOLK_INCLUDE_DIRS ${volk_SOURCE_DIR})
# message(STATUS "volk include directory: ${VOLK_INCLUDE_DIRS}")
# include_directories(${VOLK_INCLUDE_DIRS})

# Use the Vulkan package provided by the SDK
# find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)

# If Vulkan is found, link it to your executable
# if (Vulkan_FOUND)
#     target_link_libraries(${CMAKE_PROJECT_NAME} Vulkan::Vulkan)
# endif()

# Include directories for ImGui
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

target_link_libraries(imgui PRIVATE glfw opengl32)

# Link imgui to your main project target
target_link_libraries(${CMAKE_PROJECT_NAME} imgui )


# Link libraries for GLFW and Vulkan
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)

# Use C++17
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# OS specific options and libraries
IF(WIN32)
    # -Wall produces way too many warnings.
    # -pedantic is not supported.
    # Disable warning 4996.
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
    message(STATUS "Vulkan_LIBRARY is ${Vulkan_LIBRARY}")
    target_link_libraries(${CMAKE_PROJECT_NAME} ${Vulkan_LIBRARY})
    SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
ELSE()
    # Enable all pedantic warnings.
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
    IF(APPLE)
        # macOS specific frameworks for GLFW and Vulkan
        find_library(MOLTENVK_FRAMEWORK NAMES MoltenVK
                PATHS ${MOLTENVK_PATH}/include
                NO_DEFAULT_PATH)

        # target_link_libraries(${CMAKE_PROJECT_NAME} "-framework MoltenVK")
    ELSE()
        # Link the Linux Vulkan library
        target_link_libraries(${CMAKE_PROJECT_NAME} vulkan)
    ENDIF()
ENDIF()

# find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
#
# if(NOT GLSL_VALIDATOR)
#     message(FATAL_ERROR "glslangValidator not found")
# endif()
#
# FILE(GLOB_RECURSE GLSL_SOURCE_FILES
#         "shaders/*.frag"
#         "shaders/*.vert"
#         "shaders/*.comp"
# )
#
# foreach(GLSL ${GLSL_SOURCE_FILES})
#     message(STATUS "BUILDING SHADER")
#     get_filename_component(FILE_NAME ${GLSL} NAME)
#     set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
#     add_custom_command(
#             TARGET
#             ${CMAKE_PROJECT_NAME}
#             POST_BUILD
#             COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
#             DEPENDS ${GLSL})
#     MESSAGE(STATUS "Compiling ${SPIRV}")
#     list(APPEND SPIRV_BINARY_FILES ${SPIRV})
# endforeach(GLSL)
#
# add_custom_target(
#         Shaders
#         ALL DEPENDS ${SPIRV_BINARY_FILES}
# )

# Additional settings for Vulkan-based projects
# Specify Vulkan SDK paths for runtime environment (Windows)
# if(WIN32)
#     set(VULKAN_RUNTIME_PATH "${VULKAN_SDK}/Lib")
#     add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         ${VULKAN_RUNTIME_PATH}/vulkan-1.dll
#         $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
# endif()
