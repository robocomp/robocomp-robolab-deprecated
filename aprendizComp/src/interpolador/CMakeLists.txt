cmake_minimum_required(VERSION 2.6)
PROJECT( libinterpolador )

INCLUDE_DIRECTORIES(${APRENDIZ})

SET (LIBRARY_OUTPUT_PATH ${APRENDIZ}/lib)

SET ( SRC  interpolador.cpp)

# ADD library
ADD_LIBRARY ( interpol STATIC ${SRC}  )



