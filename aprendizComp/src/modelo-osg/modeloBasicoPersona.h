// -*-c++-*-

#ifndef MODELOBASICOPERSONA_H
#define MODELOBASICOPERSONA_H

#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>

#include <modelo-osg/modelo.h>

#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;

namespace osgDTE {

  class modelobasicopersona: public modelo {
    
  public:
    
    // Constructores
    modelobasicopersona() {crearsegmentos();_scale = 1.0;}
    modelobasicopersona(osgDTE::observadormodelocoli* pobs) {_pobs = pobs; crearsegmentos(); _scale = 1.0;}

    // Destructor
    ~modelobasicopersona() {}

    // Segmentos del modelo por su nombre
    joint jCintura, 
         jPecho, 
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
    bool mueveBrazoIzq(const osg::Vec3d& manoizq, const osg::Vec3d& codoizq);
    bool mueveBrazoDer(const osg::Vec3d& manoder, const osg::Vec3d& cododer);
    
    // Funciones para redimensionar el modelo
    void agrandaModelo(double factor = 1.05);
    void achicaModelo(double factor = 1.05);

    // Función para adaptar la altura del modelo a un valor determinado (en cm.)
    void setAlturaCara(int altura_cm);

    // Devuelve la altura del modelo (en cm)
    int devuelveAltura();

  protected:
    
    // ---------------------------------------------------------------
    //  MOVIMIENTO
    // ---------------------------------------------------------------

    // Apunta el brazo usando IK analítico de Joel Mitchelson
    std::vector<double> apuntaIKBrazoIzqAn(const osg::Vec3d& donde);
    std::vector<double> apuntaIKBrazoDerAn(const osg::Vec3d& donde);

    // Funciones para ofrecer listas de alternativas a una posición dada para una mano.
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

    //  Función que comprueba si un angulo esta fuera o dentro de los limites.
    //  Devuelve TRUE si el angulo es valido o FALSE si no lo es
    bool enlimites(int articulacion, double angulo);

    // Cargar todos los segmentos de ficheros
    void crearsegmentos();

  };

}
#endif
