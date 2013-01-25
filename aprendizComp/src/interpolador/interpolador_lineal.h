// -*-c++-*-

#ifndef LIBINTERP_H
#define LIBINTERP_H

#include <osg/MatrixTransform>
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::posix_time;
using namespace boost::gregorian;

class Interpolador
{
 public:
  
  // Constructor 
  Interpolador() 
    {
      // Tiempo inicial
      time_llegada = time_actual = time_imenosuno = microsec_clock::local_time();
    }

  // Destructor no hace nada
  ~Interpolador() {}

  // Tiempos en las posiciones (i) e (i-1)
  ptime time_imenosuno;
  ptime time_i;
  ptime time_actual;
  ptime time_llegada;
  time_duration intervalo;

  // Posiciones en los instantes (i) e (i-1) (en CENTIMETROS!!!)
  osg::Vec3d pos_imenosuno;
  osg::Vec3d pos_i;
  osg::Vec3d pos_actual;

  // Velocidad actual del objeto (en m/sg )
  double velocidad;

  // 'Reseteo' del interpolador
  void iniciaInterpolador(osg::Vec3d posini)
  {
    time_llegada = time_actual = time_imenosuno = microsec_clock::local_time();
    pos_actual = pos_imenosuno = posini;
    alcanzada = true;
  }
  
  // Entrada de una nueva posición en el interpolador
  void updateInterpolador(osg::Vec3d posicion);

  // Función que devuelve la posición interpolada
  osg::Vec3d posicionInterpolada();

private:
  // Flag para indicar que se ha alcanzado la posicion deseada
  bool alcanzada;
  
};

#endif
