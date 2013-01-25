cmake_minimum_required(VERSION 2.6)
PROJECT( libmodelo-osg )

INCLUDE_DIRECTORIES(${APRENDIZ})

SET (LIBRARY_OUTPUT_PATH ${APRENDIZ}/lib)

SET ( SRC  modelo.cpp modeloBasicoPersona.cpp modeloHOAP.cpp modeloVISOR.cpp modeloNOMAD.cpp modeloVISORpalotes.cpp modeloHOAPpalotes.cpp modeloNOMADpalotes.cpp)

# ADD library
ADD_LIBRARY ( modelo-osg STATIC ${SRC}  )



