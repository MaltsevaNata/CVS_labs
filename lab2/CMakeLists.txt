cmake_minimum_required(VERSION 3.15)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab2 main.cpp)

# Where to find CMake modules and OpenCV
set(OpenCV_DIR D:\\STV\\opencv-master\\build_1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${Opencv_INCLUDE_DIRS})


# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(lab2 ${OpenCV_LIBS})