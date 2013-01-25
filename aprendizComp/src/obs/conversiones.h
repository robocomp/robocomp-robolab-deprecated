#ifndef CONVERSIONES_H
#define CONVERSIONES_H

#include <modelo-osg/modeloBasicoPersona.h>
#include <cmath>

#define ONDEBUG(x)

namespace osgDTE
{


  /** Clase que agrupa funciones que convierten configuraciones del
    * modelo de la persona a valores de los angulos de los motores del
    * HOAP.
    */
  class ModeloPersonaAHoap
    {

    public:

      /// Extraer valores de los angulos del brazo izquierdo.
      static void AngulosBrazoIzquierdo(const modelobasicopersona& m, std::vector<double> & larm_joint);

      /// Extraer valores de los angulos del brazo derecho.
      static void AngulosBrazoDerecho(const modelobasicopersona& m, std::vector<double> & rarm_joint);
      
    };
  
}

#endif
