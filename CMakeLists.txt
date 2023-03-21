cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

project(link_game LANGUAGES CXX C)

FILE(GLOB_RECURSE   PIXEL_FILES   
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
                    )

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.16)
    set(CMAKE_UNITY_BUILD_BATCH_SIZE 16) # 10 ～ 20
endif()

add_executable(${PROJECT_NAME} ${PIXEL_FILES})

add_subdirectory(/home/sunirein/Desktop/OpenGL-cpp build)

target_include_directories(${PROJECT_NAME} PUBLIC

    # === 跨平台宏文件（cmake创建） ===
    ${EXECUTABLE_OUTPUT_PATH}/sysconfig

    # === 第三方库 ===
    /home/sunirein/Desktop/OpenGL-cpp/include

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

target_link_libraries(${PROJECT_NAME} openGL_cpp
                      )

# 编译选项，可以自行增删
target_compile_options(${PROJECT_NAME} PRIVATE
                       -Wall -Wextra
                       )