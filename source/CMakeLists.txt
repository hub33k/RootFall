# Generate version-header
configure_file(version.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.hpp)

# Sub-projects
# ================================================================

# Libraries
set(IDE_FOLDER "")
add_subdirectory(RootFallCore)

add_subdirectory(sdl3webgpu)

# Executables
set(IDE_FOLDER "")

# main game executable
add_subdirectory(RootFall)
