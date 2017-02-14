cmake_minimum_required(VERSION 3.5)
project(cv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
find_package( OpenCV REQUIRED )
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )