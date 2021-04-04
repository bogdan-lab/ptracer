add_executable(ptracer
            main.cpp
            objects.cpp
            renderer.cpp
            pixel.cpp
            )

target_link_libraries(ptracer PRIVATE full_set_warnings)
