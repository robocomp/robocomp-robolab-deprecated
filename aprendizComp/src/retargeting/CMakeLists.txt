cmake_minimum_required(VERSION 2.6)
PROJECT( libRetar )

INCLUDE_DIRECTORIES(${APRENDIZ})

SET (LIBRARY_OUTPUT_PATH ${APRENDIZ}/lib)

SET ( SRC  retargeting.cpp)

# ADD library
ADD_LIBRARY ( Retar STATIC ${SRC}  )



