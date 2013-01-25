cmake_minimum_required(VERSION 2.6)
PROJECT( sickldcomp )

INCLUDE ( CMakeListsSpecific.txt)

# Sources set
SET ( SOURCES
  ${SOURCES}
  sickldcomp.cpp
  laserI.cpp

  genericmonitor.cpp
  commonbehaviorI.cpp
  genericworker.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/qlog/qlog.cpp
)

# Headers set
SET ( HEADERS
  ${HEADERS}
  laserI.h

  genericmonitor.h
  commonbehaviorI.h
  genericworker.h
)
#ROBOCOMP
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( CommonBehavior  Laser  DifferentialRobot)

# sick
INCLUDE ($ENV{ROBOCOMP}/CMake/sick.cmake )
# Qt4
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
SET( QT_USE_QTXML TRUE )
SET( QT_USE_QTSTATE TRUE )
SET( QT_USE_QTSTATEMACHINE TRUE )
INCLUDE( ${QT_USE_FILE} )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )


# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( sickldcomp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( sickldcomp ${QT_LIBRARIES} ${LIBS} ${STATIC_LIBS})
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/sickldcomp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
