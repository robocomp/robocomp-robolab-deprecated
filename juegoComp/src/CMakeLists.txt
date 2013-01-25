cmake_minimum_required(VERSION 2.6)
PROJECT( juegocomp )

INCLUDE ( CMakeListsSpecific.txt)

# Sources set
SET ( SOURCES
  ${SOURCES}
  juegocomp.cpp
  juegoI.cpp

  $ENV{ROBOCOMP}/Classes/rcdraw/rcdraw.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/osgview.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/findnamednode.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/getworldcoorofnode.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/viewerqt.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/adapterwidget.cpp
  $ENV{ROBOCOMP}/Classes/innermodel/innermodel.cpp
  $ENV{ROBOCOMP}/Classes/innermodel/innermodelreader.cpp
  genericmonitor.cpp
  commonbehaviorI.cpp
  genericworker.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/qlog/qlog.cpp
)

# Headers set
SET ( HEADERS
  ${HEADERS}
  juegoI.h

  $ENV{ROBOCOMP}/Classes/rcdraw/rcdraw.h
  $ENV{ROBOCOMP}/Classes/osgviewer/osgview.h
  $ENV{ROBOCOMP}/Classes/osgviewer/findnamednode.h
  $ENV{ROBOCOMP}/Classes/osgviewer/getworldcoorofnode.h
  $ENV{ROBOCOMP}/Classes/osgviewer/viewerqt.h
  $ENV{ROBOCOMP}/Classes/osgviewer/adapterwidget.h
  $ENV{ROBOCOMP}/Classes/innermodel/innermodel.h
  $ENV{ROBOCOMP}/Classes/innermodel/innermodelreader.h
  genericmonitor.h
  commonbehaviorI.h
  genericworker.h
)
#ROBOCOMP
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( CommonBehavior  Juego Trackeropenni EvaluadorOff Speech UrsusArm  JointMotor Speechgenerator)
ROBOCOMP_LIBRARY(qmat)

# IPP
INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )
# OPENSG
INCLUDE ($ENV{ROBOCOMP}/CMake/opensg.cmake )
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
QT4_WRAP_UI( UI_HEADERS guiDlg.ui )


# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( juegocomp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( juegocomp ${QT_LIBRARIES} ${LIBS}  -lphonon )
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/juegocomp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
