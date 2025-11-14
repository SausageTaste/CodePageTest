cmake_minimum_required(VERSION 3.11)

project(CodePageTest)


add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
