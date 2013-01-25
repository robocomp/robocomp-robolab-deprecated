cmake_minimum_required(VERSION 2.6)
PROJECT( dialogmanagercomp )

INCLUDE ( CMakeListsSpecific.txt)

# Sources set
SET ( SOURCES
  ${SOURCES}
  dialogmanagercomp.cpp
  dialogmanagerI.cpp

  $ENV{ROBOCOMP}/Classes/qstatemachinewidget/qstateMachineWrapper.cpp
  $ENV{ROBOCOMP}/Classes/qstatemachinewidget/graphwidget.cpp
  $ENV{ROBOCOMP}/Classes/qstatemachinewidget/node.cpp
  $ENV{ROBOCOMP}/Classes/qstatemachinewidget/edge.cpp
  genericmonitor.cpp
  commonbehaviorI.cpp
  genericworker.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/qlog/qlog.cpp
)

# Headers set
SET ( HEADERS
  ${HEADERS}
  dialogmanagerI.h

  $ENV{ROBOCOMP}/Classes/qstatemachinewidget/qstateMachineWrapper.h
  $ENV{ROBOCOMP}/Classes/qstatemachinewidget/graphwidget.h
  $ENV{ROBOCOMP}/Classes/qstatemachinewidget/node.h
  $ENV{ROBOCOMP}/Classes/qstatemachinewidget/edge.h
  genericmonitor.h
  commonbehaviorI.h
  genericworker.h
)
#ROBOCOMP
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( CommonBehavior  DialogManager Speech Tracker Vision CommonHead DifferentialRobot Gesture ASR FaceDetector  JointMotor)

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
ADD_EXECUTABLE( dialogmanagercomp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( dialogmanagercomp ${QT_LIBRARIES} ${LIBS})
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/dialogmanagercomp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
