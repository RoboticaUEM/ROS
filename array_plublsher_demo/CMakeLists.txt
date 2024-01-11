# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
project(array_plublsher_demo)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(pruebas src/pruebas.cpp)
target_link_libraries(pruebas ${catkin_LIBRARIES})

add_executable(array_push src/array_push.cpp)
target_link_libraries(array_push ${catkin_LIBRARIES})

add_executable(array_minimo src/array_minimo.cpp)
target_link_libraries(array_minimo ${catkin_LIBRARIES})

# %EndTag(FULLTEXT)%
