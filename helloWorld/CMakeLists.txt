cmake_minimum_required(VERSION 3.27)
project(helloWorld)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "C:/opencv/build")  # Path to the OpenCV build directory
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(helloWorld main.cpp)

target_link_libraries(helloWorld ${OpenCV_LIBS})