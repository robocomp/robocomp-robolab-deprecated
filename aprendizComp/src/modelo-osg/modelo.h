// -*-c++-*-

#ifndef MODELO_H
#define MODELO_H

#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>

#include <obs/Observado.h>
#include <obs/observadormodelocoli.h>
#include <obs/observadormodelo.h> 
#include <iostream>
#include <vector>
#include <utility>
#include <string>

/// Declaracion de la clase que implementa las colisiones
class RAPID_model;

namespace osgDTE {

  class modelo: public osg::Group, public Observado {

  public:
    
    // Constructor 
    modelo() {ang_left_arm.resize(4); ang_right_arm.resize(4);}

    // Destructor
    virtual ~modelo() {}

    /// Tipo de datos para los segmentos
    struct joint {
      
      /// Transformacion respecto al padre.
      osg::ref_ptr<osg::MatrixTransform> trf;

      /// Punteros a las transformaciones de sus antecesores.
      osg::NodePath nodepath;
      
      /// Direccion en locales del segmento
      osg::Vec3d dirsegmento;
      
      /// Limites de la articulacion en el espacio de DOF
      struct limiteDOF {
	double bajo, alto;  // limites del DOF en grados      
      };
      limiteDOF limiteeje[3];
      
      /// Masa
      double masa;

      /// Centro de masas
      osg::Vec3d posCMlocales; // Posición en locales
      osg::Vec3d posCM;  // Todas estas en globales
      osg::Vec3d posCMant;
      osg::Vec3d velCM;
      osg::Vec3d velCMant;
      osg::Vec3d aceCM;

      /// Modelo de colision
      RAPID_model *_modelo_colision;
      
      /// Constructor que lo deja casi todo sin hacer.
      joint(): trf(new osg::MatrixTransform), 
	       _modelo_colision(0) 
      {}
      
      /// Destructor
      ~joint() {}
        
      /// actualiza los datos de posición, velocidad y aceleración
      void actualizadatosjoint(double intervalo);

      /// dame el nombre de esta articulacion
      const std::string & getNombre();
    };

    // Ángulos de los brazos
    std::vector<double> ang_left_arm;
    std::vector<double> ang_right_arm;
    // ---------------------------------------------------------------
    //  COLISIONES Y LIMITES
    // ---------------------------------------------------------------
    // Puntero al manejador de colisiones
    osgDTE::observadormodelocoli* _pobs;

    // Tipo para un par de colision
    typedef std::pair<joint*,joint*> ParColision;

    // Pares de colision 
    std::vector<ParColision> _pares_colision;   

    // Función para proporcionar una lista de links sospechosos de producir colisión dado el movimiento de un joint 
    virtual void linksMovidos(int i, std::vector<std::string>* lista) = 0;

    // Función para fijar los límites por defecto
    virtual void limitesDefecto() = 0;

    // Función para fijar las colisiones por defecto
    virtual void colisionesDefecto() = 0;

    // Crear un par de colision usando RAPID 
    void incluirColision(joint* parte1, joint* parte2); 

    // ---------------------------------------------------------------

    // ---------------------------------------------------------------
    //  MOVIMIENTO
    // ---------------------------------------------------------------
    // Pon postura inicial (brazos pegados al cuerpo)
    virtual void reset() = 0;

    // Función para apuntar la cabeza del modelo a un cierto punto
    virtual bool mueveCabeza(const osg::Vec3d& cabeza) = 0;

    // Función para alinear el torso en la postura dada por el eje que forman los hombros
    virtual bool alineaHombros(const osg::Vec3d& hombroizq, const osg::Vec3d& hombroder){return false;}

    // Variables para el control y la visualización del número de alternativas 
    int numero_alternativas;
    double angmax;
    std::vector<osg::Vec3d>  alternatas_izq;
    std::vector<osg::Vec3d>  alternatas_der;

    // Funciones COMPLETAS para situar los brazos del modelo en una cierta posición.
    // Tienen en cuenta límites, colisiones
    // y búsqueda de alternativas.
    virtual bool mueveBrazoIzq(const osg::Vec3d& manoizq, const osg::Vec3d& codoizq = osg::Vec3d(0.0f,0.0f,0.0f)) = 0;
    virtual bool mueveBrazoDer(const osg::Vec3d& manoder, const osg::Vec3d& cododer = osg::Vec3d(0.0f,0.0f,0.0f)) = 0;
    
    virtual std::vector<double> mueveBrazoIzq(const osg::Vec3d& manoizq,std::vector<double> angulos_brazo_izq, std::vector<double> angulos_pos_ant, const osg::Vec3d& codoizq = osg::Vec3d(0.0f,0.0f,0.0f)) = 0;
    virtual std::vector<double> mueveBrazoDer(const osg::Vec3d& manoder,std::vector<double> angulos_brazo_der, std::vector<double> angulos_pos_ant, const osg::Vec3d& cododer = osg::Vec3d(0.0f,0.0f,0.0f)) = 0;
	
	virtual std::vector<double> getAngulosBrazoIzq() { std::vector<double> v;  return v;}
	virtual std::vector<double> getAngulosBrazoDer() {std::vector<double> v; v.resize(4); return v;}
	
    // ---------------------------------------------------------------
    //  ZMP
    // ---------------------------------------------------------------
    osg::Vec3d ZMP;

    // Función que calcula el ZMP del modelo en la pose actual.      
    virtual void calculaZMP(double intervalo)
    {
      std::cerr<<"¡OJO!: En el modelo que se está usando no hay función de cálculo de ZMP"<<std::endl;
    }

    // ---------------------------------------------------------------
    // CAMBIO DE CARACTERISTICAS DEL MODELO
    // ---------------------------------------------------------------
   
    // Resalta cambiandole el color una articulación 
    void cambiaColor(joint& cual);

    // Devuelve el color normal a una articulación 
    void devuelveColor(joint& cual);
    
    // Aumenta tamanio del modelo 
    void cambiarTamanio(double factor, joint& cual);

    // Función para adaptar la altura del modelo a un valor determinado (en cm.)
    virtual void setAlturaCara(int altura_cm) = 0;

    // Devuelve la altura del modelo (en cm)
    virtual int devuelveAltura() = 0;

    // ---------------------------------------------------------------
    // OBTENCIÓN DE DATOS DEL MODELO
    // ---------------------------------------------------------------

    //Observador de variacion en los angulos
    osgDTE::observadormodelo* _obs;

    // Obtiene las posiciones 3D de las articulaciones del modelo.
    // Cada modelo proporcionará su propia lista de articulaciones (mirar .h del modelo concreto)
    virtual std::vector<osg::Vec3f> obtieneDatosModelo3D() 
    {
      std::vector<osg::Vec3f> arg;
      arg.clear();
      return arg;
    }

    // Sitúa los marcadores virtuales CODA en el modelo. Según el modelo estos marcadores estarán
    // en un lugar un otro. (consultar .h del modelo concreto)
    virtual void situaMarcadoresVirtualesCODA(){}
    
    // Obtiene las posiciones 3D de los marcadores CODA virtuales que se hayan situado en el modelo.
    virtual std::vector<osg::Vec3f> obtieneDatosVirtualesCODAModelo3D()
    {
      std::vector<osg::Vec3f> arg;
      arg.clear();
      return arg;
    }
    
    // Función para acceder a un joint del modelo
    virtual joint* accedeJoint(std::string nombre_joint) = 0;

    // Funciones para redimensionar el modelo
    virtual void agrandaModelo(double factor = 1.05) {}
    virtual void achicaModelo(double factor = 1.05) {}
    
    // Mueve todo el modelo hacia un punto determinado
    virtual void mueveModelo(const osg::Vec3d& donde) = 0;

  protected:

    // ---------------------------------------------------------------
    //  MOVIMIENTO
    // ---------------------------------------------------------------
    // Apunta un segmento a esta direccion en globales
    void apunta(joint& cual, const osg::Vec3d& donde);

    // Apunta un segmento teniendo en cuenta solo el eje X de rotacion
    void apunta1DOFx(joint& cual, const osg::Vec3d& donde);
    
    // Apunta un segmento teniendo en cuenta solo el eje Y de rotacion
    void apunta1DOFy(joint& cual, const osg::Vec3d& donde);

    // Apunta un segmento teniendo en cuenta solo los ejes XZ de rotacion
    void apunta2DOFxz(joint& cual, const osg::Vec3d& donde);

    // Apunta un segmento teniendo en cuenta solo los ejes XY de rotacion
    void apunta2DOFxy(joint& cual, const osg::Vec3d& donde);

    

    // Apunta el brazo usando IK analítico de Joel Mitchelson
    virtual std::vector<double> apuntaIKBrazoIzqAn(const osg::Vec3d& donde) = 0;
    virtual std::vector<double> apuntaIKBrazoDerAn(const osg::Vec3d& donde) = 0;

    // Funciones para ofrecer listas de alternativas a una posición dada para una mano.
    // Estas alternativas variarán en la posición del codo
    virtual std::vector<osg::Vec3d>  alternativasBrazoIzq(int opciones, double angmax) = 0;
    virtual std::vector<osg::Vec3d>  alternativasBrazoDer(int opciones, double angmax) = 0;

    // Funciones para apuntar los brazos, dadas las posiciones en globales del codo y el punto que
    // se quiere alcanzar con la mano
    virtual std::vector<double> apuntaBrazoIzq(const osg::Vec3d& codo, const osg::Vec3d& donde) = 0;
    virtual std::vector<double> apuntaBrazoDer(const osg::Vec3d& codo, const osg::Vec3d& donde) = 0;

    // Funciones para mover los brazos unos ciertos ángulos (expresados en radianes)
    virtual void giraBrazoIzq(const std::vector<double> & radianes) = 0;
    virtual void giraBrazoDer(const std::vector<double> & radianes) = 0;

    //  Función que comprueba si un angulo esta fuera o dentro de los limites.
    //  Devuelve TRUE si el angulo es valido o FALSE si no lo es
    virtual bool enlimites(int articulacion, double angulo) = 0;
    virtual double enlimitesInt(int articulacion, double angulo) = 0;

    // Cargar todos los segmentos de ficheros
    virtual void crearsegmentos() = 0; 

    /// Carga el modelo de colision
    void cargaTris(joint* j);   

    // Factor de escala global para el modelo
    double _scale;

  };

}


#endif
