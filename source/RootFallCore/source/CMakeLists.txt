set(target ${ROOTFALL_CORE})
set(target_dir ${ROOTFALL_CORE_DIR}/source)
set(target_source_dir ${ROOTFALL_CORE_SOURCE_DIR})
set(target_headers_dir ${ROOTFALL_CORE_HEADERS_DIR})
set(target_sources ${ROOTFALL_CORE_SOURCES})
set(target_headers ${ROOTFALL_CORE_HEADERS})

message(STATUS "[Library] ${target}")

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${target_headers_dir} "\\\\.h$|\\\\.hpp$"
  ${header_group} ${target_headers}
)
source_group_by_path(${target_source_dir}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$"
  ${source_group} ${target_sources}
)

# Create library
add_library(${target} OBJECT
  ${target_source_dir}/${target}PCH.cpp

  ${target_sources} ${target_headers}
)

# Project options
set_target_properties(${target}
  PROPERTIES
  ${DEFAULT_PROJECT_OPTIONS}
  FOLDER "${IDE_FOLDER}"
  VERSION "${META_VERSION}"
  SOVERSION "${META_VERSION_MAJOR}"
)

# Include directories
target_include_directories(${target}
  PRIVATE
  ${PROJECT_BINARY_DIR}/source/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include

  PUBLIC
  ${DEFAULT_INCLUDE_DIRECTORIES}
  ${target_dir} # added

  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Libraries
target_link_libraries(${target}
  PRIVATE

  PUBLIC
  ${DEFAULT_LIBRARIES}

  ${ROOTFALL_EXTERNAL}

  INTERFACE
)

# Compile definitions
target_compile_definitions(${target}
  PRIVATE

  PUBLIC
  $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_id}_STATIC_DEFINE>
  ${DEFAULT_COMPILE_DEFINITIONS}

  INTERFACE
)

# Compile options
target_compile_options(${target}
  PRIVATE
  ${DEFAULT_COMPILE_OPTIONS_PRIVATE}

  PUBLIC
  ${DEFAULT_COMPILE_OPTIONS_PUBLIC}

  INTERFACE
)

# Linker options
target_link_options(${target}
  PRIVATE

  PUBLIC
  ${DEFAULT_LINKER_OPTIONS}

  INTERFACE
)

# Precompiled headers - PCH
target_precompile_headers(${target}
  PRIVATE
  ${target_source_dir}/${target}PCH.hpp
)
