cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
message(STATUS "Configuring RootFall")

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake
)

# Setup paths
get_filename_component(DIR_ROOT . ABSOLUTE)
set(DIR_CMAKE ${DIR_ROOT}/cmake)
set(DIR_DATA ${DIR_ROOT}/data)
set(DIR_DOCS ${DIR_ROOT}/docs)
set(DIR_SCRIPTS ${DIR_ROOT}/scripts)
set(DIR_SOURCE ${DIR_ROOT}/source)
set(DIR_TOOLS ${DIR_ROOT}/tools)
set(DIR_VENDOR ${DIR_ROOT}/vendor)

set(DIR_BUILD ${CMAKE_BINARY_DIR})
set(DIR_BUILD_VENDOR ${DIR_ROOT}/build-vendor)


# Project description and (meta) information
# ================================================================

# Get git revision
execute_process(
  COMMAND git rev-parse HEAD
  WORKING_DIRECTORY ${DIR_ROOT}
  OUTPUT_VARIABLE GIT_SHA1
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)
string(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
if(NOT GIT_REV)
  set(GIT_REV "0")
endif()

# Meta information about the project
set(META_PROJECT_NAME        "RootFall")
set(META_PROJECT_DESCRIPTION "RootFall description")
set(META_AUTHOR_ORGANIZATION "hub33k")
set(META_AUTHOR_DOMAIN       "https://github.com/hub33k/RootFall")
set(META_AUTHOR_MAINTAINER   "hub33k@hub33k.pl")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "0")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
# set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION}")
set(META_CMAKE_INIT_SHA      "${GIT_REV}")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)
# string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_NORMALIZED)
# TODO (hub33k): add META_PROJECT_NORMALIZED - eg. if META_PROJECT_NAME is "Game Sandbox Two" then META_PROJECT_NORMALIZED is "GameSandboxTwo"


# PreConfigure
# Config that needs to be executed before `project()`
# ================================================================

include(FetchContent)
include(GenerateExportHeader)
include(CheckCXXCompilerFlag)
# include(WriteCompilerDetectionHeader)

include(Utils)
include(Common)

# Set policies
set_policy(CMP0054 NEW) #  ENABLE CMP0054: Only interpret if() arguments as variables or keywords when unquoted.
set_policy(CMP0042 NEW) #  ENABLE CMP0042: MACOSX_RPATH is enabled by default.
set_policy(CMP0063 NEW) #  ENABLE CMP0063: Honor visibility properties for all target types.
set_policy(CMP0069 NEW) #  ENABLE CMP0069: INTERPROCEDURAL_OPTIMIZATION is enforced when enabled.
set_policy(CMP0077 NEW) #  ENABLE CMP0077: option() honors normal variables.
# https://cmake.org/cmake/help/latest/policy/CMP0120.html
# set_policy(CMP0120 OLD) # DISABLE CMP0120: The WriteCompilerDetectionHeader module is removed.

# Project options
option(TSWG_BUILD_SHARED_LIBS "Build shared instead of static libraries."              ON)
option(TSWG_SELF_CONTAINED    "Create a self-contained install with all dependencies." OFF)
option(TSWG_BUILD_TESTS       "Build tests."                                           ON)
option(TSWG_BUILD_DOCS        "Build documentation."                                   OFF)
option(TSWG_BUILD_EXAMPLES    "Build examples."                                        OFF)
option(TSWG_ENABLE_COVERAGE   "Add coverage information."                              OFF)

# option(TSWG_ENABLE_TESTING   "Enable testing"                                ON)
# option(TSWG_DEV_MODE         "Set up development helper settings"            ON)


# Project
# ================================================================

# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "")

# Declare project
project(${META_PROJECT_NAME}
  VERSION ${META_VERSION}
  DESCRIPTION ${META_PROJECT_DESCRIPTION}
)

enable_language(C)
enable_language(CXX)


# Configuration
# ================================================================

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Create version file
file(WRITE "${PROJECT_BINARY_DIR}/VERSION" "${META_NAME_VERSION}")

# Compiler settings and options
include(CompileOptions)


# Project modules
#  ================================================================

add_subdirectory(vendor)
add_subdirectory(source)

# Sandbox
if(EXISTS "${DIR_ROOT}/CMakeListsSandbox.cmake")
  include(CMakeListsSandbox.cmake)
endif()

if(EXISTS "${DIR_SOURCE}/sandbox/CMakeLists.txt")
  add_subdirectory(source/sandbox)
endif()
