set(target ${ROOTFALLC})
set(target_dir ${ROOTFALLC_DIR}/source)
set(target_source_dir ${ROOTFALLC_SOURCE_DIR})
set(target_headers_dir ${ROOTFALLC_HEADERS_DIR})
set(target_sources ${ROOTFALLC_SOURCES})
set(target_headers ${ROOTFALLC_HEADERS})

message(STATUS "[Executable] ${target}")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(${target}
  # MACOSX_BUNDLE
  # ${target_source_dir}/${target}Build.c
  ${target_source_dir}/${target}Main.c
  ${target_source_dir}/${target}PCH.c

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

  dawn
  imgui
  SDL3-static
  stb
)

set_target_properties(dawn PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:dawn,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(imgui PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:imgui,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(SDL3-static PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:SDL3-static,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(stb PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:stb,INTERFACE_INCLUDE_DIRECTORIES>)

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
  ${target_source_dir}/${target}PCH.h
)
