add_library(ptracer SHARED
            objects.cpp
            renderer.cpp
            pixel.cpp
            )
target_link_libraries(ptracer PUBLIC full_set_warnings)

add_executable(mk_image
                main.cpp)

target_link_libraries(mk_image PRIVATE ptracer)

