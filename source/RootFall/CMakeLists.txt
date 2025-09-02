set(ROOTFALL "RootFall")
set(ROOTFALL_DIR ${DIR_ROOT}/source/${ROOTFALL})

set(ROOTFALL_SOURCE_DIR ${ROOTFALL_DIR}/source/${ROOTFALL})
set(ROOTFALL_HEADERS_DIR ${ROOTFALL_DIR})

set(ROOTFALL_SOURCES
  # ${ROOTFALL_SOURCE_DIR}/Core/Application.cpp
)

set(ROOTFALL_HEADERS
  ${ROOTFALL_SOURCE_DIR}/RootFallPCH.hpp
)

add_subdirectory(source)
add_subdirectory(tests)
