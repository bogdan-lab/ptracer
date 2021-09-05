add_library(ptracer SHARED
            objects.cpp
            pixel.cpp
            PngWriter.cpp
            )
target_link_libraries(ptracer PUBLIC full_set_warnings CONAN_PKG::libpng)

add_executable(mk_image
                main.cpp)

target_link_libraries(mk_image PRIVATE ptracer)

