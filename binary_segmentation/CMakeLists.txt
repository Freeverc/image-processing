cmake_minimum_required(VERSION 3.10)

project(point_cluster)

find_package(Eigen3 REQUIRED)
find_package(PCL 1.8 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(${PCL_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

link_directories(${PCL_LIBRARY_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

add_definitions(${PCL_DEFINITIONS})

add_executable(point_cluster main.cpp cluster.h cluster.cpp)
# add_executable(point_cluster sc.cpp)

target_link_libraries(point_cluster ${PCL_LIBRARIES})
target_link_libraries(point_cluster ${OpenCV_LIBRARIES})