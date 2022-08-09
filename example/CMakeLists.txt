cmake_minimum_required(VERSION 3.10.2)

project(example)

set(SOURCES
  example.cpp
)

#-- catkin stuff
find_package(catkin REQUIRED roscpp std_msgs ads)

catkin_package(
   LIBRARIES ${PROJECT_NAME}
   CATKIN_DEPENDS roscpp std_msgs
   message_runtime
   ads
   )
   
include_directories(
include
  ${catkin_INCLUDE_DIRS}
)

##--

#replace binary call (as the example.bin file is setup to be inside ads)
#add_executable(example.bin ${SOURCES})
#target_link_libraries(example.bin PUBLIC ${catkin_LIBRARIES})


add_executable(example ${SOURCES})
target_link_libraries(example ${catkin_LIBRARIES})


