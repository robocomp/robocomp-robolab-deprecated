cmake_minimum_required(VERSION 2.6)
PROJECT( libtcp )

INCLUDE_DIRECTORIES(${APRENDIZ})

SET (LIBRARY_OUTPUT_PATH ${APRENDIZ}/lib)

# Headers set
SET ( HEADERS angulos.h Socket_Cliente.h Socket_Servidor.h Socket.h

)

SET ( SRC  Socket_Servidor.c Socket_Cliente.c angulos.c Socket.c )

# ADD library
ADD_LIBRARY ( tcp STATIC ${HEADERS} ${SRC}  )



