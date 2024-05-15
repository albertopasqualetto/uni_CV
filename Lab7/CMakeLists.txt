cmake_minimum_required(VERSION 3.5)
project(Lab7)

find_package(OpenCV REQUIRED)

set (CMAKE_CXX_STANDARD 17)

include_directories(include
                    PanoramicUtils/header
)

add_executable(${PROJECT_NAME} src/task1.cpp)

add_library(PanoramicUtils
        PanoramicUtils/header/panoramic_utils.h
        PanoramicUtils/panoramic_utils.cpp
)
target_link_libraries(PanoramicUtils
        ${OpenCV_LIBS}
)

add_library(Match
        include/match.h
        src/match.cpp
)

target_link_libraries(Match
        ${OpenCV_LIBS}
)

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
        PanoramicUtils
        Match
)