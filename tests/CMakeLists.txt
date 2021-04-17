
add_executable(unit_tests
    main.cpp
    GeoVecTests.cpp
    ColorTests.cpp
    RayTests.cpp
    ObjTests.cpp
    CameraTests.cpp
    )

target_link_libraries(unit_tests PRIVATE ptracer CONAN_PKG::gtest)

add_test(
    NAME UnitTests
    COMMAND unit_tests)
