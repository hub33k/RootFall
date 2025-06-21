set(target ${ROOTFALL_CORE})
set(target_dir ${ROOTFALL_CORE_DIR}/source)

message(STATUS "[Library] ${target}")

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${ROOTFALL_CORE_HEADERS_DIR} "\\\\.h$|\\\\.hpp$"
  ${header_group} ${ROOTFALL_CORE_HEADERS}
)
source_group_by_path(${ROOTFALL_CORE_SOURCE_DIR}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$"
  ${source_group} ${ROOTFALL_CORE_SOURCES}
)

# Create library
add_library(${target} OBJECT ${ROOTFALL_CORE_SOURCES} ${ROOTFALL_CORE_HEADERS})

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

  spdlog

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
# target_precompile_headers(${target}
#   PRIVATE
#   ${ROOTFALL_CORE_SOURCE_DIR}/RootFallCorePCH.hpp
# )
