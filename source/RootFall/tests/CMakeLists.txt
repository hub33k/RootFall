set(target ${ROOTFALL}_tests)
set(target_dir ${ROOTFALL_DIR}/tests)

enable_testing()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/tests)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/tests)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/tests)

add_executable(${target}
  RootFall/TestExample.cpp
)

target_link_libraries(${target}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${target})
