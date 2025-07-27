set(PONG "Pong")
set(PONG_DIR ${DIR_ROOT}/prototypes/${PONG})

set(PONG_SOURCE_DIR ${PONG_DIR}/source/${PONG})
set(PONG_HEADERS_DIR ${PONG_DIR})

set(PONG_SOURCES
  ${PONG_SOURCE_DIR}/Core/Application.cpp

  ${PONG_SOURCE_DIR}/Platform/WebGPU/WebGPUUtils.cpp

  ${PONG_SOURCE_DIR}/Renderer/Renderer.cpp
)

set(PONG_HEADERS
  ${PONG_SOURCE_DIR}/PongPCH.hpp

  ${PONG_SOURCE_DIR}/Core/Application.hpp
  ${PONG_SOURCE_DIR}/Core/Base.hpp

  ${PONG_SOURCE_DIR}/ECS/Components.hpp

  ${PONG_SOURCE_DIR}/Platform/WebGPU/WebGPUUtils.hpp

  ${PONG_SOURCE_DIR}/Renderer/Renderer.hpp
)

add_subdirectory(source)
