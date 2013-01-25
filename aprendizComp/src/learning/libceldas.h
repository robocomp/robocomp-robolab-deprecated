// -*- C++ -*-
#ifndef LIBCELDAS_H
#define LIBCELDAS_H

#include <iostream>


typedef unsigned char uchar;
typedef long int CELDA;

struct ESPACIO{
	int xmax;
	int delta_x;
	int ymax;
	int delta_y;
	int zmax;
	int delta_z;
};

struct COORDENADA{
  COORDENADA() 
    : x(0), y(0), z(0) {}
  COORDENADA(int _x, int _y, int _z) 
    : x(_x), y(_y), z(_z) {}
	int x;
	int y;
	int z;
};

class manejadorceldas 
{

 public:
  
  // Constructor 
  manejadorceldas();

  // Destructor no hace nada
  ~manejadorceldas() {}

  void cambiarDeltas(int x, int y, int z);
  void cambiarEspacio(int x, int y, int z);

  CELDA calculaCelda(COORDENADA) const;
  COORDENADA coordenadaCelda(CELDA) const;
  double distanciaEntreCoordenadasAlCuadrado(COORDENADA, COORDENADA) const;
  double distanciaEntreCoordenadas(COORDENADA, COORDENADA) const;
  double distanciaEntreCeldas(CELDA,CELDA) const;

  ESPACIO espacio;
  
};


inline std::ostream & operator<<(std::ostream & o, const manejadorceldas & m)
{
  o << m.espacio.xmax << " " << m.espacio.delta_x << " " 
    << m.espacio.ymax << " " << m.espacio.delta_y << " " 
    << m.espacio.zmax << " " << m.espacio.delta_z;
  return o;
} 

inline std::istream & operator >>(std::istream & i, manejadorceldas & m)
{
  i >> m.espacio.xmax >> m.espacio.delta_x >> m.espacio.ymax >> m.espacio.delta_y >> m.espacio.zmax >> m.espacio.delta_z;
  return i;
}

#endif
