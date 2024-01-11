cmake_minimum_required(VERSION 3.0.2)
project(find_color)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

find_package(
  OpenCV REQUIRED 
)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(
	include
	${catkin_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS}
)

add_executable(yeloblueJson src/yeloblueJson.cpp)
target_link_libraries(yeloblueJson ${catkin_LIBRARIES} ${OpenCV_LIBS})

add_executable(json_parse src/json_parse.cpp)
target_link_libraries(json_parse ${catkin_LIBRARIES} ${OpenCV_LIBS})
