# TODO (hub33k): create variables

add_library(RootFallExternal INTERFACE)

target_link_libraries(RootFallExternal
  INTERFACE
  # dawn
  EnTT
  glm
  imgui
  SDL3::SDL3-static
  spdlog
  stb
)

# disable warnings from external libraries
# https://stackoverflow.com/questions/56707123/how-to-suppress-clang-warnings-in-third-party-library-header-file-in-cmakelists
set_target_properties(EnTT PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:EnTT,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(spdlog PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:spdlog,INTERFACE_INCLUDE_DIRECTORIES>)

# target_include_directories(RootFallExternal
#   SYSTEM INTERFACE
#     ${DIR_SOURCE}/vendor/entt/code/src
# )
