#include "interpolador.h"
#include <iostream>

#define DEBUGO 0

using namespace std;

//Calculo de la distancia euclidea entre x,y,x y xant, yant, zant
double DistanciaPosic(osg::Vec3d a, osg::Vec3d b)
{
  osg::Vec3d res;

  res = a - b;
  return ( res.length() );
}

// Entrada de una nueva posición en el interpolador
void Interpolador::updateInterpolador(osg::Vec3d posicion)
{
  double dE;

  // actualización del tiempo
  time_imenosuno = time_i;
  time_i = microsec_clock::local_time();
  intervalo = time_i - time_imenosuno;

  if (DEBUGO) cout << "Intervalo en segs: " << ((double)(intervalo.total_microseconds())/1000000) << endl;

  // actualización de la posicion deseada
  pos_imenosuno = pos_actual;
  pos_i = posicion;
  dE = DistanciaPosic(pos_i, pos_actual);

  if (DEBUGO) cout << "Distancia entre posiciones (cm): " << dE << endl;

  // Cálculo de la velocidad de desplazamiento
  velocidad = (10000)*(dE / ((double)(intervalo.total_microseconds()))) ;

  if (DEBUGO) cout << "Velocidad: " << velocidad << endl;

  // Cálculo del tiempo de llegada a la posición deseada
  time_llegada = time_i + microseconds( (long)(10000*(dE / velocidad)) );

  if (DEBUGO) cout << "Tiempo de llegada a la pos. deseada (segs): " << to_simple_string(time_llegada - time_i) << endl;

  // Por último, se desactiva el flag 'alcanzada' hasta que se alcance la posición
  alcanzada = false;

}

// Función que devuelve la posición interpolada,
// actualizándola además en la variable pos_actual
osg::Vec3d Interpolador::posicionInterpolada()
{
  osg::Vec3d nueva_pos;
  
  // Captura del tiempo actual
  time_actual = microsec_clock::local_time();

  // comprobación de que se ha alcanzado la posicion deseada (por tiempo transcurrido)
  if ( (!alcanzada)&&(time_actual >= time_llegada) )
    {
      if (DEBUGO) cout << "Pos.deseada: X: " << pos_i.x() << " Y: " << pos_i.y() << " Z: " << pos_i.z() << endl;

      alcanzada = true;
      // se ajusta la posición a la deseada... de cualquier modo, debería estar muy cerca
      nueva_pos = pos_i;
    }
  else
    {
      // si aún no se ha alcanzado la posición y no ha pasado el tiempo sufic, se 
      // devuelve la posición actualizada
      if (!alcanzada) 
	{
	  osg::Vec3d incremento_pos;
	  time_duration incremento_time = time_actual - time_i;

	  incremento_pos = pos_i - pos_imenosuno; 
	  incremento_pos.normalize();             // vector unitario apuntando en la direcc. del mov.
	  incremento_pos *= velocidad;            // vector velocidad
	  incremento_pos *= ((double)(incremento_time.total_microseconds())/1000000); // v*t = s
	  incremento_pos *= 100;                  // en centímetros!  

	  nueva_pos = pos_imenosuno + incremento_pos; // en centímetros!

	  if (DEBUGO) cout << "Pos. actual: X: " << pos_actual.x() << " Y: " << pos_actual.y() << " Z: " << pos_actual.z() << endl;
	}
      else
	{
	  nueva_pos = pos_i; // Si se había alcanzado la posición, se devuelve pos_i
	}
    }

  pos_actual = nueva_pos;
  return nueva_pos;
}
