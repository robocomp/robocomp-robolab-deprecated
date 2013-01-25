cmake_minimum_required(VERSION 2.6)
PROJECT( libtracker )

INCLUDE_DIRECTORIES(${APRENDIZ})

SET (LIBRARY_OUTPUT_PATH ${APRENDIZ}/lib)

SET ( SRC  Clasificacion.cpp Config.cpp Funcpir.cpp FuncTracking.cpp IORaw.cpp Silueta.cpp BIP.cpp)

# ADD library
ADD_LIBRARY ( tracker STATIC ${SRC}  )



