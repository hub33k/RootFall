#set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)
#message(${FETCHCONTENT_BASE_DIR})

# include(cmake/GoogleTest.cmake)

# include(cmake/dawn.cmake)
# include(cmake/entt.cmake)
# include(cmake/glm.cmake)
# include(cmake/imgui.cmake)
# include(cmake/sdl.cmake)
# include(cmake/spdlog.cmake)
# include(cmake/stb.cmake)

add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(sdl3)
