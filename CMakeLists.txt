cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(DoomWeek VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG v0.1.36
)
set(GLAD_PROFILE   "core" CACHE STRING "OpenGL profile")
set(GLAD_API       "gl=3.3" CACHE STRING "API version")
set(GLAD_GENERATOR "c" CACHE STRING "Language")
FetchContent_MakeAvailable(glad)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(DoomWeek ${SOURCES})

target_include_directories(DoomWeek PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${glad_SOURCE_DIR}/include
)

target_link_libraries(DoomWeek PRIVATE
        glfw
        glad
        glm
        GL
        dl
        pthread
)