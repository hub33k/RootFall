set(ROOTFALL "RootFall")
set(ROOTFALL_DIR ${DIR_SOURCE}/${ROOTFALL})

set(ROOTFALL_SOURCE_DIR ${ROOTFALL_DIR}/source/${ROOTFALL})
set(ROOTFALL_HEADERS_DIR ${ROOTFALL_DIR})

set(ROOTFALL_SOURCES
  ${ROOTFALL_SOURCE_DIR}/RootFallMain.cpp
  ${ROOTFALL_SOURCE_DIR}/RootFallBuild.cpp
  ${ROOTFALL_SOURCE_DIR}/RootFallPCH.cpp

  ${ROOTFALL_SOURCE_DIR}/Core/Application.cpp

  ${ROOTFALL_SOURCE_DIR}/Renderer/Renderer.cpp

  ${ROOTFALL_SOURCE_DIR}/Platform/Window.cpp
  ${ROOTFALL_SOURCE_DIR}/Platform/SDL/SDLWindow.cpp
  ${ROOTFALL_SOURCE_DIR}/Platform/WebGPU/WebGPUUtils.cpp
)
set(ROOTFALL_HEADERS)

add_subdirectory(source)
