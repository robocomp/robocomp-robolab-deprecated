cmake_minimum_required(VERSION 2.6)
PROJECT( evaluacionkinectComp )

# Sources set
SET ( SOURCES
  evaluacionkinectComp.cpp
  evaluacionkinectI.cpp

  monitor.cpp
  commonbehaviorI.cpp
  worker.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/qlog/qlog.cpp
  $ENV{ROBOCOMP}/Classes/rcdraw/rcdraw.cpp
)

# Headers set
SET ( HEADERS
  evaluacionkinectI.h

  monitor.h
  commonbehaviorI.h
  worker.h
  $ENV{ROBOCOMP}/Classes/rcdraw/rcdraw.h    
)

# RoboComp
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( CommonBehavior  Evaluacionkinect  Kinect DifferentialRobot CommonHead JointMotor )
ROBOCOMP_LIBRARY( qmat )
SET (UIS evaluacionDlg.ui)

# IPP
INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )

# FrameWave
# INCLUDE( $ENV{ROBOCOMP}/CMake/framewave.cmake )

# Qt4
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
SET( QT_USE_QTXML TRUE )
INCLUDE( ${QT_USE_FILE} )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )
QT4_WRAP_UI( UI_HEADERS ${UIS} )


# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( evaluacionkinectComp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( evaluacionkinectComp ${QT_LIBRARIES} ${LIBS})
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/evaluacionkinectComp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
