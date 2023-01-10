cmake_minimum_required(VERSION 3.8)
project(son8samplegl LANGUAGES C CXX)

add_executable(${PROJECT_NAME} source/main.cxx)

target_compile_features(${PROJECT_NAME}
    PRIVATE cxx_std_17
    PRIVATE c_std_11
)

target_link_libraries(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
    PUBLIC include
    PRIVATE source
)