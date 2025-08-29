set(EXAMPLER "Exampler")
set(EXAMPLER_DIR ${DIR_ROOT}/prototypes/${EXAMPLER})

set(EXAMPLER_SOURCE_DIR ${EXAMPLER_DIR}/source/${EXAMPLER})
set(EXAMPLER_HEADERS_DIR ${EXAMPLER_DIR})

set(EXAMPLER_SOURCES
  # ${EXAMPLER_SOURCE_DIR}/Core/Application.cpp
)

set(EXAMPLER_HEADERS
  ${EXAMPLER_SOURCE_DIR}/ExamplerPCH.hpp
)

add_subdirectory(source)
