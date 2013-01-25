#antes de compilar haz esto.
export CXXFLAGS="-fpermissive"

#para utilizar las librerias de osgart, simplemente, añade en el src/CMakeList.txt osgART. Te quedará algo así:
TARGET_LINK_LIBRARIES( sevillaComp ${QT_LIBRARIES} ${LIBS}  osgART -lphonon)


