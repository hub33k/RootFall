# Global functions
# ================================================================

# Set policy if policy is available
function(set_policy POL VAL)
  if(POLICY ${POL})
    cmake_policy(SET ${POL} ${VAL})
  endif()
endfunction(set_policy)

# Define function "source_group_by_path with three mandatory arguments (PARENT_PATH, REGEX, GROUP, ...)
# to group source files in folders (e.g. for MSVC solutions).
#
# Example:
# source_group_by_path("${CMAKE_CURRENT_SOURCE_DIR}/src" "\\\\.h$|\\\\.inl$|\\\\.cpp$|\\\\.c$|\\\\.ui$|\\\\.qrc$" "Source Files" ${sources})
function(source_group_by_path PARENT_PATH REGEX GROUP)
  foreach (FILENAME ${ARGN})
    get_filename_component(FILEPATH "${FILENAME}" REALPATH)
    file(RELATIVE_PATH FILEPATH ${PARENT_PATH} ${FILEPATH})
    get_filename_component(FILEPATH "${FILEPATH}" DIRECTORY)

    string(REPLACE "/" "\\" FILEPATH "${FILEPATH}")

    source_group("${GROUP}\\${FILEPATH}" REGULAR_EXPRESSION "${REGEX}" FILES ${FILENAME})
  endforeach()
endfunction(source_group_by_path)

# Function that extract entries matching a given regex from a list.
# ${OUTPUT} will store the list of matching filenames.
function(list_extract OUTPUT REGEX)
  foreach(FILENAME ${ARGN})
    if(${FILENAME} MATCHES "${REGEX}")
      list(APPEND ${OUTPUT} ${FILENAME})
    endif()
  endforeach()

  set(${OUTPUT} ${${OUTPUT}} PARENT_SCOPE)
endfunction(list_extract)

# Creates an export header similar to generate_export_header, but for templates.
# The main difference is that for MSVC, templates must not get exported.
# When the file ${export_file} is included in source code, the macro ${target_id}_TEMPLATE_API
# may get used to define public visibility for templates on GCC and Clang platforms.
function(generate_template_export_header target target_id export_file)
  if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    configure_file(${PROJECT_SOURCE_DIR}/source/codegeneration/template_msvc_api.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/${export_file})
  else()
    configure_file(${PROJECT_SOURCE_DIR}/source/codegeneration/template_api.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/${export_file})
  endif()
endfunction()


# Global macros
# ================================================================

# https://github.com/oven-sh/bun/blob/main/cmake/Globals.cmake

# setx()
# Description:
#   Sets a variable, similar to `set()`, but also prints the value.
# Arguments:
#   variable string - The variable to set
#   value    string - The value to set the variable to
macro(setx)
  set(${ARGV})
  message(STATUS "Set ${ARGV0}: ${${ARGV0}}")
endmacro()
