// -*-c++-*-
// Esta clase es idéntica a 'modeloNOMAD', pero emplea un modelo de palotes (sticky figure) en lugar de un modelo 
// de mallas 3D 

#ifndef MODELONOMADpalotes_H
#define MODELONOMADpalotes_H

#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>

#include <modelo-osg/modelo.h>

#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;
using namespace osg;

namespace osgDTE {

  class modeloNOMADpalotes: public modelo {
    
  public:

    // Constructores
    modeloNOMADpalotes() 
    {
      crearsegmentos(); _scale = 1;
      ang_left_arm.resize(4,0.0);ang_right_arm.resize(4,0.0);
      numero_alternativas = 20; angmax = PI/8; 
      alternatas_izq.resize(numero_alternativas);
    }
    modeloNOMADpalotes(osgDTE::observadormodelocoli* pobs,osgDTE::observadormodelo* obs) 
    {
      _pobs = pobs; _obs = obs; crearsegmentos(); _scale = 1;
      ang_left_arm.resize(4,0.0);ang_right_arm.resize(4,0.0);
      numero_alternativas = 20; angmax = PI/8; 
      alternatas_izq.resize(numero_alternativas);
    }
    modeloNOMADpalotes(osgDTE::observadormodelocoli* pobs) 
    {
      _pobs = pobs; _obs = NULL; crearsegmentos(); _scale = 1;
      ang_left_arm.resize(4,0.0);ang_right_arm.resize(4,0.0);
      numero_alternativas = 20; angmax = PI/8; 
      alternatas_izq.resize(numero_alternativas);
    }

    // Destructor
    ~modeloNOMADpalotes(){}
  
    // Segmentos del modelo por su nombre
    joint jBase, 
         jCuerpo, 
          jCabeza, 
           jBrazoIzq, 
            jAntebrazoIzq,
           jBrazoDer, 
            jAntebrazoDer;

    // ---------------------------------------------------------------
    //  COLISIONES Y LIMITES
    // ---------------------------------------------------------------
    // Función para proporcionar una lista de links sospechosos de producir colisión dado el movimiento de un joint 
    void linksMovidos(int i, std::vector<std::string>* lista);

    // Función para fijar los límites por defecto
    void limitesDefecto();

    // Función para fijar las colisiones por defecto
    void colisionesDefecto();

    // ---------------------------------------------------------------

    // ---------------------------------------------------------------
    // OBTENCIÓN DE DATOS DEL MODELO
    // ---------------------------------------------------------------
    // Función para acceder a un joint del modelo
    joint* accedeJoint(std::string nombre_joint);
    // ---------------------------------------------------------------

    // ---------------------------------------------------------------
    //  MOVIMIENTO
    // ---------------------------------------------------------------
    // Mueve todo el modelo hacia un punto determinado
    void mueveModelo(const osg::Vec3d& donde);

    // Pon postura inicial (brazos pegados al cuerpo)
    void reset();

    // Función para apuntar la cabeza del modelo a un cierto punto
    bool mueveCabeza(const osg::Vec3d& cabeza);

    // Función para alinear el torso en la postura dada por el eje que forman los hombros
    bool alineaHombros(const osg::Vec3d& hombroizq, const osg::Vec3d& hombroder);

    // Funciones COMPLETAS para situar los brazos del modelo en una cierta posición.
    // Tienen en cuenta límites, colisiones
    // y búsqueda de alternativas.
    std::vector<double> mueveBrazoIzq(const osg::Vec3d& manoizq,std::vector<double> angulos_brazo_izq, std::vector<double> angulos_pos_ant, const osg::Vec3d& codoizq);
    std::vector<double> mueveBrazoDer(const osg::Vec3d& manoder,std::vector<double> angulos_brazo_der, std::vector<double> angulos_pos_ant, const osg::Vec3d& cododer);
    bool mueveBrazoIzq(const osg::Vec3d& manoizq, const osg::Vec3d& codoizq);
    bool mueveBrazoDer(const osg::Vec3d& manoder, const osg::Vec3d& cododer);
    
    // Apunta el segmento usando IK analítico de Joel Mitchelson
    std::vector<double> apuntaIKBrazoIzqAn(const osg::Vec3d& donde);
    std::vector<double> apuntaIKBrazoDerAn(const osg::Vec3d& donde);
    
    // ---------------------------------------------------------------

    // Funciones para redimensionar el modelo
    void agrandaModelo(double factor = 1.05) {_scale *= factor; cambiarTamanio(_scale, jBase);cout << "Factor de escala del modelo: " << _scale << endl;}
    void achicaModelo(double factor = 1.05) {_scale /= factor; cambiarTamanio(_scale, jBase);cout << "Factor de escala del modelo: " << _scale << endl;}

    // Función para adaptar la altura del modelo a un valor determinado (en cm.)
    void setAlturaCara(int altura_cm);

    // Devuelve la altura del modelo (en cm)
    int devuelveAltura();

  protected:

    // ---------------------------------------------------------------
    //  MOVIMIENTO
    // ---------------------------------------------------------------
    
    // Funciones para ofrecer listas de alternativas a una posición dada.
    // Estas alternativas variarán en la posición del codo
    std::vector<osg::Vec3d>  alternativasBrazoIzq(int opciones, double angmax = 3.1416);
    std::vector<osg::Vec3d>  alternativasBrazoDer(int opciones, double angmax = 3.1416);

    // Funciones para apuntar los brazos, dadas las posiciones en globales del codo y el punto que
    // se quiere alcanzar con la mano
    std::vector<double> apuntaBrazoIzq(const osg::Vec3d& codo, const osg::Vec3d& donde);
    std::vector<double> apuntaBrazoDer(const osg::Vec3d& codo, const osg::Vec3d& donde);
    
    // Funciones para mover los brazos unos ciertos ángulos (expresados en radianes)
    void giraBrazoIzq(const std::vector<double> & radianes);
    void giraBrazoDer(const std::vector<double> & radianes);

    //  Funciones que comprueban si un angulo esta fuera o dentro de los limites.
    bool enlimites(int articulacion, double angulo);
    double enlimitesInt(int articulacion, double angulo);

    // Cargar todos los segmentos de ficheros
    void crearsegmentos();

    // Función que actualiza las aceleraciones y velocidades
    void actualizadatos(double intervalo);       

  };

}

#endif
