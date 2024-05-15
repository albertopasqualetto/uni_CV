cmake_minimum_required(VERSION 3.5)
project(Lab8)

find_package(OpenCV REQUIRED)

set (CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(${PROJECT_NAME} src/task1.cpp)

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
)