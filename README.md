# BeckhoffADSROS
ROS node adaptation of the Beckhoff/ADS library

This library is intended to provide easy use as ADS client applications running on non-windows systems (e.g. FreeBSD, Linux, ...) to communicate with TwinCAT devices via TCP/IP. This is a modification of the Beckhoff/ADS library so that it can be used with ROS (https://github.com/Beckhoff/ADS.git). Modifications made are found at the bottom of the READ ME.

To build this library a recent compiler with C++14 support is required. 

Currently (2022-02-21) tested with:
===================================

host (amd64)     | target| compiler
-----------------|-------|-------------
TC/BSD 12        | amd64 | clang 10.0.1
Debian Bullseye  | amd64 | clang 11.0.1
Debian Bullseye  | amd64 | gcc 10.2.1
Debian Bullseye  | i686  | gcc 10.2.1
Debian Bullseye  | mips  | gcc 10.2.1
Ubuntu 18.04 LTS | win32 | gcc 5.5.0
Windows 10       | win64 | gcc 8.3.0


Prepare your target to run the example:
======================================
- Download your PLC project to your target e.g. "PLC-TestProject" of our GitHub repository.
- Authorise your ADS client for the TwinCAT target by adding an AMS route.

Sample AMS route:
  Name:           MyAdsClient     
  AMS Net Id:     192.168.0.1.1.1 # Derived from the IP address of your ADS client
  Address:        192.168.0.1     # Use the IP which is connected to your client
  Transport Type: TCP/IP
  Remote Route:   None / Server
  Unidirectional: false
  Secure ADS:     false
  
Routes can be configured by several possibilities:
TwinCAT Engineering:  Go to the tree item SYSTEM/Routes and add a static route.
TwinCAT Systray:      Open the context menue by right click the TwinCAT systray icon. (not available on Windows CE devices) 
  TC2*: Go to Properties/AMS Router/Remote Computers
  TC3:  Go to  Router/Edit routes.
TcAmsRemoteMgr: Windows CE devices can be configured locally (TC2*/TC3). Tool location: /Hard Disk/System/TcAmsRemoteMgr.exe
IPC Diagnose: Beckhoff IPC’s provide a web interface for diagnose and configuration. Further information: http://infosys.beckhoff.de/content/1033/devicemanager/index.html?id=286

*Requires a TwinCAT restart.

- set your target to a static ip address with something like:
192.168.0.2 subnet mask: 255.255.255.0
- check using ping

- download the PLC-TestProject file from the https://github.com/Beckhoff/ADS and run the NotificationTest project

Prepare your client to run the ads library
======================================
- set your client to a static ip address with something like:
192.168.0.1 subnet mask: 255.255.255.0

- make a catkin directory (http://wiki.ros.org/catkin/Tutorials/create_a_workspace)

- git clone the ads repo into the src folder of catkin workspace:
git clone https://github.com/Beckhoff/ADS.git <directory>

- get python 3 (https://docs.python-guide.org/starting/install3/linux/)

- navigate into the catkin directory

- build the ads package (the version of python is 3.10, can be different)
catkin build <package name> -DPYTHON_EXECUTABLE=/usr/bin/python3 
-DPYTHON_INCLUDE_DIR=/usr/local/include/python3.10

- If there is a weird error and catkin does not build try this:
catkin clean

- Once the package is built, source the files so that the packages in the workspace recognisable to the rest of the system
source ~/catkin_ws/devel/setup.bash

- get ros to run:
source /opt/ros/noetic/setup.bash
roscore

- find the ROS package, make sure that ROS replies witht the file path of the ads library:
rospack find ads

- to test this library build, modify and run (using rosrun) the example folder as a separate node (the example.cpp file adapted for ROS)
	- set "remoteNetId" and "remoteIpV4" and enable bhf::ads::SetLocalAddress() in "example/example.cpp" according to 	your own setup
	- rosrun example example


- more info on ADS/AMS Specification found here: https://infosys.beckhoff.com/content/1033/tc3_ads_intro/index.html

# List of modifications made:

- filepath:
AdsLib to include/AdsLib

- Package file:
added a package.xml file to allow catkin to build
	
- CMakeLists.txt file inside ads:
added after find_package(Threads):
Added to enable catkin to build this package (locate catkin step)
find_package(catkin REQUIRED)

#The catkin_package macro generates cmake config files for your package (config step)
catkin_package(
   INCLUDE_DIRS include
   LIBRARIES ${PROJECT_NAME}
   )
   
#set the include directory, needs to be called include (config step)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

replaced add_subdirectory commands to:
add_subdirectory(include/AdsLib)
add_subdirectory(AdsLibTest)
add_subdirectory(example)

added after the add_subdirectory commands:
#Catkin edits, need to install the target to Catkin as well as all of the header files inside include, special command is used to only detect the .h files (install step)
install(
	TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)


install (DIRECTORY include/AdsLib/
	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
	FILES_MATCHING PATTERN "*.h"
)

- CMakeLists.txt file inside ads/include/AdsLib:
added this for catkin so that catkin libraries are linked to the target (linking step)
target_link_libraries(ads PUBLIC ${catkin_LIBRARIES})
