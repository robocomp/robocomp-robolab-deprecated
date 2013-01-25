// -*-c++-*-

#ifndef LIBINTERP_H
#define LIBINTERP_H

#include <osg/MatrixTransform>
#include <osg/Matrix>
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::posix_time;
using namespace boost::gregorian;
  


class Interpolador
{
 public:
 int i;
 bool listo;
 osg::Vec3d pos;
 bool primera;
 bool alcanzada;
 double ta,tf,tac,intermedio;
 // Estructura auxiliar para poder tener en una sola variable las dos componentes
  struct Punto
  {
       osg::Vec3d pos;
       double t;
  };
  ptime tii,ti;
  ptime ti_abs,tac_abs;// hay que tener en cuenta que mis tiempos son relativos y empiezan desde 0 y debo seguir asi
  //por esto tengo que calcularlos con los tiempos del sistema pero mantiniendo mi estilo relativo
  ptime tri,trii;
  typedef Punto TPunto[5];// grupo de 4 puntos a interpolar
  typedef float TVector4[5];
  typedef float TVector3[4];
  typedef float TCoeficientes[5][5]; // Con esta variable guardare los coeficientes para montar la función,a b c d
  
  TPunto grupeto;
  // Constructor 
  Interpolador() 
    {
      // Tiempo inicial
      time_llegada = time_actual = time_imenosuno = time_i = tii = ti = ti_abs = tac_abs = microsec_clock::local_time();
      primera = true;
      alcanzada = listo = false;
      n_puntos = 0;
      ta=tf=tac=intermedio=eje_t=0.0;
      i=0;
      pos=osg::Vec3(5.0,1.0,5.0);// posicion para que no salgan raros los brazos
    }

  // Destructor no hace nada
  ~Interpolador() {}

  // Tiempos en las posiciones (i) e (i-1)
  ptime time_imenosuno;
  ptime time_i;
  ptime time_actual;
  ptime time_llegada;
  
  time_duration intervalo,incremento;
  // Esto es para crear el eje t de tiempo
  
  // Posiciones en los instantes (i) e (i-1) (en CENTIMETROS!!!)
  osg::Vec3d pos_imenosuno;
  osg::Vec3d pos_i;
  osg::Vec3d pos_actual;

// --------------------------------------------
//        Potencia de un número
// --------------------------------------------
float potencia(float n,int x);

  // 'Reseteo' del interpolador
  void iniciaInterpolador(osg::Vec3d posini)
  {
    time_llegada = time_actual = time_imenosuno = microsec_clock::local_time();
    pos_actual = pos_imenosuno = posini;

  }
  
  void cspline(TPunto &grupeto);
  // Entrada de una nueva posición en el interpolador 
  void updateInterpolador(osg::Vec3d posicion);

  // Función que devuelve la posición interpolada
  osg::Vec3d posicionInterpolada();

private:
  // Flag para indicar que se ha alcanzado la posicion deseada
 

  int n_puntos;// numero de puntos
  float aex,aey,aez;
  double eje_t;
  TCoeficientes cdx,cdy,cdz;  
};

#endif
