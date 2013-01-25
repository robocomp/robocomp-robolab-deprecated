cmake_minimum_required(VERSION 2.6)
PROJECT( cenitalComp )

# Sources set
SET ( SOURCES
  cenitalComp.cpp
  cenitalI.cpp
  worker.cpp
  $ENV{ROBOCOMP}/Classes/qvisor/qvisor.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/qworld/qworld.cpp
  $ENV{ROBOCOMP}/Classes/innermodel/robotinnermodel.cpp
  $ENV{ROBOCOMP}/Classes/innermodel/innermodel.cpp
  $ENV{ROBOCOMP}/Classes/innermodel/innermodelreader.cpp
)

# Headers set
SET ( HEADERS
  cenitalI.h
  worker.h
  $ENV{ROBOCOMP}/Classes/qvisor/qvisor.h
  $ENV{ROBOCOMP}/Classes/qworld/qworld.h
)

# RoboComp
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP} )
ROBOCOMP_WRAP_ICE( Cenital Camera DifferentialRobot CamMotion Vision Laser CommonHead JointMotor)
ROBOCOMP_LIBRARY( qmat )

# IPP
INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )

# QtStateMachine
#INCLUDE( $ENV{ROBOCOMP}/CMake/qtsmachine.cmake )

# Qt4
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
SET( QT_USE_QTXML TRUE )
INCLUDE( ${QT_USE_FILE} )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )
QT4_WRAP_UI( UI_HEADERS cenitalDlg.ui )


# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( cenitalComp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( cenitalComp ${QT_LIBRARIES} ${LIBS})
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/cenitalComp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
