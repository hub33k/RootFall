set(PONG "Pong")
set(PONG_DIR ${DIR_ROOT}/prototypes/${PONG})

set(PONG_SOURCE_DIR ${PONG_DIR}/source/${PONG})
set(PONG_HEADERS_DIR ${PONG_DIR})

set(PONG_SOURCES
  ${PONG_SOURCE_DIR}/PongMain.cpp
  ${PONG_SOURCE_DIR}/PongBuild.cpp
  ${PONG_SOURCE_DIR}/PongPCH.cpp

  ${PONG_SOURCE_DIR}/Core/Application.cpp

  ${PONG_SOURCE_DIR}/Platform/WebGPU/WebGPUUtils.cpp
)
set(PONG_HEADERS)

add_subdirectory(source)
