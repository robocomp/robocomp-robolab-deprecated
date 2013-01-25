cmake_minimum_required(VERSION 2.6)
PROJECT( sevillaComp )

# Sources set
SET ( SOURCES
  sevillaComp.cpp
  sevillaI.cpp

  $ENV{ROBOCOMP}/Classes/qvisor/qvisor.cpp  
  $ENV{ROBOCOMP}/Classes/qworld/qworld.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/osgview.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/findnamednode.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/getworldcoorofnode.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/viewerqt.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/adapterwidget.cpp
  
  $ENV{ROBOCOMP}/Classes/innermodel/robotinnermodel.cpp
  $ENV{ROBOCOMP}/Classes/innermodel/innermodel.cpp
  $ENV{ROBOCOMP}/Classes/innermodel/innermodelreader.cpp

  juego1.cpp
  juego2.cpp
  cameraDlgControl.cpp
  osgArt.cpp
  worker.cpp
)

# Headers set
SET ( HEADERS
  sevillaI.h
  $ENV{ROBOCOMP}/Classes/qvisor/qvisor.h  
  $ENV{ROBOCOMP}/Classes/qworld/qworld.h  
  $ENV{ROBOCOMP}/Classes/osgviewer/osgview.h
  $ENV{ROBOCOMP}/Classes/osgviewer/findnamednode.h
  $ENV{ROBOCOMP}/Classes/osgviewer/viewerqt.h 
  $ENV{ROBOCOMP}/Classes/osgviewer/adapterwidget.h 
  
  juego.h	
  juego1.h
  juego2.h
  osgArt.h
  cameraDlgControl.h
  worker.h
)

# RoboComp
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
INCLUDE( $ENV{ROBOCOMP}/CMake/qmat.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE(  Sevilla  DifferentialRobot  Speech CamMotion JointMotor CommonHead Laser Camera Armrocio)#qmat

SET (UIS sevillaDlg.ui proyectorDlg.ui)

#IPP
INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )

#Open Scene Graph
INCLUDE( $ENV{ROBOCOMP}/CMake/opensg.cmake )

# FrameWave
# INCLUDE( $ENV{ROBOCOMP}/CMake/framewave.cmake )

# Qt4
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
SET( QT_USE_QTXML TRUE )
SET( QT_USE_QTPHONON TRUE )

INCLUDE( ${QT_USE_FILE} )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )
QT4_WRAP_UI( UI_HEADERS ${UIS} )


# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( sevillaComp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( sevillaComp ${QT_LIBRARIES} ${LIBS}  osgART -lphonon)
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/sevillaComp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
