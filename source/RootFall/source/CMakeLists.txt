set(target ${ROOTFALL})
set(target_dir ${ROOTFALL_DIR}/source)

message(STATUS "[Executable] ${target}")

add_executable(${target}
  # MACOSX_BUNDLE
  ${ROOTFALL_SOURCES} ${ROOTFALL_HEADERS}
)

# Project options
set_target_properties(${target}
  PROPERTIES
  ${DEFAULT_PROJECT_OPTIONS}
  INSTALL_RPATH "${EXECUTABLE_INSTALL_RPATH}"
  FOLDER "${IDE_FOLDER}"
)

# Include directories
target_include_directories(${target}
  PRIVATE
  ${DEFAULT_INCLUDE_DIRECTORIES}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PROJECT_BINARY_DIR}/source/include
  ${target_dir} # added
)

# Libraries
target_link_libraries(${target}
  PRIVATE
  ${DEFAULT_LIBRARIES}

  RootFallCore

  sdl3webgpu

  dawn
  entt
  glm
  imgui
  SDL3
  spdlog
  stb
)

# Compile definitions
target_compile_definitions(${target}
  PRIVATE
  ${DEFAULT_COMPILE_DEFINITIONS}
)

# Compile options
target_compile_options(${target}
  PRIVATE
  ${DEFAULT_COMPILE_OPTIONS_PRIVATE}

  PUBLIC
  ${DEFAULT_COMPILE_OPTIONS_PUBLIC}
)

# Linker options
target_link_options(${target}
  PRIVATE
  ${DEFAULT_LINKER_OPTIONS}
)

# Precompiled headers - PCH
target_precompile_headers(${target}
  PRIVATE
  ${ROOTFALL_SOURCE_DIR}/RootFallPCH.hpp

  # https://discourse.cmake.org/t/leveraging-precompiled-headers/10137
  # https://cmake.org/cmake/help/latest/command/target_precompile_headers.html#reusing-precompile-headers
  # REUSE_FROM ${ROOTFALL_CORE}
  # ${ROOTFALL_CORE_DIR}/source/RootFallCore/RootFallCorePCH.hpp
)

# Setup vendors
setup_dawn(${target})
