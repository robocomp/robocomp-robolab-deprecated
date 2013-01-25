// -*-c++-*-

#ifndef OBSERVADOR_H
#define OBSERVADOR_H

#include <obs/Observado.h>

class Observador
{
 public:
  
  // Las clases que implementen este observador
  // deben devolver false si quieren que se interrumpa la cadena
  // de "updateFroms" de los siguientes observadores, 
  // y true en caso contrario.
  virtual bool updateFrom(Observado* obs) = 0;

  // Destructor virtual
  virtual ~Observador() {}
};

#endif
