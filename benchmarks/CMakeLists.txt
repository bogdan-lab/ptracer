
add_executable(bench_simple_scene SimpleSceneBenchmark.cpp)
target_link_libraries(bench_simple_scene PRIVATE ptracer)


set(BENCHMARK_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(${CMAKE_SOURCE_DIR}/generated/benchmark_info.h.in
    ${CMAKE_BINARY_DIR}/generated/benchmark_info.h)

target_include_directories(bench_simple_scene PRIVATE ${CMAKE_BINARY_DIR}/generated)
