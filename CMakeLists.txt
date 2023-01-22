cmake_minimum_required(VERSION 3.8)
project(son8samplegl LANGUAGES C CXX)

add_library(${PROJECT_NAME} source/main.cxx)

target_compile_features(${PROJECT_NAME}
    PRIVATE cxx_std_17
    PRIVATE c_std_11
)

add_subdirectory(coatglop)
add_subdirectory(libs/glfw)

target_link_libraries(${PROJECT_NAME} son8coatglop glfw)
target_include_directories(${PROJECT_NAME}
    PUBLIC include
    PRIVATE source
)

add_subdirectory(samples/glfw)
add_subdirectory(samples/glad)
add_subdirectory(samples/son8samplegl)