cmake_minimum_required(VERSION 3.15)
project(GLCube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files
file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/GLCube/src/*.cpp
    ${CMAKE_SOURCE_DIR}/GLCube/src/*.c
    ${CMAKE_SOURCE_DIR}/GLCube/src/*.h
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# -------------------------
# Dependencies
# -------------------------

find_package(OpenGL REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

include(FetchContent)

# Getting GLFW
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(GLFW_LIBRARY_TYPE "Link glfw static or dynamic" STATIC)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_INSTALL "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG        3.3.8
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
message("Fetching glfw")
FetchContent_MakeAvailable(glfw)


# Getting GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG        master
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

message("Fetching glm")
FetchContent_MakeAvailable(glm)
target_compile_definitions(glm INTERFACE GLM_FORCE_SILENT_WARNINGS)

# Getting stb_image
FetchContent_Declare(
    stb_image
    GIT_REPOSITORY  https://github.com/nothings/stb.git
    GIT_TAG         master
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
)
FetchContent_GetProperties(stb_image)
if(NOT stb_image_POPULATED)
    FetchContent_MakeAvailable(stb_image)
    message("Fetching stb_image")

    add_library(stb_image INTERFACE ${stb_image_SOURCE_DIR}/stb_image.h)
    target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm stb_image)
#####################
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty)

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# -------------------------
# Copy assets folder after build
# -------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/GLCube/Assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets"
)

# -------------------------
# Set working directory (for Visual Studio, CLion, etc.)
# -------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLCube)
