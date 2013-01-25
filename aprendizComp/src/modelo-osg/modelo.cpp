// -*-c++-*-

#include <osg/MatrixTransform>
#include <osg/Vec3d>
#include <osg/TriangleFunctor>
#include <rapid/rapid.h>

#include <modelo-osg/modelo.h>
#include <modelo-osg/utils.h>

#include <typeinfo>

using namespace std;

namespace osgDTE {

  // ---------------------------------------------------------------
  // FUNCIONES DE LA CLASE JOINT
  // ---------------------------------------------------------------


  void modelo::joint::actualizadatosjoint(double intervalo)
  {
    // Actualiza posición en globales
    posCM = posCMlocales * computeLocalToWorld(nodepath);

    // Actualiza velocidades
    velCM.x() = (posCM.x() - posCMant.x())/intervalo;
    velCM.y() = (posCM.y() - posCMant.y())/intervalo;
    velCM.z() = (posCM.z() - posCMant.z())/intervalo;

    //Actualiza aceleraciones
    aceCM.x() = (velCM.x() - velCMant.x())/intervalo;
    aceCM.y() = (velCM.y() - velCMant.y())/intervalo;
    aceCM.z() = (velCM.z() - velCMant.z())/intervalo;

    // Rellena los campos de memoria
    posCMant = posCM;
    velCMant = velCM;

  }

  /// Devuelve el nombre de la articulacion, que esta guardado en la transformacion
  const string & modelo::joint::getNombre()
  {
    return trf->getName();
  }

  // ---------------------------------------------------------------
  // ---------------------------------------------------------------

  // Apuntar una articulacion a un punto dado en coordenadas globales
  void modelo::apunta(joint& cual, const osg::Vec3d& donde)
  {
    //: Transformar a locales las coordenadas.
    osg::Vec3d donde_local = donde * computeWorldToLocal(cual.nodepath);
    
    //: Calcular la rotacion que nos lleva allí.
    osg::Matrixf rotacion;
    rotacion.makeRotate(cual.dirsegmento, donde_local);
    
    //: Aplicarsela al segmento
    cual.trf->preMult(rotacion);
    
  }

  // Apuntar una articulacion con un grado de libertad a un punto dado en coordenadas globales 
  void modelo::apunta1DOFx(joint& cual, const osg::Vec3d& donde)
  {

    //: Transformar a locales las coordenadas.
    osg::Vec3d donde_local = donde * computeWorldToLocal(cual.nodepath);
    
    //: Proyectar punto al plano normal al eje
    osg::Vec3d donde_local_proyectado(0.0f,donde_local.y(),donde_local.z());
    
    //: Calcular la rotacion que nos lleva allí.
    osg::Matrixf rotacion;
    rotacion.makeRotate(cual.dirsegmento, donde_local_proyectado);
    
    //: Aplicarsela al segmento
    cual.trf->preMult(rotacion);
  }

  // Apuntar una articulacion con un grado de libertad a un punto dado en coordenadas globales 
  void modelo::apunta1DOFy(joint& cual, const osg::Vec3d& donde)
  {

    //: Transformar a locales las coordenadas.
    osg::Vec3d donde_local = donde * computeWorldToLocal(cual.nodepath);
    
    //: Proyectar punto al plano normal al eje
    osg::Vec3d donde_local_proyectado(donde_local.x(),0.0f,donde_local.z());
    
    //: Calcular la rotacion que nos lleva allí.
    osg::Matrixf rotacion;
    rotacion.makeRotate(cual.dirsegmento, donde_local_proyectado);
    
    //: Aplicarsela al segmento
    cual.trf->preMult(rotacion);
  }

  // Apuntar una articulacion con dos grados de libertad (uno de tipo twist y otro un codo) 
  // Esto asume que el enlace esta alineado con el eje z local
  void modelo::apunta2DOFxz(joint& cual, const osg::Vec3d& donde)
  {
    // Primero se coloca el brazo en posición inicial, para que el 
    // plano de giro del eje Z sea correcto
    cual.trf->setMatrix(osg::Matrix::translate(0.0f,0.0f,115.0f));

    //: Transformar a locales las coordenadas
    osg::Vec3d donde_local_1 = donde * computeWorldToLocal(cual.nodepath);

    //: Proyectar punto al plano normal al eje Z.
    osg::Vec3d donde_local_XY(donde_local_1.x(), donde_local_1.y(), 0.0);

    //: Rotar allí el eje Y
    osg::Matrixf rotacionZ;
    rotacionZ.makeRotate(osg::Vec3d(0.0,1.0,0.0), donde_local_XY);
    cual.trf->preMult(rotacionZ);
    
    //: Llegados a este punto, "donde" está en el plano contenido 
    //: formado por los ejes Y y Z. Si lo pasamos a locales otra vez, la
    //: coordenada x deberia ser cero!
    osg::Vec3d donde_local_2 = donde * computeWorldToLocal(cual.nodepath);

    //: Ya podemos alinear el segmento (suponemos que su direccion es la del eje Z)
    osg::Matrixf rotacionX;
    rotacionX.makeRotate(osg::Vec3d(0.0,0.0,1.0), donde_local_2);
    cual.trf->preMult(rotacionX);
  }

  // Apuntar una articulacion tipo hombro con dos grados de libertad X e Y 
  void modelo::apunta2DOFxy(joint& cual, const osg::Vec3d& donde)
  {
    //: Transformar a locales las coordenadas
    osg::Vec3d donde_local_1 = donde * computeWorldToLocal(cual.nodepath);

    //: Proyectar punto al plano normal al eje X.
    osg::Vec3d donde_local_ZY(0.0, donde_local_1.y(), donde_local_1.z());

    //: Rotar allí el eje Z
    osg::Matrixf rotacionX;
    rotacionX.makeRotate(osg::Vec3d(0.0,0.0,1.0), donde_local_ZY);
    cual.trf->preMult(rotacionX);

    //: Llegados a este punto, "donde" está en el plano contenido 
    //: formado por los ejes Z y X. Si lo pasamos a locales otra vez, la
    //: coordenada y deberia ser cero!
    osg::Vec3d donde_local_2 = donde * computeWorldToLocal(cual.nodepath);

    //: Ya podemos alinear el segmento (suponemos que su direccion es la del eje Z)
    osg::Matrixf rotacionY;
    rotacionY.makeRotate(osg::Vec3d(0.0,0.0,1.0), donde_local_2);
    cual.trf->preMult(rotacionY);
  }

  void modelo::cambiaColor(joint& cual)
  {
  	// Primero obtener la mesh
    osg::Node* node = dynamic_cast<osg::Group*>(cual.trf->getChild(0))->getChild(0);
	if (node) {
		osgDTE::cambiarColorNodo(node);
	}
  }

  void modelo::devuelveColor(joint& cual)
  {
  	// Primero obtener la mesh
    osg::Node* node = dynamic_cast<osg::Group*>(cual.trf->getChild(0))->getChild(0);
	if (node) {
		osgDTE::devolverColorNodo(node);
	}
  }
 
  void modelo::cambiarTamanio(double factor, joint& cual)
  {
    // Cambia la mesh (sólo transformación del OSG)
    osg::Matrixd m = cual.trf->getMatrix();
    m(0,0) = factor;
    m(1,1) = factor;
    m(2,2) = factor;
    cual.trf->setMatrix(m);    
  }

  // ---------------------------------------------------------------
  // FUNCIONES PARA EL TRATAMIENTO DE COLISIONES
  // ---------------------------------------------------------------

  /** El TriangleFunctor que hay más abajo llamará al operator() de esta estructura */
  struct PasarTrianguloARAPID { 

    PasarTrianguloARAPID()
      :n_triangulos(0) {}

    // Modelo de colision
    RAPID_model* _modelo_colision;
    inline void set(RAPID_model* modelo)
    { _modelo_colision = modelo; }

    // Para pasarle los triángulos al RAPID como le gusta...
    double aux[3][3];
    
    // Numero de triangulos de esta mesh
    int n_triangulos; 
    
   

    // Tengo lo que tú quieres, RAPID...
    inline void operator() (const Vec3& v1, const Vec3& v2, const Vec3& v3, bool /* treatVertexDataAsTemporary */)
    {
      aux [0][0] =  v1.x();
      aux [0][1] =  v1.y();
      aux [0][2] =  v1.z();
      		    
      aux [1][0] =  v2.x();
      aux [1][1] =  v2.y();
      aux [1][2] =  v2.z();
      		    
      aux [2][0] =  v3.x();
      aux [2][1] =  v3.y();
      aux [2][2] =  v3.z();
      
      // Como no haya modelo de colision...esto se mata
      if (_modelo_colision) {
	_modelo_colision->AddTri(aux[0], aux[1], aux[2], n_triangulos++);
      }
      else {
	cerr << __FILE__ << ":" << __LINE__ << ":\nError al pasar triangulo a la libreria de colisiones " << endl;
      }

    }
  };
  
  /// Función para añadir un nuevo par de colisión al modelo
  void modelo::incluirColision(joint* parte1, joint* parte2)
  {  
    // Si no hay modelo de colisión del RAPID, se hace uno
    // y se carga dentro la malla de triángulos
    if(!parte1->_modelo_colision)
      {
	parte1->_modelo_colision = new RAPID_model; 	
	cargaTris(parte1);
      }

    if(!parte2->_modelo_colision)
      {
	parte2->_modelo_colision = new RAPID_model; 	
	cargaTris(parte2);
      }
     // Añade el par de colisión a la lista
    _pares_colision.push_back(std::pair<joint*,joint*>(parte1, parte2));
  }

  void modelo::cargaTris(joint* j)
  {
    if(j->_modelo_colision)
      {
    	
	// Sacar la estructura que contiene la mesh de la articulacion.
	osg::Group* group1 = dynamic_cast<osg::Group*>(j->trf->getChild(0));
	osg::Group* group2 = dynamic_cast<osg::Group*>(group1->getChild(0));
	osg::Geode* geode = dynamic_cast<osg::Geode*>(group2->getChild(0));
	if (geode) {
	  
	  // Crear un triangle functor de esos que va sacandole los triangulos.
	  TriangleFunctor<PasarTrianguloARAPID> tri;
	  tri.set(j->_modelo_colision);

	  // Y pasarselos al rapid
	  j->_modelo_colision->BeginModel();
	  int D = geode->getNumDrawables();
	  for (int d = 0; d < D; d++) {
	    geode->getDrawable(d)->accept(tri);
	  }
	  j->_modelo_colision->EndModel();
	}
	else {
	  cerr << "GÜORNING: ¡No se pudo crear el modelo de colision porque no hay mesh!" << endl;
	}
      }
    else {
      cerr << "GÜORNING: ¡No hay modelo de colision!" << endl;
    }
  }


  
}
