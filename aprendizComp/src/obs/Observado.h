//-------------------------------------------------------------------
//
// Modelo observado para implementar modelo - vista
// (Subject-Observer en "Desingn Patterns").
//
//-------------------------------------------------------------------
#ifndef OBSERVADO_H
#define OBSERVADO_H


#include <vector>

// TODO: Hacerlo con punteros listillos

class Observador;

class Observado
{
 public:

  /// Registrar los observadores que quieren ser notificados de cambios.
  void registrar(Observador* obsr)
    { if (obsr) observadores.push_back(obsr); }

  // El objeto observado ha cambiado y se lo comunica a sus observadores.
  void changed();

  virtual ~Observado(){}

 protected:

  /// Lista de observadores
  std::vector<Observador*> observadores;
};


#endif
