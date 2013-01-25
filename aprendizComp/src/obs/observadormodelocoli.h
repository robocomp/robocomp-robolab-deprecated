// -*-c++-*-

#ifndef OBSV_MODELO_COLIS_H
#define OBSV_MODELO_COLIS_H

#include <obs/Observador.h>
#include <tcp/angulos.h>
#include <vector>
#include <iostream>
using namespace std;

namespace osgDTE {
  
  /// Observa como se mueve el modelo y simplemente cambia el color
  /// de las articulaciones que chocan
  class observadormodelocoli : public Observador
    {
    public:

      /// Variable de clase que indica si actualmente se tiene o no colisión
      bool no_hay_colision;
      /// Si hay colision, aquí se almacena el par colisionado
      int par_colisionado;
      /// Lista de links que se han de comprobar
      std::vector<string> _links_sospechosos;

      /// Lista de links que están produciendo colisión
      std::vector<int> _pares_colisionados;
    
      /// Constructor no hace nada. 
      observadormodelocoli() {no_hay_colision = true; par_colisionado = -1;}

      /// Función que llama el observado, o sea, el modelo, cada vez que este cambia.
      bool updateFrom(Observado* obs);

      /// Destructor no hace nada
      ~observadormodelocoli() {}

      /// Funciones para añadir y quitar links de la lista de comprobación
      void incluirLink(string j);
      void quitarLink(string j);
      void limpiarLinks();

    private:
      /// Calcula si hay colisiones entre el par numero i
      bool haycolision(unsigned int i, Observado* obs);

    };
}

#endif
