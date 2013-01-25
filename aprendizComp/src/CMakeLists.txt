cmake_minimum_required(VERSION 2.6)
PROJECT( aprendizComp )

set (APRENDIZ   ${CMAKE_CURRENT_SOURCE_DIR}   )
INCLUDE_DIRECTORIES(${APRENDIZ} )
SUBDIRS( obs tcp modelo-osg tracker rapid learning interpolador retargeting)

# Sources set
SET ( SOURCES
  aprendizComp.cpp
  aprendizI.cpp

  monitor.cpp
  commonbehaviorI.cpp
  worker.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/qlog/qlog.cpp
)

# Headers set
SET ( HEADERS
  aprendizI.h

  monitor.h
  commonbehaviorI.h
  worker.h
)

# RoboComp
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( CommonBehavior  Aprendiz )

#open scene graph
INCLUDE_DIRECTORIES( /usr/local/include/osg /usr/include/osgViewer )
SET( LIBS ${LIBS} -L/usr/local/lib64/ -losg -losgViewer )

# IPP
INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )
# FrameWave
# INCLUDE( $ENV{ROBOCOMP}/CMake/framewave.cmake )

# Qt4
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
FIND_PACKAGE( Qt4 REQUIRED )
#opencv
FIND_PACKAGE( OpenCV REQUIRED )
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
SET( QT_USE_QTXML TRUE )
INCLUDE( ${QT_USE_FILE} )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )

#set library
SET(APRENDIZ_LIB libinterpol.a  liblearning.a  libmodelo-osg.a  libobs.a  librapid.a libRetar.a  libtcp.a  libtracker.a)


# Specify construction and link process
SET (EXECUTABLE_OUTPUT_PATH ../bin)
ADD_EXECUTABLE( aprendizComp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( aprendizComp ${QT_LIBRARIES} ${LIBS} ${OpenCV_LIBS} -L${APRENDIZ}/lib  ${APRENDIZ_LIB})
INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/aprendizComp DESTINATION /opt/robocomp/bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
