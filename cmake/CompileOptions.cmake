# Platform and architecture setup
# ================================================================

# Get upper case system name
string(TOUPPER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_UPPER)

# Determine architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(X64 ON)
endif()


# Project options
# ================================================================

set(DEFAULT_PROJECT_OPTIONS
  DEBUG_POSTFIX               "d"
  CMAKE_CXX_STANDARD_REQUIRED ON
  CXX_STANDARD                23
  LINKER_LANGUAGE             "CXX"
  POSITION_INDEPENDENT_CODE   ON
  #COMPILE_WARNING_AS_ERROR   ON
  #CXX_VISIBILITY_PRESET      "hidden"
  CXX_EXTENSIONS              Off
)


# Include directories
# ================================================================

set(DEFAULT_INCLUDE_DIRECTORIES)

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
  LIST(APPEND DEFAULT_INCLUDE_DIRECTORIES "/usr/local/include")
endif()


# Libraries
# ================================================================

set(DEFAULT_LIBRARIES)


# Compile definitions
# ================================================================

set(DEFAULT_COMPILE_DEFINITIONS
  SYSTEM_${SYSTEM_NAME_UPPER}
)

# Variables in CPP
set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
  -DPROJECT_NAME="${PROJECT_NAME}"
  -DBINARY_DIR="${CMAKE_BINARY_DIR}"
)

# Build type
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
    -DBUILD_DEBUG
    -DPROJECT_ROOT_DIR="${DIR_ROOT}/"
    -DPROJECT_DATA_DIR="${DIR_DATA}/"
  )
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
    -DBUILD_RELEASE
    -DPROJECT_ROOT_DIR="/"
    -DPROJECT_DATA_DIR="data/"
  )
endif()

# MSVC compiler options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC" OR
  "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND
  "x${CMAKE_CXX_SIMULATE_ID}" MATCHES "xMSVC"
)
  set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
    _SCL_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the Standard C++ Library
    _CRT_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the CRT Library
  )
endif()


# Compile options
# ================================================================

set(DEFAULT_COMPILE_OPTIONS_PRIVATE)
set(DEFAULT_COMPILE_OPTIONS_PUBLIC)

# MSVC compiler options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  set(DEFAULT_COMPILE_OPTIONS_PRIVATE ${DEFAULT_COMPILE_OPTIONS_PRIVATE}
    $<$<CXX_COMPILER_ID:MSVC>:
      /MP           # -> build with multiple processes
    >
    /W4           # -> warning level 4
    # /WX         # -> treat warnings as errors
    /wd4251       # -> disable warning: 'identifier': class 'type' needs to have dll-interface to be used by clients of class 'type2'
    /wd4592       # -> disable warning: 'identifier': symbol will be dynamically initialized (implementation limitation)
    # /wd4201     # -> disable warning: nonstandard extension used: nameless struct/union (caused by GLM)
    /wd4127       # -> disable warning: conditional expression is constant (caused by Qt)

    # /Zm114      # -> Memory size for precompiled headers (insufficient for msvc 2013)
    /Zm200        # -> Memory size for precompiled headers

    $<$<CXX_COMPILER_ID:Clang>:
      -Wno-microsoft-cast
    >

    $<$<CONFIG:Debug>:
      /ZI
      /INCREMENTAL
      # /RTCc         # -> value is assigned to a smaller data type and results in a data loss
    >

    $<$<CONFIG:Release>:
      /Gw           # -> whole program global optimization
      /GS-          # -> buffer security check: no
      /GL           # -> whole program optimization: enable link-time code generation (disables Zi)
      /GF           # -> enable string pooling
    >

    # No manual c++11 enable for MSVC as all supported MSVC versions for cmake-init have C++11 implicitly enabled (MSVC >=2013)
  )
endif()


# GCC and Clang compiler options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND NOT MSVC)
  set(DEFAULT_COMPILE_OPTIONS_PRIVATE ${DEFAULT_COMPILE_OPTIONS_PRIVATE}
    #-fno-exceptions # since we use stl and stl is intended to use exceptions, exceptions should not be disabled

    -Wall
    -Wextra
    -Wunused

    -Wreorder
    -Wignored-qualifiers
    -Wmissing-braces
    -Wreturn-type
    -Wswitch
    -Wswitch-default
    -Wuninitialized
    -Wmissing-field-initializers

    $<$<CXX_COMPILER_ID:GNU>:
      -Wmaybe-uninitialized

      -Wno-unknown-pragmas

      $<$<VERSION_GREATER:$<CXX_COMPILER_VERSION>,4.8>:
        -Wpedantic
        -Wreturn-local-addr
      >
    >

    $<$<CXX_COMPILER_ID:Clang>:
      -Wpedantic

      $<$<PLATFORM_ID:Windows>:
        -Wno-language-extension-token
        -Wno-microsoft-cast
      >

      # -Wreturn-stack-address # gives false positives
    >

    # Custom
    -Wno-gnu-anonymous-struct
    -Wno-nested-anon-types
  )

  set(DEFAULT_COMPILE_OPTIONS_PUBLIC ${DEFAULT_COMPILE_OPTIONS_PUBLIC}
    $<$<PLATFORM_ID:Darwin>:
      -pthread
    >
  )
endif()

# CMake PCH + CCache options
# https://discourse.cmake.org/t/ccache-clang-and-fno-pch-timestamp/7253
# https://github.com/ccache/ccache/discussions/1218
# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
#   list(APPEND CMAKE_CXX_COMPILE_OPTIONS_CREATE_PCH -Xclang -fno-pch-timestamp)
# endif()


# Linker options
# ================================================================

set(DEFAULT_LINKER_OPTIONS)

# Use pthreads on mingw and linux
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  set(DEFAULT_LINKER_OPTIONS ${DEFAULT_LINKER_OPTIONS}
    PUBLIC
      -pthread
  )

  if(${OPTION_COVERAGE_ENABLED})
    set(DEFAULT_LINKER_OPTIONS ${DEFAULT_LINKER_OPTIONS}
      PUBLIC
        -fprofile-arcs
        -ftest-coverage
    )
  endif()
endif()


# ================================================================

# CMake PCH + CCache options
# https://discourse.cmake.org/t/ccache-clang-and-fno-pch-timestamp/7253
# https://github.com/ccache/ccache/discussions/1218
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
  list(APPEND CMAKE_CXX_COMPILE_OPTIONS_CREATE_PCH -Xclang -fno-pch-timestamp)
endif()
