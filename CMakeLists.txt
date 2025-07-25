﻿cmake_minimum_required(VERSION 3.12)

project ("Vulkan-Engine")


find_package(Vulkan REQUIRED)

add_subdirectory(third_party)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_subdirectory(src)

find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)


file(GLOB_RECURSE GLSL_SOURCE_FILES
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/shaders/*.comp"
    )


foreach(GLSL ${GLSL_SOURCE_FILES})
  message(STATUS "BUILDING SHADER")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR}
          -V
          "-I${PROJECT_SOURCE_DIR}/shaders"  
          "${GLSL}"
          -o "${SPIRV}"
  DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

# turn on virtual folders in VS
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# group by file‐extension
source_group("Shaders/Vertex"    REGULAR_EXPRESSION "^${PROJECT_SOURCE_DIR}/shaders/.*\\.vert$")
source_group("Shaders/Fragment"  REGULAR_EXPRESSION "^${PROJECT_SOURCE_DIR}/shaders/.*\\.frag$")
source_group("Shaders/Compute"   REGULAR_EXPRESSION "^${PROJECT_SOURCE_DIR}/shaders/.*\\.comp$")

# register the Shaders target so VS actually lists them
add_custom_target(Shaders
    SOURCES
      ${GLSL_SOURCE_FILES}
    DEPENDS
      ${SPIRV_BINARY_FILES}
)