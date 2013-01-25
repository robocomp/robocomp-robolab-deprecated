cmake_minimum_required(VERSION 2.6)
PROJECT( baseComp )

# Sources set
SET ( SOURCES
  ./baseComp.cpp
  ./baseI.cpp
  ./basehandler.cpp
  ./odometer.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/q4serialport/q4serialport.cpp
  $ENV{ROBOCOMP}/Classes/qmatriz/qmat.cpp
  $ENV{ROBOCOMP}/Classes/qmatriz/qmovingrobot.cpp
  $ENV{ROBOCOMP}/Classes/qmatriz/qvec.cpp
  $ENV{ROBOCOMP}/Classes/qmatriz/qmatrot.cpp
)

# Headers set
SET ( HEADERS
  ./baseI.h
  ./basehandler.h
  ./odometer.h
  $ENV{ROBOCOMP}/Classes/q4serialport/q4serialport.h
)

# RoboComp
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( Base )

# Qt4
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )

# IPP
INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )

# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( baseComp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( baseComp ${QT_LIBRARIES} ${LIBS})
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/baseComp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
