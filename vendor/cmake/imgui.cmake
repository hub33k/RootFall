set(imgui_target imgui)
set(imgui_vendor_dir ${DIR_ROOT}/build-vendor/imgui)
set(imgui_code_dir ${imgui_vendor_dir}/code)

message(STATUS "[vendor] Using imgui")

add_library(${imgui_target} STATIC
  ${imgui_code_dir}/imgui.cpp
  ${imgui_code_dir}/imgui_demo.cpp
  ${imgui_code_dir}/imgui_draw.cpp
  ${imgui_code_dir}/imgui_tables.cpp
  ${imgui_code_dir}/imgui_widgets.cpp
)
add_library(${imgui_target}::${imgui_target} ALIAS ${imgui_target})

target_include_directories(${imgui_target}
  SYSTEM
  INTERFACE
  ${imgui_code_dir}
)

set_property(TARGET ${imgui_target} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(
  ${imgui_target} INTERFACE "${imgui_code_dir}"
)
set_target_properties(${imgui_target} PROPERTIES FOLDER "${imgui_code_dir}")
