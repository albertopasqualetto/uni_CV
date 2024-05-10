cmake_minimum_required(VERSION 3.5)
project(Lab5)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} task1.cpp)
#add_executable(${PROJECT_NAME} task2.cpp)
#add_executable(${PROJECT_NAME} task3.cpp)


target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})