// -*-c++-*-
#ifndef LIBLEARNINGV_H
#define LIBLEARNINGV_H

#include "boost/date_time/posix_time/posix_time.hpp"

#include <cassert>
#include <OpenThreads/Thread>
#include <osgProducer/Viewer>

#include <iostream>
#include <vector>
#include "distceldas.h"
#include "libceldas.h"
#include "curvaturaLEARNER.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

// Representación de un patrón aprendido
struct Patron
{
  vector<Tuple<CELDA> > ristra; // ristra de celdas visitadas por los brazos izquierdo y derecho          
  string nombre; // nombre del patrón
  COORDENADA mov_izq; // amplitud de movimientos del brazo izquierdo (X,Y,Z)
  COORDENADA mov_der; // amplitud de movimientos del brazo derecho (X,Y,Z)
  vector<COORDENADA> Secuencia_KeyXZ_izq; // Keypoints del patrón en el eje XZ
  vector<COORDENADA> Secuencia_KeyXZ_der; 
  vector<COORDENADA> Secuencia_KeyYZ_izq; // Keypoints del patrón en el eje YZ
  vector<COORDENADA> Secuencia_KeyYZ_der;
};

// Clase que contiene las rutinas de aprendizaje
class Learning
{
 public:

  // Constructor
  Learning() {
    secuencia.clear(); 
    patrones.clear(); 
    secuencia_coord.clear();
    CurvaXZ_izq.clear();
    CurvaYZ_izq.clear();
    CurvaXZ_der.clear();
    CurvaYZ_der.clear();
    manceldas = new manejadorceldas();
    dist2D = new DistanciaParCeldas(*manceldas);
    distCoord = new DistanciaCoordenadas(*manceldas);
    min_izq = COORDENADA(10,10,10);
    max_izq = COORDENADA(-10,-10,-10);
    min_der = COORDENADA(10,10,10);
    max_der = COORDENADA(-10,-10,-10);
  }

  // Destructor
  ~Learning() {}

  // ---------------------------------------------------------------------------------
  // CELDAS

  // Manejador de celdas. Dado que las celdas se usan para el aprendizaje,
  // tiene sentido que este manejador este aquí y no en el programa principal
  manejadorceldas* manceldas;

  // Lista de patrones almacenados en el aprendiz
  vector<Patron> patrones;

  // Secuencia que se quiere comparar (y, si procede, aprender)
  vector<Tuple<CELDA> > secuencia;          

  // Objeto que calcula distancias entre pares de celdas
  DistanciaParCeldas* dist2D;

  // Cambia los valores del grid de celdas
  void configuraCeldas(int delta_x, int delta_y, int delta_z, int x, int y, int z);

  // Función para leer un fichero de configuración que contiene la ruta de los ficheros
  // de patrón que se van a utilizar en el aprendiz
  bool cargaPatrones(string fichero_config);

  // Función para guardar la secuencia almacenada como un nuevo patrón
  void guardaSecComoPatron(string name, bool guarda_en_fichero = true);

  // Función para filtrar la secuencia
  void filtraSecuencia();

  // Función para filtrar un patrón determinado
  void filtraPatron(Patron *patronco);

  // Función para reducir el tamaño de los ficheros de patrón eliminando lecturas consecutivas
  // en la misma celda, pues el algoritmo de Viterbi no tiene en cuenta el tiempo que el efector 
  // final está en cada celda
  void filtraPatrones();

  // Función para leer un fichero de secuencia.
  bool cargaSecuencia(string fichero_sec);

  // Función para guardar un fichero de secuencia.
  void guardaSecuencia(string fichero_sec);

  // Función para resetear la secuencia
  void reseteaSecuencia() 
  {
    secuencia.clear(); secuencia_coord.clear();
    CurvaXZ_izq.clear(); CurvaYZ_izq.clear();
    CurvaXZ_der.clear(); CurvaYZ_der.clear();
  }

  // Función para añadir una posición al final de la secuencia
  void incluirEnSecuencia(CELDA izq, CELDA der);
  void incluirEnSecuencia(CELDA izq, CELDA der, COORDENADA coord_izq, COORDENADA coord_der);

  // Calcula distancias entre la secuencia y los patrones
  void calculaDistancias();

  // ---------------------------------------------------------------------------------

  // ---------------------------------------------------------------------------------
  // AMPLITUD DE MOVIMIENTOS

  // Atributos que indican la amplitud de movimientos de la secuencia percibida
  COORDENADA secuencia_mov_izq;
  COORDENADA secuencia_mov_der;

  // Función para actualizar los valores de amplitud de movimientos de la sec. percibida
  void actualizaAmplitudMovimientos(COORDENADA pos_izq, COORDENADA pos_der);
  void actualizaAmplitudMovimientos(int izq_x, int izq_y, int izq_z, int der_x, int der_y, int der_z);

  // Función para calcular las distancias en amplitud de movimientos
  double parecidoAmplitudes(COORDENADA a_izq, COORDENADA a_der, COORDENADA b_izq, COORDENADA b_der);

  // ---------------------------------------------------------------------------------

  // ---------------------------------------------------------------------------------
  // FUNCIÓN DE CURVATURA

  // Coordenadas de la secuencia percibida ¡¡¡OJO!!! -----> (¡¡¡EN MICROMETROS!!!) <------ ¡¡¡ OJO !!!
  vector<Tuple<COORDENADA> > secuencia_coord;          

  // Funciones de Curvatura de la secuencia percibida
  vector<float> CurvaXZ_izq; // Curvatura del patrón en el eje XZ
  vector<float> CurvaXZ_der;
  vector<float> CurvaYZ_izq; // Curvatura del patrón en el eje YZ
  vector<float> CurvaYZ_der;
  // Keypoints asociados a las funciones de curvatura percibidas
  vector<int> KeyXZ_izq;
  vector<int> KeyXZ_der;
  vector<int> KeyYZ_izq;
  vector<int> KeyYZ_der;
  vector<COORDENADA> Secuencia_KeyXZ_izq; // ¡¡¡OJO!!!: Al igual que TODAS las coordenadas asociadas a 
  vector<COORDENADA> Secuencia_KeyXZ_der; // curvaturas, éstas están en MICRÓMETROS!!!
  vector<COORDENADA> Secuencia_KeyYZ_izq;
  vector<COORDENADA> Secuencia_KeyYZ_der;

  // Objetos para manejar las funciones de curvatura
  CURVATURALEARNER *curvaturaXZ_izq;
  CURVATURALEARNER *curvaturaYZ_izq;
  CURVATURALEARNER *curvaturaXZ_der;
  CURVATURALEARNER *curvaturaYZ_der;

  // Función para calcular las cuatro funciones de curvatura que definen el gesto
  void calculaCurvaturas(int factor_diezmado = 1);

  // Objeto que calcula las distancias entre coordenadas
  DistanciaCoordenadas *distCoord;

  // ---------------------------------------------------------------------------------


private:

  vector<Patron>::iterator patron;  // Iterador para recorrer la lista de patrones

  COORDENADA min_izq;
  COORDENADA max_izq;
  COORDENADA min_der;
  COORDENADA max_der;

};

// Thread para detectar automáticamente el comienzo y el final de un gesto
class ProcesoGestos : public OpenThreads::Thread, public osgGA::GUIEventHandler
{
public:
  
  // Constructor
  ProcesoGestos(const bool& para):_para(para) 
  { 
    movido = false; 
    esta_quieto = true; 
    intervalo_mic = 250000; // por defecto, cada cuarto de segundo
  }

  // Proceso en si mihmo. OJO, no llamar directamente, utilizar start().
  void run();
  
  //Destructor
  ~ProcesoGestos(){}

  /// Aceptar un evento del teclado
  virtual void accept(osgGA::GUIEventHandlerVisitor& v) { v.visit(*this); }

  /// Procesar el evento del teclado
  virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&){return false;}

  // Flag que indica al thread que termine cuando es == true.
  const volatile bool& _para;

  // Flag que indica si ha habido movimiento desde la última vez que se ejcutó el thread o no
  // (si no lo ha habido es indicación de final de gesto)
  bool movido;

  // Flag que indica el estado del detector de gestos. A 1 indica que ahora mismo se considera 
  // que la persona esta quieta. A 0, que está moviéndose
  bool esta_quieto;

  // Intervalo en microsegundos entre comprobaciones de movimiento
  unsigned int intervalo_mic;
  
  // PARA PROBAR: Número del frame
  int _frameNumber;

private:
  
  // Frame que indica el inicio del movimiento
  int _frameInicio;

  // Frame que indica el final del movimiento
  int _frameFin;

};


#endif
