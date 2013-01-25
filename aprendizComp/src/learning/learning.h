// -*-c++-*-
#ifndef LIBLEARNINGV_H
#define LIBLEARNINGV_H

#include "boost/date_time/posix_time/posix_time.hpp"

#include <osg/Geode>
#include <osg/Matrix>
#include <osg/MatrixTransform>

#include <osg/Drawable>

#include <cassert>
#include <OpenThreads/Thread>
#include <osgViewer/Viewer>

#include <iostream>
#include <vector>
#include "utils.h"
#include "distceldas.h"
#include "libceldas.h"
#include "curvaturaLEARNER.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

// Representación de un patrón aprendido
struct Patron
{
  string nombre; // nombre del patrón
  vector<string> nombres; // nombre de cada trayectoria (ej. mano derecha)
  vector< vector<CELDA> > celdas; // celdas visitadas por cada trayectoria
  vector<COORDENADA> mov; // amplitudes de movimiento
  vector< vector<COORDENADA> > secs_key_XZ; // Keypoints XYZ en XZ
  vector< vector<COORDENADA> > secs_key_YZ; // Keypoints XYZ en YZ
  vector< vector<CAL> > cal_key_XZ; // Keypoints CAL en XZ 
  vector< vector<CAL> > cal_key_YZ; // Keypoints CAL en YZ

  void clear() {
    nombre="NULL"; nombres.clear(); celdas.clear();
    mov.clear(); secs_key_XZ.clear(); secs_key_YZ.clear();
    cal_key_XZ.clear(); cal_key_YZ.clear();
  }
  
};

// Clase que contiene las rutinas de aprendizaje
class Learning
{
 public:

  // Constructor
  Learning();

  // Destructor
  ~Learning() {}

  // ---------------------------------------------------------------------------------

  // Manejador de celdas. Dado que las celdas se usan para el aprendizaje,
  // tiene sentido que este manejador este aquí y no en el programa principal
  manejadorceldas* manceldas;

  // Lista de patrones almacenados en el aprendiz
  vector<Patron> patrones;

  // Secuencia que se quiere comparar (y, si procede, aprender)
  Patron *entrada;
  vector< vector<COORDENADA> > secuencia_entrada; // Secuencia de posiciones de entrada
  vector<COORDENADA> maximos_entrada;
  vector<COORDENADA> minimos_entrada;   

  // Pesos [0...1] que indican la importancia para el reconocimiento que tendrá cada una 
  // de las trayectorias que componen el gesto.
  vector<float> pesos;

  // Función para cambiar los valores de los pesos
  void cargaPesos(vector<float> nuevos_pesos);

  // Objeto que calcula distancias entre pares de celdas
  DistanciaParCeldas* dist2D;

  // Cambia los valores del grid de celdas
  void configuraCeldas(int delta_x, int delta_y, int delta_z, int x, int y, int z);

  // Función para leer un fichero de configuración que contiene la ruta de los ficheros
  // de patrón que se van a utilizar en el aprendiz
  bool cargaPatrones(string fichero_config, string fichero_nombres = "");

  // Función para guardar la secuencia almacenada como un nuevo patrón
  void guardaSecComoPatron(string name, bool guarda_en_fichero = true);

  // Función para filtrar un patrón determinado OJO QUE NO HACE NADA!!!
  void filtraPatron(Patron *patronco){}

  // Función para comenzar a leer una nueva secuencia
  void nuevaEntrada(string nombre, vector<string> nombres);

  // Función para añadir una posición al final de la secuencia, actualizando todos los campos de 'entrada'
  bool incluirEnSecuencia( vector<osg::Vec3f> in );

  // ---------------------------------------------------------------------------------

  // ---------------------------------------------------------------------------------
  // FUNCIÓN DE CURVATURA

  // Funciones de Curvatura de la secuencia percibida
  vector< vector<float> > Curvas_XZ; // Curvatura del patrón en el eje XZ
  vector< vector<float> > Curvas_YZ;;
  // Keypoints asociados a las funciones de curvatura percibidas
  vector< vector<int> > Keys_XZ;
  vector< vector<int> > Keys_YZ;
  vector< vector<COORDENADA> > Secuencias_Key_XZ; 
  vector< vector<COORDENADA> > Secuencias_Key_YZ;

  // Objetos para manejar las funciones de curvatura
  CURVATURALEARNER *curvaturaXZ;
  CURVATURALEARNER *curvaturaYZ;

  // Función para calcular las funciones de curvatura que definen el gesto
  // (K = -1 para curv. adaptativa)
  void calculaCurvaturas(int K = -1, int factor_diezmado = 1);

  // Objeto que calcula las distancias entre coordenadas
  DistanciaCoordenadas *distCoord;

  // ---------------------------------------------------------------------------------
  // CÁLCULO DE DISTANCIAS

  // Función para calcular las distancias en amplitud de movimientos
  double parecidoAmplitudes(vector<COORDENADA> a, vector<COORDENADA> b);

  // Función para calcular las distancias usando DTW sobre los valores XYZ de los Keypoints
  double parecidoDTWxyzKeypoints(vector< vector<COORDENADA> > a_XZ,
				 vector< vector<COORDENADA> > a_YZ,
				 vector< vector<COORDENADA> > b_XZ,
				 vector< vector<COORDENADA> > b_YZ,
				 int marcador_offset = -1);

  // Función para calcular las distancias usando EDR sobre los valores XYZ de los Keypoints
  double parecidoEDRxyzKeypoints(vector< vector<COORDENADA> > a_XZ, vector< vector<COORDENADA> > a_YZ,
				 vector< vector<COORDENADA> > b_XZ, vector< vector<COORDENADA> > b_YZ,
				 double umbralmatchEDR, 
				 int marcador_offset = -1);

  // Función para calcular las distancias usando ERP sobre los valores XYZ de los Keypoints
  double parecidoERPxyzKeypoints(vector< vector<COORDENADA> > a_XZ, vector< vector<COORDENADA> > a_YZ,
				 vector< vector<COORDENADA> > b_XZ, vector< vector<COORDENADA> > b_YZ,
				 COORDENADA hueco, int marcador_offset = -1);

  // Función para calcular las distancias usando ERP sobre los valores XYZ de los Keypoints
  double parecidoLCSSxyzKeypoints(vector< vector<COORDENADA> > a_XZ, vector< vector<COORDENADA> > a_YZ,
				  vector< vector<COORDENADA> > b_XZ, vector< vector<COORDENADA> > b_YZ,
				  double umbralmatchLCSS, int marcador_offset = -1);

  // Función para calcular las distancias usando DTW sobre los valores XYZ de los Keypoints
  double parecidoEuclideoKeypoints(vector< vector<COORDENADA> > a_XZ,
				   vector< vector<COORDENADA> > a_YZ,
				   vector< vector<COORDENADA> > b_XZ,
				   vector< vector<COORDENADA> > b_YZ,
				   int marcador_offset = -1);
  
  // Objeto que indica el número de patrones ganadores que se representa como resultado del cálculo de distancia
  // (a -1 los representa TODOS)
  int num_ganadores;

  // Patrón ganador (más parecido) y su valor de confianza
  string ganador;
  double conf_ganador;
  bool dos_primeros_ganadores;

  // Funciones para calcular distancias entre la secuencia y los patrones
  void calculaDistancias(int marcador_offset = -1);

  void calculaDistanciasEDRxyzKey(double umbral_match_EDR = 30.0, int marcador_offset = -1);
  void calculaDistanciasERPxyzKey(int huecox, int huecoy, int huecoz, int marcador_offset = -1);
  void calculaDistanciasLCSSxyzKey(double umbral_match_LCSS = 30.0, int marcador_offset = -1);
  void calculaDistanciasEuclideas(int marcador_offset = -1); // dist. euclideas entre keypoints 

private:

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
    semaforo_fin_gesto = false;
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
  
  // Flag que indica que se ha finalizado un gesto. OJO: Se desactiva desde fuera!
  bool semaforo_fin_gesto;

  // PARA PROBAR: Número del frame
  int _frameNumber;

private:
  
  // Frame que indica el inicio del movimiento
  int _frameInicio;

  // Frame que indica el final del movimiento
  int _frameFin;

};

#endif
