cmake_minimum_required(VERSION 3.19)

project(PathTracer
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(include/)

#WARNINGS
add_library(full_set_warnings INTERFACE)
set(WARNINGS_AS_ERRORS FALSE)
include(../cmake/CompilerWarnings.cmake)
set_project_warnings(full_set_warnings)


#CONAN
find_program(CONAN conan)
if (NOT CONAN)
    message(FATAL_ERROR "Needs conan package manager: please visit https://conan.io and install")
endif ()
include(../cmake/conan.cmake)
conan_check(VERSION 1.14.4 REQUIRED)
conan_cmake_run(REQUIRES
                gtest/1.10.0
                libpng/1.6.37
        OPTIONS BASIC_SETUP CMAKE_TARGETS
        BUILD missing)


if(BENCHMARK)
    add_compile_options(-fno-omit-frame-pointer)
endif()


add_subdirectory(src)
add_subdirectory(benchmarks)
enable_testing()
add_subdirectory(tests)


