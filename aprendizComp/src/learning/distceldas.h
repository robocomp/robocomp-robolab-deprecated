// -*- C++ -*-
#ifndef DISTCELDAS_H
#define DISTCELDAS_H

#include "utils.h"
#include "libceldas.h"
#include <vector>

// Clase auxiliar para guardar agrupaciones de celdas.
template<typename T, int tamanio=2> class Tuple {
public:
  inline int size() { return size; }
  inline T& operator[] (int i) { return v[i]; }
  inline const T& operator[] (int i) const { return v[i]; }
  T v[tamanio];
};


// Clase auxiliar para calcular distancia entre celdas.
class DistanciaCeldas
{
  // Referencia al manejador de celdas
  const manejadorceldas& _m;

public:

  // Al constructor hay que pasarle el manejador de celdas.
  DistanciaCeldas(const manejadorceldas& m) : _m(m) {}


  // Con este operador se calcula la distancia.
  inline double operator() ( const CELDA& celda1, 
			     const CELDA& celda2 )
  {
    return _m.distanciaEntreCeldas(celda1, celda2);
  }

};


// Clase auxiliar para calcular distancia entre pares de celdas.
class DistanciaParCeldas
{
  // Referencia al manejador de celdas
  const manejadorceldas& _m;
public:
  // Al constructor hay que pasarle el manejador de celdas.
  DistanciaParCeldas(const manejadorceldas& m) : _m(m) {}

  // Con este operador se calcula la distancia.
  inline double operator() ( const  Tuple<CELDA> & par1,
			     const  Tuple<CELDA> & par2)
  {
    
    return (_m.distanciaEntreCeldas(par1[0], par2[0]) +
	    _m.distanciaEntreCeldas(par1[1], par2[1]));
  }
};

// Clase auxiliar para calcular distancia entre celdas izquierdas.
class DistanciaCeldasIzquierdas
{
  // Referencia al manejador de celdas
  const manejadorceldas& _m;
public:
  // Al constructor hay que pasarle el manejador de celdas.
  DistanciaCeldasIzquierdas(const manejadorceldas& m) : _m(m) {}

  // Con este operador se calcula la distancia.
  inline double operator() ( const  Tuple<CELDA> & par1,
			     const  Tuple<CELDA> & par2)
  {
    
    return (_m.distanciaEntreCeldas(par1[0], par2[0]));
  }
};

// Clase auxiliar para calcular distancia entre celdas derechas
class DistanciaCeldasDerechas
{
  // Referencia al manejador de celdas
  const manejadorceldas& _m;
public:
  // Al constructor hay que pasarle el manejador de celdas.
  DistanciaCeldasDerechas(const manejadorceldas& m) : _m(m) {}

  // Con este operador se calcula la distancia.
  inline double operator() ( const  Tuple<CELDA> & par1,
			     const  Tuple<CELDA> & par2)
  {
    
    return (_m.distanciaEntreCeldas(par1[1], par2[1]));
  }
};

// Clase auxiliar para calcular distancias entre coordenadas X,Y,Z
class DistanciaCoordenadas
{
  // Referencia al manejador de celdas
  const manejadorceldas& _m;
public:
  // Al constructor hay que pasarle el manejador de celdas.
  DistanciaCoordenadas(const manejadorceldas& m) : _m(m) {}

  // Con este operador se calcula la distancia.
  inline double operator() ( const  COORDENADA & coord1,
			     const  COORDENADA & coord2)
  {
    
    return (_m.distanciaEntreCoordenadas(coord1, coord2));
  }
};

// Clase auxiliar para calcular distancias entre valores AAL
class DistanciaAAL
{
public:
  DistanciaAAL() {}

  // Con este operador se calcula la distancia.
  inline double operator() ( const  AAL & aal1,
			     const  AAL & aal2)
  {
    
    return ( fabs(aal1.angulo -  aal2.angulo) );
  }
};

// Calcula la distancia minima entre dos secuencias a base de deformar
// (warp) los ejes de tiempo de ambas.
//
// Ver artículo: Hiroaki Sakoe, Seibi Chiba; "Dynamic Programming Algorithm 
//               Optimization for Spoken Word Recognition". IEEE Transactions
//               on Acoustics 
// Objeto: Puede ser una CELDA, un par de CELDAS, etc.
// Distancia: Tipo de la clasecilla que se usa para obtener las distancias
// patron: lo que su nombre indica.
// secuencia: lo que su nombre indica.
// tipo: distancia simetrica o asimetrica.
// dist: Objeto que se usa para calcular distancias
// ventana: maxima deformacion que se permite tanto del eje de tiempos
//          del patron como del eje de tiempos de la secuencia a la hora
//          de compararlos.

template<
  typename Objeto,
  typename Distancia
  >
double DistanciaSecuencias(const std::vector<Objeto>& patron,
			   const std::vector<Objeto>& secuencia,
			   double peso_horizontal,
			   double peso_vertical,
			   double peso_diagonal,
			   Distancia& dist,
			   int ventana);

// Calcula la distancia minima entre dos secuencias
// utilizando el algoritmo EDR (Edit Distance on Real Sequences)
//
// Ver artículo: Lei Chen, Tamer Ozsu, Vincent Oria; "Robust and Fast 
//               Similarity Search for Moving Object Trajectories".
//
// Objeto: Puede ser una CELDA, un par de CELDAS, etc.
// Distancia: Tipo de la clasecilla que se usa para obtener las distancias
// patron: lo que su nombre indica.
// secuencia: lo que su nombre indica.
// umbral_matching: valor de distancia por debajo del cual se considera que hay matching
// dist: Objeto que se usa para calcular distancias
template<
  typename Objeto,
  typename Distancia
  >
double DistanciaSecuenciasEDR(const std::vector<Objeto>& patron,
			      const std::vector<Objeto>& secuencia,
			      double umbral_matching,
			      Distancia& distancia);

// Calcula la distancia minima entre dos secuencias
// utilizando el algoritmo ERP (Edit Distance with Real Penalty)
//
// Ver artículo: Lei Chen, Tamer Ozsu, Vincent Oria; "Robust and Fast 
//               Similarity Search for Moving Object Trajectories".
// Ver artículo: L. Chen, R. Ng; "On the marriage
//               of edit distance and Lp norms".
//
// Objeto: Puede ser una CELDA, un par de CELDAS, etc.
// Distancia: Tipo de la clasecilla que se usa para obtener las distancias
// patron: lo que su nombre indica.
// secuencia: lo que su nombre indica.
// hueco: Llamado 'g' en los artículos. La penalización por insertar o borrar un valor es su distancia al hueco.
// dist: Objeto que se usa para calcular distancias
template<
  typename Objeto,
  typename Distancia
  >
double DistanciaSecuenciasERP(const std::vector<Objeto>& patron,
			      const std::vector<Objeto>& secuencia,
			      Objeto hueco,
			      Distancia& distancia);

// Calcula la distancia minima entre dos secuencias
// utilizando el algoritmo LCSS (Longest Common Subsequence)
//
// Ver artículo: Lei Chen, Tamer Ozsu, Vincent Oria; "Robust and Fast 
//               Similarity Search for Moving Object Trajectories".
//
// Objeto: Puede ser una CELDA, un par de CELDAS, etc.
// Distancia: Tipo de la clasecilla que se usa para obtener las distancias
// patron: lo que su nombre indica.
// secuencia: lo que su nombre indica.
// umbral_matching: valor de distancia por debajo del cual se considera que hay matching
// dist: Objeto que se usa para calcular distancias
template<
  typename Objeto,
  typename Distancia
  >
double DistanciaSecuenciasLCSS(const std::vector<Objeto>& patron,
			      const std::vector<Objeto>& secuencia,
			      double umbral_matching,
			      Distancia& distancia);

#include "distceldas.tcpp"


#endif
