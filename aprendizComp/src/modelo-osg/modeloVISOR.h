// -*-c++-*-

#ifndef MODELOVISOR_H
#define MODELOVISOR_H

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

  class modeloVISOR: public modelo {
    
  public:
    
    // Constructores
    modeloVISOR() {_pobs = NULL; _obs = NULL; crearsegmentos();_scale = 1.0; particula = 0.0f;
                   ang_left_arm.resize(4,0.0);ang_right_arm.resize(4,0.0);
		   numero_alternativas = 20; angmax = 3*PI/4; //PI/8; 
		   alternatas_izq.resize(numero_alternativas);
		   alternatas_der.resize(numero_alternativas);}
    modeloVISOR(osgDTE::observadormodelocoli* pobs) {_pobs = pobs; _obs = NULL; crearsegmentos(); _scale = 1.0;
                ang_left_arm.resize(4,0.0);ang_right_arm.resize(4,0.0);
		numero_alternativas = 20; angmax = 3*PI/4; //PI/8; 
		alternatas_izq.resize(numero_alternativas);
		alternatas_der.resize(numero_alternativas);}

    // Destructor
    ~modeloVISOR() {}

    // Segmentos del modelo por su nombre
    joint jCintura, 
         jPecho, 
          jCabeza, 
           jBrazoIzq, 
            jAntebrazoIzq,
           jBrazoDer, 
            jAntebrazoDer,
         jMusloIzq,
          jPantorrillaIzq,
         jMusloDer,
          jPantorrillaDer;

    // Ángulos de los brazos
    //vector<double> ang_left_arm;
    //vector<double> ang_right_arm;

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
    
    std::vector <double> mueveBrazoIzq(const osg::Vec3d& manoizq,std::vector<double> angulos_brazo_izq, std::vector<double> angulos_pos_ant, const osg::Vec3d& codoizq);
    std::vector <double> mueveBrazoDer(const osg::Vec3d& manoder,std::vector<double> angulos_brazo_der, std::vector<double> angulos_pos_ant, const osg::Vec3d& cododer);
	
	virtual std::vector<double> getAngulosBrazoIzq();
	virtual std::vector<double> getAngulosBrazoDer();

    // Funciones para redimensionar el modelo
    void agrandaModelo(double factor = 1.05);
    void achicaModelo(double factor = 1.05);

    // Función para adaptar la altura del modelo a un valor determinado (en cm.)
    void setAlturaCara(int altura_cm);

    // Devuelve la altura del modelo (en cm)
    int devuelveAltura();

    // Particula de movimiento del mapa de reflexion, para que quede mas bonito
    float particula;

    // ---------------------------------------------------------------
    // OBTENCIÓN DE DATOS DEL MODELO
    // ---------------------------------------------------------------

    // Obtiene las posiciones 3D de las articulaciones del modelo.
    // En este caso se proporcionan las siguientes posiciones 3D:
    // 0:ManoDer , 1:CodoDer, 2:HombroDer , 3:Mano Izq , 4:CodoIzq , 5:HombroIzq , 6:Cabeza , 7:Cintura
    std::vector<osg::Vec3f> obtieneDatosModelo3D();

    // Lista de marcadores virtuales que se corresponden con los marcadores empleados en el sistema CODA
    std::vector<MatrixTransform*> marcadoresVirtuales;

    // Sitúa los marcadores virtuales CODA en el modelo
    void situaMarcadoresVirtualesCODA();
    
    // Obtiene las posiciones 3D de los marcadores CODA virtuales.
    // Para hacer coincidir el orden de estos marcadores virtuales con los reales, se devuelven en el siguiente formato
    // 0: Left Shoulder, 3: Middle Chest, 8: Right Head, 11: Right Shoulder, 12: Right Elbow Down,
    // 13: Right Elbow Up, 14: Right Hand Down, 15: Right Hand Up, 16: Left Hand Up, 17: Left Hand Down, 
    // 18: Left Elbow Up, 19: Left Elbow Down, 20: Left Head, 22: Neck (el resto de marcas, vacías)
    std::vector<osg::Vec3f> obtieneDatosVirtualesCODAModelo3D();

    // Función para acceder a un joint del modelo
    joint* accedeJoint(std::string nombre_joint);

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
    double enlimitesInt(int articulacion, double angulo);

    // Cargar todos los segmentos de ficheros
    void crearsegmentos();

  };

}
#endif
