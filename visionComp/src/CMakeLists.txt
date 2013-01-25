cmake_minimum_required(VERSION 2.6)
PROJECT( visionComp )

# Sources set
SET ( SOURCES
  visionComp.cpp
  visionI.cpp
  roidetector.cpp
  worker.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/logpolar/lpolar.cpp
)

# Headers set
SET ( HEADERS
  visionI.h
  roidetector.h
  worker.h
)

# RoboComp
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( Vision DifferentialRobot CamMotion Camera )

# Qt4
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
SET( QT_USE_QTXML TRUE)
INCLUDE( ${QT_USE_FILE} )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )

# IPP
INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )

# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( visionComp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( visionComp ${QT_LIBRARIES} ${LIBS})
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/visionComp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
