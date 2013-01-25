cmake_minimum_required(VERSION 2.6)
PROJECT( moleComp )

INCLUDE_DIRECTORIES (${QT_INCLUDES} ../ARToolKit/include/ /usr/local/include/osg /usr/include/osgViewer ${PROJECT_SOURCE_DIR}/tinyxml/)
LINK_DIRECTORIES (../ARToolKit/lib/)


# Sources set
SET ( SOURCES
  moleComp.cpp
  moleI.cpp

  camara.cpp
  armarker.cpp
  osgArt.cpp
  monitor.cpp
  commonbehaviorI.cpp
  worker.cpp
  
  $ENV{ROBOCOMP}/Classes/rcdraw/rcdraw.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/osgview.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/findnamednode.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/getworldcoorofnode.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/viewerqt.cpp
  $ENV{ROBOCOMP}/Classes/osgviewer/adapterwidget.cpp
  
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/qlog/qlog.cpp
  
  $ENV{ROBOCOMP}/Classes/innermodel/robotinnermodel.cpp
  $ENV{ROBOCOMP}/Classes/innermodel/innermodel.cpp
  $ENV{ROBOCOMP}/Classes/innermodel/innermodelreader.cpp
)

# Headers set
SET ( HEADERS
  moleI.h

  osgArt.h
  monitor.h
  commonbehaviorI.h
  worker.h
  
  $ENV{ROBOCOMP}/Classes/rcdraw/rcdraw.h    
  $ENV{ROBOCOMP}/Classes/osgviewer/osgview.h
  $ENV{ROBOCOMP}/Classes/osgviewer/findnamednode.h
  $ENV{ROBOCOMP}/Classes/osgviewer/viewerqt.h 
  $ENV{ROBOCOMP}/Classes/osgviewer/adapterwidget.h 
  $ENV{ROBOCOMP}/Classes/qlog/qlog.h

#   cv.h
#   cvaux.h
#   cvconfig.h
#   cvwimage.h
  
)

# RoboComp
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake ../ARToolKit/include)
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( CommonBehavior  Mole  Camera CommonHead JointMotor DifferentialRobot  Kinect Laser CamMotion  )
ROBOCOMP_LIBRARY( qmat ) 

# IPP
INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )
#Open Scene Graph
INCLUDE( $ENV{ROBOCOMP}/CMake/opensg.cmake )

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
QT4_WRAP_UI( UI_HEADERS guiDlg.ui )


# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( moleComp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( moleComp ${QT_LIBRARIES} ${LIBS} ${OpenCV_LIBS} osgART -losgART -lARgsub -lARvideo -lAR -lfltk -lglut -lX11 -lm)
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/moleComp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
