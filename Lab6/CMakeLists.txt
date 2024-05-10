cmake_minimum_required(VERSION 3.5)
project(Lab6)

find_package(OpenCV REQUIRED)

include_directories(include)

add_executable(${PROJECT_NAME} src/task1.cpp)

add_library(Matchers
        include/matchers.h
        src/matchers.cpp
)

target_link_libraries(Matchers
        ${OpenCV_LIBS}
)

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
        Matchers
)