cmake_minimum_required(VERSION 3.5)
project(Lab8)

find_package(OpenCV REQUIRED)

set (CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(${PROJECT_NAME} src/task1.cpp)

add_library(Camera_Calibration src/cameraCalibration.cpp)

add_library(Utils src/Utils.cpp)

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
        Camera_Calibration
        Utils
)

target_link_libraries(Camera_Calibration
        ${OpenCV_LIBS}
)

target_link_libraries(Utils
        ${OpenCV_LIBS}
)
