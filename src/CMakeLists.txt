add_library(ptracer SHARED
            objects.cpp
            pixel.cpp
            config.cpp
            PngWriter.cpp
            )

target_include_directories(ptracer PUBLIC ${NLOHMANN_JSON_PATH}/include)
target_link_libraries(ptracer PUBLIC full_set_warnings CONAN_PKG::libpng nlohmann_json::nlohmann_json)

add_executable(mk_image
                main.cpp)

target_link_libraries(mk_image PRIVATE ptracer)

