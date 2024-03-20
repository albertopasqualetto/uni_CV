cmake_minimum_required(VERSION 3.27)
project(Lab1)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "C:/opencv/build")  # Path to the OpenCV build directory
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#add_executable(${PROJECT_NAME} task1.cpp)
#add_executable(${PROJECT_NAME} task2.cpp)
#add_executable(${PROJECT_NAME} task3.cpp)
#add_executable(${PROJECT_NAME} task4.cpp)
add_executable(${PROJECT_NAME} task5.cpp)


target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})