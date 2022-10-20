cmake_minimum_required(VERSION 3.10.2)

project(adsros)

find_package(Threads)

#---
# Added to enable catkin to build this package (locate catkin step)
find_package(catkin REQUIRED)

# The catkin_package macro generates cmake config files for your package (config step)
catkin_package(
   INCLUDE_DIRS include
   LIBRARIES ${PROJECT_NAME}
   )
   

# set the include directory, needs to be called include (config step)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

#---
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(COMPILE_OPTIONS
    -pedantic
    -Wall
    -Wextra
  )
  add_definitions(-D_GNU_SOURCE)
  
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Compiler flags and definitions for Visual Studio come here
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# catkin edit- need to put the header files in include or else catkin cannot see it
add_subdirectory(include/AdsLib)

# Catkin edits, need to install the target to Catkin as well as all of the header files inside include, special command is used to only detect the .h files (install step)
install(
	TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)


install (DIRECTORY include/AdsLib/
	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
	FILES_MATCHING PATTERN "*.h"
)

