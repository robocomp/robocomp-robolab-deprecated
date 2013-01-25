cmake_minimum_required(VERSION 2.6)
PROJECT( leandroComp )

# Sources set
SET ( SOURCES
  leandroComp.cpp
  leandroI.cpp

  monitor.cpp
  commonbehaviorI.cpp
  worker.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/qlog/qlog.cpp
  $ENV{ROBOCOMP}/Classes/rcdraw/rcdraw.cpp#MODIFICADO LANDER PARA DIBUJAR LA UI EN FRAME (VIDEO)
)

# Headers set
SET ( HEADERS
  leandroI.h
  monitor.h
  commonbehaviorI.h
  worker.h
  $ENV{ROBOCOMP}/Classes/rcdraw/rcdraw.h  #MODIFICADO LANDER PARA DIBUJAR LA UI EN FRAME (VIDEO)
)

# RoboComp
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( CommonBehavior  Leandro  Camera CommonHead JointMotor DifferentialRobot  Kinect DifferentialRobot CommonHead JointMotor )
ROBOCOMP_LIBRARY( qmat ) #MODIFICADO LANDER PARA DIBUJAR LA UI EN FRAME (VIDEO)

# Graphical User Interfaces
SET (UIS leandroDlg.ui) #MODIFICADO LANDER PARA DIBUJAR LA UI

# IPP
 INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )#MODIFICADO LANDER PARA DIBUJAR LA UI EN FRAME (VIDEO)
# FrameWave
# INCLUDE( $ENV{ROBOCOMP}/CMake/framewave.cmake )

# Qt4
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
FIND_PACKAGE( Qt4 REQUIRED )
FIND_PACKAGE( OpenCV REQUIRED )
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
SET( QT_USE_QTXML TRUE )
INCLUDE( ${QT_USE_FILE} )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )
QT4_WRAP_UI( UI_HEADERS ${UIS} )#MODIFICADO LANDER PARA DIBUJAR LA UI


# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( leandroComp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( leandroComp ${QT_LIBRARIES} ${LIBS} ${OpenCV_LIBS})
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/leandroComp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
