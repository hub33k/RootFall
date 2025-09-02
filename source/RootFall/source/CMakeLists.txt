set(target ${ROOTFALL})
set(target_dir ${ROOTFALL_DIR}/source)
set(target_source_dir ${ROOTFALL_SOURCE_DIR})
set(target_headers_dir ${ROOTFALL_HEADERS_DIR})
set(target_sources ${ROOTFALL_SOURCES})
set(target_headers ${ROOTFALL_HEADERS})

message(STATUS "[Executable] ${target}")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(${target}
  # MACOSX_BUNDLE
  ${target_source_dir}/${target}Build.cpp
  ${target_source_dir}/${target}Main.cpp
  ${target_source_dir}/${target}PCH.cpp

  ${target_sources} ${target_headers}
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

  ${ROOTFALL_EXTERNAL}
  ${ROOTFALL_CORE}
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
  ${target_source_dir}/${target}PCH.hpp

  # https://discourse.cmake.org/t/leveraging-precompiled-headers/10137
  # https://cmake.org/cmake/help/latest/command/target_precompile_headers.html#reusing-precompile-headers
  # REUSE_FROM ${ROOTFALL_CORE}
  # ${ROOTFALL_CORE_DIR}/source/RootFallCore/RootFallCorePCH.hpp
)

# Emscripten
if(EMSCRIPTEN)
  # https://dawn.googlesource.com/dawn/+/refs/heads/main/src/emdawnwebgpu/pkg/README.md
  # https://github.com/emscripten-core/emscripten/blob/main/tools/ports/emdawnwebgpu.py

  message(STATUS "Emscripten build ${target}")

  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set_target_properties(${target} PROPERTIES SUFFIX ".html")

  target_link_options(${target} PRIVATE
    -sUSE_WEBGPU=1 # Handle WebGPU symbols
    # --use-port=emdawnwebgpu
    # em++: warning: USE_WEBGPU is deprecated (please try migrating to --use-port=emdawnwebgpu, which implements a newer, incompatible version of webgpu.h (see tools/ports/emdawnwebgpu.py for more info)). Please open a bug if you have a continuing need for this setting [-Wdeprecated]
    --closure=1
    -sUSE_GLFW=3
    -sASYNCIFY # Required by WebGPU-C++
    # -sINITIAL_MEMORY=1GB
    -sALLOW_MEMORY_GROWTH=1
    # -sTOTAL_STACK=512MB
    # -sUSE_PTHREADS=1
    --preload-file "${DIR_DATA}"
  )

  # target_compile_options(${target}
  #   PUBLIC
  #   -pthread
  # )
endif()

# Not Emscripten
if(NOT EMSCRIPTEN)
  message(STATUS "Not Emscripten build ${target}")

  # target_link_libraries(${target}
  #   PRIVATE
  #   ${DEFAULT_LIBRARIES}
  #   dawn
  # )

  # Setup vendors
  setup_dawn(${target})
endif()
