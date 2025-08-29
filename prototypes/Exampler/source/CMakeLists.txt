set(target ${EXAMPLER})
set(target_dir ${EXAMPLER_DIR}/source)
set(target_source_dir ${EXAMPLER_SOURCE_DIR})
set(target_headers_dir ${EXAMPLER_HEADERS_DIR})

message(${target_dir})
message(${EXAMPLER_SOURCE_DIR})
message(${EXAMPLER_HEADERS_DIR})

message(STATUS "[Executable] ${target}")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/${target})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/${target})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/${target})

add_executable(${target}
  # MACOSX_BUNDLE
  ${EXAMPLER_SOURCE_DIR}/${target}Build.cpp
  ${EXAMPLER_SOURCE_DIR}/${target}Main.cpp
  ${EXAMPLER_SOURCE_DIR}/${target}PCH.cpp

  ${EXAMPLER_SOURCES} ${EXAMPLER_HEADERS}
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
  ${EXAMPLER_SOURCE_DIR}/${target}PCH.hpp

  # https://discourse.cmake.org/t/leveraging-precompiled-headers/10137
  # https://cmake.org/cmake/help/latest/command/target_precompile_headers.html#reusing-precompile-headers
  # REUSE_FROM ${EXAMPLER_CORE}
  # ${EXAMPLER_CORE_DIR}/source/RootFallCore/RootFallCorePCH.hpp
)

# Setup vendors
# setup_dawn(${target})
