set(ROOTFALL_EXTERNAL "RootFallExternal" CACHE STRING "RootFallExternal")
set(ROOTFALL_EXTERNAL_DIR ${DIR_SOURCE}/${ROOTFALL_EXTERNAL})

add_library(${ROOTFALL_EXTERNAL} INTERFACE)

target_link_libraries(${ROOTFALL_EXTERNAL}
  INTERFACE
  EnTT
  glm
  imgui
  SDL3-static
  spdlog
  stb

  sdl3webgpu
)

if(EMSCRIPTEN)
  target_link_libraries(${ROOTFALL_EXTERNAL}
    INTERFACE
    emdawnwebgpu_cpp
  )
  set_target_properties(emdawnwebgpu_cpp PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:emdawnwebgpu_cpp,INTERFACE_INCLUDE_DIRECTORIES>)
endif()

if(NOT EMSCRIPTEN)
  target_link_libraries(${ROOTFALL_EXTERNAL}
    INTERFACE
    webgpu_dawn
  )
  set_target_properties(webgpu_dawn PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:webgpu_dawn,INTERFACE_INCLUDE_DIRECTORIES>)
endif()

# disable warnings from external libraries
# https://stackoverflow.com/questions/56707123/how-to-suppress-clang-warnings-in-third-party-library-header-file-in-cmakelists
set_target_properties(EnTT PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:EnTT,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(glm PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(imgui PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:imgui,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(SDL3-static PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:SDL3-static,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(spdlog PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:spdlog,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(stb PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:stb,INTERFACE_INCLUDE_DIRECTORIES>)

# target_include_directories(RootFallExternal
#   SYSTEM INTERFACE
#     ${DIR_SOURCE}/vendor/entt/code/src
# )
