set(ROOTFALL "RootFall" CACHE STRING "RootFall")
set(ROOTFALL_DIR ${DIR_ROOT}/source/${ROOTFALL})

set(ROOTFALL_SOURCE_DIR ${ROOTFALL_DIR}/source/${ROOTFALL})
set(ROOTFALL_HEADERS_DIR ${ROOTFALL_DIR})

set(ROOTFALL_SOURCES
  ${ROOTFALL_SOURCE_DIR}/Game/Application.cpp

  ${ROOTFALL_SOURCE_DIR}/Platform/WebGPU/WebGPUUtils.cpp

  ${ROOTFALL_SOURCE_DIR}/Renderer/Renderer.cpp

  ${ROOTFALL_SOURCE_DIR}/Utils/Utils.cpp
)

set(ROOTFALL_HEADERS
  ${ROOTFALL_SOURCE_DIR}/RootFallPCH.hpp

  ${ROOTFALL_SOURCE_DIR}/Game/Application.hpp

  ${ROOTFALL_SOURCE_DIR}/Platform/WebGPU/WebGPUUtils.hpp

  ${ROOTFALL_SOURCE_DIR}/Renderer/Renderer.hpp

  ${ROOTFALL_SOURCE_DIR}/Utils/Utils.hpp
)

add_subdirectory(source)
add_subdirectory(tests)
