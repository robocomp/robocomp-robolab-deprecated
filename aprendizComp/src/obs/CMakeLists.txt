cmake_minimum_required(VERSION 2.6)
PROJECT( libobs )

INCLUDE_DIRECTORIES(${APRENDIZ})

SET (LIBRARY_OUTPUT_PATH ${APRENDIZ}/lib)
# ADD library
ADD_LIBRARY ( obs STATIC Observado.cpp observadormodelocoli.cpp observadormodelolinux.cpp conversiones.cpp
)



