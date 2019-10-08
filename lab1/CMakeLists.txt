cmake_minimum_required(VERSION 3.15)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1 main.cpp libs/matrix.cpp libs/approximation.cpp libs/approximation.h libs/rectangle.cpp libs/rectangle.h)

# Where to find CMake modules and OpenCV
set(OpenCV_DIR D:\\STV\\opencv-master\\build_1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${Opencv_INCLUDE_DIRS})


# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(lab1 ${OpenCV_LIBS})