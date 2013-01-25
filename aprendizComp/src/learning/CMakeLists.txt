cmake_minimum_required(VERSION 2.6)
PROJECT( liblearning )

INCLUDE_DIRECTORIES(${APRENDIZ})

SET (LIBRARY_OUTPUT_PATH ${APRENDIZ}/lib)

SET ( SRC  libceldas.cpp learning.cpp curvaturaLEARNER.cpp learningCODA.cpp)

# ADD library
ADD_LIBRARY ( learning STATIC ${SRC}  )



