set(ROOTFALLC "RootFallC" CACHE STRING "RootFallC")
set(ROOTFALLC_DIR ${DIR_ROOT}/source/${ROOTFALLC})

set(ROOTFALLC_SOURCE_DIR ${ROOTFALLC_DIR}/source/${ROOTFALLC})
set(ROOTFALLC_HEADERS_DIR ${ROOTFALLC_DIR})

set(ROOTFALLC_SOURCES
  # ${ROOTFALLC_SOURCE_DIR}/Game/Application.cpp
)

set(ROOTFALLC_HEADERS
  ${ROOTFALLC_SOURCE_DIR}/${ROOTFALLC}PCH.h

  # ${ROOTFALLC_SOURCE_DIR}/Game/Application.hpp
)

add_subdirectory(source)
add_subdirectory(tests)
