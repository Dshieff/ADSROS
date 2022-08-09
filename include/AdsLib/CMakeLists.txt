set(SOURCES
  AdsDef.cpp
  AdsDevice.cpp
  AdsFile.cpp
  AdsLib.cpp
  Frame.cpp
  LicenseAccess.cpp
  Log.cpp
  RouterAccess.cpp
  RTimeAccess.cpp
  Sockets.cpp

  standalone/AdsLib.cpp
  standalone/AmsConnection.cpp
  standalone/AmsNetId.cpp
  standalone/AmsPort.cpp
  standalone/AmsRouter.cpp
  standalone/NotificationDispatcher.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(ADSROS PUBLIC .)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_link_libraries(ADSROS PUBLIC wsock32)
endif()


if(WIN32 EQUAL 1)
    target_link_libraries(ADSROS PUBLIC ws2_32)
endif()

target_link_libraries(ADSROS PUBLIC Threads::Threads)

# added this for catkin so that catkin libraries are linked to the target (linking step)
target_link_libraries(ADSROS PUBLIC ${catkin_LIBRARIES})
