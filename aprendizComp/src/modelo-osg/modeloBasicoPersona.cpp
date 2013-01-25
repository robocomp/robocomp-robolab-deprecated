// -*-c++-*-

#include <osgDB/ReadFile>
#include <string>
#include <cassert>
#include <algorithm>
#include <modelo-osg/modeloBasicoPersona.h>
#include <modelo-osg/utils.h>

using namespace std;

namespace osgDTE {

  void modelobasicopersona::crearsegmentos() 
  { 
    // Cintura
    jCintura.trf->setName("jCintura");
    jCintura.trf->setMatrix(osg::Matrix::identity());
    this->addChild(jCintura.trf.get());
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(string("../data/TIOcintura.osg"));
    if (node.valid()) {
    	 jCintura.trf->addChild(node.get());
    }
    else {
      cerr << "Warning: No encontre fichero de mesh para la cintura " << endl;
    } 
    jCintura.dirsegmento.set(0.0f, 0.0f, 19.5f);
    jCintura.nodepath.push_back(jCintura.trf.get());

    jCintura.trf->addChild(crearEjeCoordenadas(3.0));
    
    // Esto es necesario para que, cuando se modifique el tamanio del
    // modelo, no se oscurezca o se aclare su color.
    jCintura.trf->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
    
    // Pecho
    jPecho.trf->setName("jPecho");
    jPecho.trf->setMatrix(osg::Matrix::translate(0.0f, 0.0f, 19.5f));
    jCintura.trf->addChild(jPecho.trf.get());
    node = osgDB::readNodeFile(string("../data/TIOpecho.osg"));
    if (node.valid()) jPecho.trf->addChild(node.get());
    jPecho.dirsegmento.set(0.0f, 0.0f, 42.5f);
    jPecho.nodepath.push_back(jCintura.trf.get());
    jPecho.nodepath.push_back(jPecho.trf.get());  
    
    jPecho.trf->addChild(crearEjeCoordenadas(3.0));
    
    // Cabeza
    jCabeza.trf->setName("jCabeza");
    jCabeza.trf->setMatrix(osg::Matrix::translate(0.0f, 0.0f, 42.5f));
    jPecho.trf->addChild(jCabeza.trf.get());
    node = osgDB::readNodeFile(string("../data/TIOcabeza.osg"));
    if (node.valid()) jCabeza.trf->addChild(node.get());
    jCabeza.dirsegmento.set(0.0f,0.0f,25.0f);
    jCabeza.nodepath.push_back(jCintura.trf.get());
    jCabeza.nodepath.push_back(jPecho.trf.get());  
    jCabeza.nodepath.push_back(jCabeza.trf.get());
    
    jCabeza.trf->addChild(crearEjeCoordenadas(3.0));

    // Brazo izquierdo
    jBrazoIzq.trf->setName("jBrazoIzq");
    jBrazoIzq.trf->setMatrix(osg::Matrix::translate(17.0f,0.0f,37.5f));
    jPecho.trf->addChild(jBrazoIzq.trf.get());
    node = osgDB::readNodeFile(string("../data/TIObrazo.osg"));
    if (node.valid()) jBrazoIzq.trf->addChild(node.get());
    jBrazoIzq.dirsegmento.set(0.0f,0.0f,30.5f); 
    jBrazoIzq.nodepath.push_back(jCintura.trf.get());
    jBrazoIzq.nodepath.push_back(jPecho.trf.get());  
    jBrazoIzq.nodepath.push_back(jBrazoIzq.trf.get());
    
    jBrazoIzq.trf->addChild(crearEjeCoordenadas(3.0));
    
    
    // Antebrazo izquierdo
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::translate(0.0f,0.0f,30.5f));
    jAntebrazoIzq.trf->setName("jAntebrazoIzq");
    jBrazoIzq.trf->addChild(jAntebrazoIzq.trf.get());
    node = osgDB::readNodeFile(string("../data/TIOantebrazo.osg"));
    if (node.valid()) jAntebrazoIzq.trf->addChild(node.get());
    jAntebrazoIzq.dirsegmento.set(0.0f,0.0f,33.0f); // OJO ESTE NO LO SE...
    jAntebrazoIzq.nodepath.push_back(jCintura.trf.get());
    jAntebrazoIzq.nodepath.push_back(jPecho.trf.get());
    jAntebrazoIzq.nodepath.push_back(jBrazoIzq.trf.get());
    jAntebrazoIzq.nodepath.push_back(jAntebrazoIzq.trf.get());
    
    jAntebrazoIzq.trf->addChild(crearEjeCoordenadas(3.0));
    

    // Brazo derecho
    jBrazoDer.trf->setName("jBrazoDer");
    jBrazoDer.trf->setMatrix(osg::Matrix::translate(-17.0f,0.0f,37.5f));
    jPecho.trf->addChild(jBrazoDer.trf.get());
    node = osgDB::readNodeFile(string("../data/TIObrazo.osg"));
    if (node.valid()) jBrazoDer.trf->addChild(node.get());
    jBrazoDer.dirsegmento.set(0.0f,0.0f,30.5f); 
    jBrazoDer.nodepath.push_back(jCintura.trf.get());
    jBrazoDer.nodepath.push_back(jPecho.trf.get());
    jBrazoDer.nodepath.push_back(jBrazoDer.trf.get());
    
    jBrazoDer.trf->addChild(crearEjeCoordenadas(3.0));
    
    // Antebrazo derecho
    jAntebrazoDer.trf->setMatrix(osg::Matrix::translate(0.0f,0.0f,30.5f));
    jAntebrazoDer.trf->setName("jAntebrazoDer");
    jBrazoDer.trf->addChild(jAntebrazoDer.trf.get());
    node = osgDB::readNodeFile(string("../data/TIOantebrazo.osg"));
    if (node.valid()) jAntebrazoDer.trf->addChild(node.get());
    jAntebrazoDer.dirsegmento.set(0.0f,0.0f,33.0f); // OJO ESTE NO LO SE...
    jAntebrazoDer.nodepath.push_back(jCintura.trf.get());
    jAntebrazoDer.nodepath.push_back(jPecho.trf.get());
    jAntebrazoDer.nodepath.push_back(jBrazoDer.trf.get());
    jAntebrazoDer.nodepath.push_back(jAntebrazoDer.trf.get());
    
    jAntebrazoDer.trf->addChild(crearEjeCoordenadas(3.0));

    reset();
    
  }

  // Función para acceder a un joint del modelo
  modelo::joint* modelobasicopersona::accedeJoint(std::string nombre_joint)
  {
    if (nombre_joint == "jCintura")
      return &jCintura;
    if (nombre_joint =="jPecho")
      return &jPecho;
    if (nombre_joint == "jCabeza")
      return &jCabeza;
    if (nombre_joint == "jBrazoIzq")
      return &jBrazoIzq;
    if (nombre_joint == "jAntebrazoIzq")
      return &jBrazoIzq;
    if (nombre_joint == "jBrazoDer")
      return &jBrazoDer;
    if (nombre_joint == "jAntebrazoDer")
      return &jAntebrazoDer;
    
    cout<<"AVISO: No se ha encontrado el joint "<<nombre_joint<<" en el modelo básico de persona"<<endl;
    return 0;
  }

  void modelobasicopersona::mueveModelo(const osg::Vec3d& donde)
  {
     // Pasos previos para considerar los cambios de escala. En primer lugar, se fija la escala del modelo a 1...
    cambiarTamanio(1.0,jCintura);
    // ...en segunda lugar, se aplica el factor de escala inverso al punto 3D que se quiere alcanzar
    Vec3d dondenorm(donde);
    dondenorm = dondenorm - jCintura.trf->getMatrix().getTrans();
    dondenorm /= _scale;
    dondenorm = dondenorm + jCintura.trf->getMatrix().getTrans();

    jCintura.trf->setMatrix(osg::Matrix::translate(dondenorm));

    // Antes de volver, se deshace el cambio de escala
    cambiarTamanio(_scale,jCintura);
  }

  void modelobasicopersona::reset()
  {
    jCintura.trf->setMatrix(osg::Matrix::identity());
    jPecho.trf->setMatrix(osg::Matrix::translate(0.0f, 0.0f, 19.5f));
    jCabeza.trf->setMatrix(osg::Matrix::translate(0.0f, 0.0f, 42.5f));
    jBrazoIzq.trf->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0),1.0,0.0,0.0) *
			     osg::Matrix::translate(17.0f,0.0f,37.5f));
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::rotate(osg::inDegrees(45.0),0.0,0.0,1.0) *
				 osg::Matrix::translate(0.0f,0.0f,30.5f));
    jBrazoDer.trf->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0),1.0,0.0,0.0) *
			     osg::Matrix::translate(-17.0f,0.0f,37.5f));
    jAntebrazoDer.trf->setMatrix(osg::Matrix::rotate(osg::inDegrees(-45.0),0.0,0.0,1.0) *
				 osg::Matrix::translate(0.0f,0.0f,30.5f));
  }

  void modelobasicopersona::linksMovidos(int i, std::vector<std::string>* lista)
  {
    switch (i)
      {
      case 19:
      case 18:
	lista->push_back(jAntebrazoIzq.trf->getName());
	break;

      case 9:
      case 8:
	lista->push_back(jAntebrazoDer.trf->getName());
	break;

      case 17:
      case 16:
	lista->push_back(jAntebrazoIzq.trf->getName());
	lista->push_back(jBrazoIzq.trf->getName());
	break;
	
      case 7:
      case 6:
	lista->push_back(jAntebrazoDer.trf->getName());
	lista->push_back(jBrazoDer.trf->getName());
	break;

      default:
	cerr<<"Índice de articulación no válido para el robot HOAP-1"<<endl;
	break;
      }

  }

  // Nota: Los límites de la persona, cuando ha sido posible, se han extraído
  // empíricamente a base de retorcer mis articulaciones hasta el dolor... espero 
  // que esto sirva como media de los movimientos que puede hacer un humano, porque
  // no pienso repetir los experimentos
  void modelobasicopersona::limitesDefecto()
  {
    jBrazoIzq.limiteeje[2].bajo = 0;
    jBrazoIzq.limiteeje[2].alto = 0;
    jBrazoIzq.limiteeje[1].bajo = -70;   
    jBrazoIzq.limiteeje[1].alto = 130;
    jBrazoIzq.limiteeje[0].bajo = -100; 
    jBrazoIzq.limiteeje[0].alto = 180;

    jBrazoDer.limiteeje[2].bajo = 0;
    jBrazoDer.limiteeje[2].alto = 0;
    jBrazoDer.limiteeje[1].bajo = -130; 
    jBrazoDer.limiteeje[1].alto = 70;
    jBrazoDer.limiteeje[0].bajo = -100; 
    jBrazoDer.limiteeje[0].alto = 180;

    jAntebrazoIzq.limiteeje[2].bajo = -90; 
    jAntebrazoIzq.limiteeje[2].alto = 90;
    jAntebrazoIzq.limiteeje[1].bajo = 0;
    jAntebrazoIzq.limiteeje[1].alto = 0;
    jAntebrazoIzq.limiteeje[0].bajo = -170;  
    jAntebrazoIzq.limiteeje[0].alto = 1;

    jAntebrazoDer.limiteeje[2].bajo = -90; 
    jAntebrazoDer.limiteeje[2].alto = 90;
    jAntebrazoDer.limiteeje[1].bajo = 0;
    jAntebrazoDer.limiteeje[1].alto = 0;
    jAntebrazoDer.limiteeje[0].bajo = -170; 
    jAntebrazoDer.limiteeje[0].alto = 1;   

  }

  // Función para fijar las colisiones por defecto
  void modelobasicopersona::colisionesDefecto()
  {
    // Establece una serie de colisiones por defecto
    // colisiones del antebrazo izdo
    incluirColision(&jAntebrazoIzq,&jCintura);
    incluirColision(&jAntebrazoIzq,&jPecho);
    incluirColision(&jAntebrazoIzq,&jCabeza);
    incluirColision(&jAntebrazoIzq,&jBrazoDer);
    incluirColision(&jAntebrazoIzq,&jAntebrazoDer);
    // colisiones del antebrazo dcho
    incluirColision(&jAntebrazoDer,&jCintura);
    incluirColision(&jAntebrazoDer,&jPecho);
    incluirColision(&jAntebrazoDer,&jCabeza);
    incluirColision(&jAntebrazoDer,&jBrazoIzq);
    // Colisiones del brazo izdo
    incluirColision(&jBrazoIzq,&jCintura);
    incluirColision(&jBrazoIzq,&jCabeza);
    // Colisiones del brazo dcho
    incluirColision(&jBrazoDer,&jCintura);
    incluirColision(&jBrazoDer,&jCabeza);  
  }

  // Comprobación de ángulos para el modelo de persona
  bool modelobasicopersona::enlimites(int articulacion, double angulo)
  {
    switch (articulacion)
      {
      case 6:
	if ( (angulo<jBrazoDer.limiteeje[0].bajo)||(angulo>jBrazoDer.limiteeje[0].alto) )
	  return false;
	break;
      case 7:
	if ( (angulo<jBrazoDer.limiteeje[1].bajo)||(angulo>jBrazoDer.limiteeje[1].alto) )
	  return false;
	break;
      case 8:
	if ( (angulo<jAntebrazoDer.limiteeje[2].bajo)||(angulo>jAntebrazoDer.limiteeje[2].alto) )
	  return false;
	break;
      case 9:
	if ( (angulo<jAntebrazoDer.limiteeje[0].bajo)||(angulo>jAntebrazoDer.limiteeje[0].alto) )
	  return false;
	break;

      case 16:
	if ( (angulo<jBrazoIzq.limiteeje[0].bajo)||(angulo>jBrazoIzq.limiteeje[0].alto) )
	  return false;
	break;
      case 17:
	if ( (angulo<jBrazoIzq.limiteeje[1].bajo)||(angulo>jBrazoIzq.limiteeje[1].alto) )
	  return false;
	break;
      case 18:
	if ( (angulo<jAntebrazoIzq.limiteeje[2].bajo)||(angulo>jAntebrazoIzq.limiteeje[2].alto) )
	  return false;
	break;
      case 19:
	if ( (angulo<jAntebrazoIzq.limiteeje[0].bajo)||(angulo>jAntebrazoIzq.limiteeje[0].alto) )
	  return false;
	break;
	
      default:
	cerr<<"modeloBasicoPersona: Indice de articulacion no valido"<<endl; 
	break;
      } 

    return true;
  }

  // Funciones para redimensionar el modelo
  void modelobasicopersona::agrandaModelo(double factor) 
  {
    _scale *= factor; 
    cambiarTamanio(_scale, jCintura); 
    cout << "Factor de escala del modelo: " << _scale << endl;
  }

  void modelobasicopersona::achicaModelo(double factor) 
  {
    _scale /= factor; 
    cambiarTamanio(_scale, jCintura); 
    cout << "Factor de escala del modelo: " << _scale << endl;
  }

  // Función para adaptar la altura a la que se sitúa la cara del modelo  a un valor determinado (en cm.)
  void modelobasicopersona::setAlturaCara(int altura_cm)
  {
    double altura_modelo; // altura normalizada (sin aplicar el factor de escala actual) del modelo
    double factor;        // factor de escalado

    // En primer lugar, se fija la escala del modelo a 1
    cambiarTamanio(1.0,jCintura);
    altura_modelo = 42.5 + 42.5 + // las dimensiones de muslo y pantorrilla me las invento porque yo lo valgo
      jCintura.dirsegmento.length() + jPecho.dirsegmento.length() + jCabeza.dirsegmento.length();
    altura_modelo = 0.9025*altura_modelo; // Altura de la nariz, obtenida como aproximación usando tablas antropométricas
    _scale = 1.0; // El factor de escala que pudiera haber se sobreescribe con 1

    // En segundo lugar, se fija el nuevo factor de escala del modelo
    factor = ((double)altura_cm) / altura_modelo;
    if (factor >= 1.0) {
      agrandaModelo(factor);
    }
    else {
      factor = 1 / factor;
      achicaModelo(factor);
    }    
  }

  // Devuelve la altura del modelo (en cm)
  int modelobasicopersona::devuelveAltura()
  {
    cout<<"ADVERTENCIA: Este modelo no tiene piernas. Se devuelve la altura como distancia cintura-cara"<<endl;
    double cabcintura = jPecho.dirsegmento.length() + jCintura.dirsegmento.length() + jCabeza.dirsegmento.length()/2;

    return( (int)(cabcintura) );
  }

  // Apuntar el brazo derecho utilizando método geométrico 
  //
  //  Ver tesis de Joel Mitchelson (2003)
  //
  std::vector<double> modelobasicopersona::apuntaIKBrazoDerAn(const osg::Vec3d& donde)
  {
    // Matrices de resultados parciales
    osg::Matrix homb;
    osg::Matrix cod;

    // Transformar a coordenadas locales del hombro el punto que
    // se quiere alcanzar. 
    Vec3d final_localhombro = donde * computeWorldToLocal(jBrazoDer.nodepath);

    // Si no es alcanzable, transformarlo en otro que sí lo sea
    Vec3d alcanzable(final_localhombro);
    double donde_llega = final_localhombro.length();
    // _TIEMPO
    double longitud_brazo = jBrazoDer.dirsegmento.length();
    double longitud_antebrazo = jAntebrazoDer.dirsegmento.length();
    double donde_puede_llegar =  longitud_brazo + longitud_antebrazo;
    if (donde_llega > donde_puede_llegar) alcanzable *= donde_puede_llegar/donde_llega;
    donde_llega = alcanzable.length();

    // Calcular el radio del circulo que describe el codo
    double radio_length =
      (( donde_llega  + longitud_brazo + longitud_antebrazo ) *
       (  -donde_llega + longitud_brazo + longitud_antebrazo ) *
       (  donde_llega  - longitud_brazo + longitud_antebrazo ) *
       (  donde_llega  + longitud_brazo - longitud_antebrazo )) /
      (4.0f * donde_llega * donde_llega);
    // ¿Cuando da esto negativo?...
    radio_length = (radio_length > 0.0f) ? sqrt(radio_length) : 0.0f;

    // Calcular la distancia del hombro a la que se dobla el codo
    double donde_dobla = sqrt(longitud_brazo*longitud_brazo - radio_length*radio_length);
    
    // Calcular la nueva direccion del eje del codo. Esta direccion esta en el plano
    // perpendicular a la linea que une el hombro y el punto de alcance (linea_brazo)
    // De todas las direcciones posibles, tomamos la que esta mas cerca a la anterior
    // Si se hace uno el dibujo, se puede comprobar que esta es:
    // linea_brazo x (direccion_anterior x linea_brazo) 
    // Del signo no estoy muy seguro...
    Vec3d linea_brazo = Matrixd::transform3x3(alcanzable, computeLocalToWorld(jBrazoDer.nodepath));
    Vec3d direccion_anterior = Matrixd::transform3x3(Vec3d(1.0f,0.0f,0.0f),computeLocalToWorld(jAntebrazoDer.nodepath));

    // Direccion nueva de la bisagra del codo...
    // Esta es la mas próxima a la anterior. Si se quisiera hacer caso de un marcador del codo,
    // quizás éste sería el sito para hacerlo.
    Vec3d direccion_nueva =  linea_brazo ^ (direccion_anterior ^ linea_brazo);

    // Calcular el punto donde se va el codo
    Vec3d codo = Unit(linea_brazo) * donde_dobla + Unit(direccion_nueva ^ linea_brazo) * radio_length;

    // Calcular la matriz actualizada de rotacion del hombro
    Vec3d trans1 = jBrazoDer.trf->getMatrix().getTrans();
    Vec3d x1 = Unit(Matrixd::transform3x3(direccion_nueva,computeWorldToLocal(jPecho.nodepath)));
    Vec3d z1 = Unit(Matrixd::transform3x3(codo,computeWorldToLocal(jPecho.nodepath))); // El brazo va por el eje z
    Vec3d y1 = z1 ^ x1;
    // Matriz de resultado parcial: Contiene los 3 DOF que el método de Joel Mitchelson asigna al hombro. 
    homb.set(Matrixd(x1._v[0],   x1._v[1],   x1._v[2],   0,
		     y1._v[0],   y1._v[1],   y1._v[2],   0,
		     z1._v[0],   z1._v[1],   z1._v[2],   0,
		     trans1._v[0],trans1._v[1],trans1._v[2],1));  

    // Calcular la matriz actualizada de rotacion del codo
    Vec3d trans2 = jAntebrazoDer.trf->getMatrix().getTrans();
    Vec3d x2(1,0,0); // Este no cambia, es el de la articulacion. 
    Vec3d z2 = Unit(Matrixd::transform3x3(Vec3d(linea_brazo - codo),computeWorldToLocal(jBrazoDer.nodepath)));
    Vec3d y2 = z2 ^ x2;				    
    // Matriz de resultado parcial: contiene el único DOF que el método de Joel Mitchelson asigna al codo    
    cod.set(Matrixd(x2._v[0],   x2._v[1],   x2._v[2],   0,
		    y2._v[0],   y2._v[1],   y2._v[2],   0,
		    z2._v[0],   z2._v[1],   z2._v[2],   0,
		    trans2._v[0],trans2._v[1],trans2._v[2],1));

    // Angulo que gira el codo derecho
    Vec3d auxvec = Unit(Matrixd::transform3x3(Vec3d(linea_brazo - codo),computeWorldToLocal(jBrazoDer.nodepath)));
    double anglo = -acos( Vec3d(0.0f,0.0f,1.0f) * auxvec);

    // Extracción de los ángulos a partir de la matriz de transformación
    std::vector<double> rarm_joint(4);

    // RARM_JOINT[1] del hoap:
    rarm_joint[0] = atan2(-homb(2,1),homb(2,2));
    // RARM_JOINT[2] del hoap
    rarm_joint[1] = atan2(homb(2,0),sqrt(homb(0,0)*homb(0,0) + homb(1,0)*homb(1,0)));
    // RARM_JOINT[3] del hoap
    rarm_joint[2] = atan2(-homb(1,0),homb(0,0));
    // RARM_JOINT[4] del hoap
    rarm_joint[3] = anglo;

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoDer.trf->setMatrix(osg::Matrix::rotate(rarm_joint[3], 1.0, 0.0, 0.0) * // RARM_JOINT[4]
				 osg::Matrix::rotate(rarm_joint[2],0.0,0.0,1.0) *  // RARM_JOINT[3]	    
				 osg::Matrix::translate(0.0f,0.0f,30.5f));

    jBrazoDer.trf->setMatrix(osg::Matrix::rotate(rarm_joint[1],0.0,1.0,0.0) *  // RARM_JOINT[2]
			     osg::Matrix::rotate(rarm_joint[0],1.0,0.0,0.0) *  // RARM_JOINT[1]
			     osg::Matrix::translate(-17.0f,0.0f,37.5f));

    // Offsets
    rarm_joint[0] = rarm_joint[0] - PI/2;
    if (rarm_joint[0]<-PI) rarm_joint[0] += 2*PI;
    else if (rarm_joint[0]>PI) rarm_joint[0] -= 2*PI;

    return(rarm_joint);
  }
  
  // Apuntar el brazo izquierdo utilizando método geométrico 
  //
  //  Ver tesis de Joel Mitchelson (2003)
  //
  std::vector<double> modelobasicopersona::apuntaIKBrazoIzqAn(const osg::Vec3d& donde)
  {
    // Matrices de resultados parciales
    osg::Matrix homb;
    osg::Matrix cod;

    // Transformar a coordenadas locales del hombro el punto que
    // se quiere alcanzar. 
    Vec3d final_localhombro = donde * computeWorldToLocal(jBrazoIzq.nodepath);

    // Si no es alcanzable, transformarlo en otro que sí lo sea
    Vec3d alcanzable(final_localhombro);
    double donde_llega = final_localhombro.length();
    // _TIEMPO
    double longitud_brazo = jBrazoIzq.dirsegmento.length();
    double longitud_antebrazo = jAntebrazoIzq.dirsegmento.length();
    double donde_puede_llegar =  longitud_brazo + longitud_antebrazo;
    if (donde_llega > donde_puede_llegar){ alcanzable *= donde_puede_llegar/donde_llega;} 
    donde_llega = alcanzable.length();

    // Calcular el radio del circulo que describe el codo
    double radio_length =
      (( donde_llega  + longitud_brazo + longitud_antebrazo ) *
       (  -donde_llega + longitud_brazo + longitud_antebrazo ) *
       (  donde_llega  - longitud_brazo + longitud_antebrazo ) *
       (  donde_llega  + longitud_brazo - longitud_antebrazo )) /
      (4.0f * donde_llega * donde_llega);
    // ¿Cuando da esto negativo?...
    radio_length = (radio_length > 0.0f) ? sqrt(radio_length) : 0.0f;

    // Calcular la distancia del hombro a la que se dobla el codo
    double donde_dobla = sqrt(longitud_brazo*longitud_brazo - radio_length*radio_length);
    
    // Calcular la nueva direccion del eje del codo. Esta direccion esta en el plano
    // perpendicular a la linea que une el hombro y el punto de alcance (linea_brazo)
    // De todas las direcciones posibles, tomamos la que esta mas cerca a la anterior
    // Si se hace uno el dibujo, se puede comprobar que esta es:
    // linea_brazo x (direccion_anterior x linea_brazo) 
    // Del signo no estoy muy seguro...
    Vec3d linea_brazo = Matrixd::transform3x3(alcanzable, computeLocalToWorld(jBrazoIzq.nodepath));
    Vec3d direccion_anterior = Matrixd::transform3x3(Vec3d(1.0f,0.0f,0.0f),computeLocalToWorld(jAntebrazoIzq.nodepath));

    // Direccion nueva de la bisagra del codo...
    // Esta es la mas próxima a la anterior. Si se quisiera hacer caso de un marcador del codo,
    // quizás éste sería el sito para hacerlo.
    Vec3d direccion_nueva =  linea_brazo ^ (direccion_anterior ^ linea_brazo);

    // Calcular el punto donde se va el codo
    Vec3d codo = Unit(linea_brazo) * donde_dobla + Unit(direccion_nueva ^ linea_brazo) * radio_length;

    // Calcular la matriz actualizada de rotacion del hombro
    Vec3d trans1 = jBrazoIzq.trf->getMatrix().getTrans();
    Vec3d x1 = Unit(Matrixd::transform3x3(direccion_nueva,computeWorldToLocal(jPecho.nodepath)));
    Vec3d z1 = Unit(Matrixd::transform3x3(codo,computeWorldToLocal(jPecho.nodepath))); // El brazo va por el eje z

    Vec3d y1 = z1 ^ x1;
    // Matriz de resultado parcial: Contiene los 3 DOF que el método de Joel Mitchelson asigna al hombro.
    homb.set(Matrixd(x1._v[0],   x1._v[1],   x1._v[2],   0,
		     y1._v[0],   y1._v[1],   y1._v[2],   0,
		     z1._v[0],   z1._v[1],   z1._v[2],   0,
		     trans1._v[0],trans1._v[1],trans1._v[2],1));  

    // Calcular la matriz actualizada de rotacion del codo
    Vec3d trans2 = jAntebrazoIzq.trf->getMatrix().getTrans();
    Vec3d x2(1,0,0); // Este no cambia, es el de la articulacion. 
    Vec3d z2 = Unit(Matrixd::transform3x3(Vec3d(linea_brazo - codo),computeWorldToLocal(jBrazoIzq.nodepath)));
    
    Vec3d y2 = z2 ^ x2;				    
    // Matriz de resultado parcial: contiene el único DOF que el método de Joel Mitchelson asigna al codo    
    cod.set(Matrixd(x2._v[0],   x2._v[1],   x2._v[2],   0,
		    y2._v[0],   y2._v[1],   y2._v[2],   0,
		    z2._v[0],   z2._v[1],   z2._v[2],   0,
		    trans2._v[0],trans2._v[1],trans2._v[2],1));

    // Angulo que gira el codo derecho
    Vec3d auxvec = Unit(Matrixd::transform3x3(Vec3d(linea_brazo - codo),computeWorldToLocal(jBrazoIzq.nodepath)));

    double anglo = -acos( Vec3d(0.0f,0.0f,1.0f) * auxvec);
      
    // Extracción de los ángulos a partir de la matriz de transformación
    std::vector<double> larm_joint(4);
    // LARM_JOINT[1] del hoap:
    larm_joint[0] = atan2(-homb(2,1),homb(2,2));
    // LARM_JOINT[2] del hoap
    larm_joint[1] = atan2(homb(2,0),sqrt(homb(0,0)*homb(0,0) + homb(1,0)*homb(1,0)));
    // LARM_JOINT[3] del hoap
    larm_joint[2] = atan2(-homb(1,0),homb(0,0));
    // LARM_JOINT[4] del hoap
    larm_joint[3] = anglo;

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::rotate(larm_joint[3],1.0,0.0,0.0) * // LARM_JOINT[4]
				 osg::Matrix::rotate(larm_joint[2],0.0,0.0,1.0) * // LARM_JOINT[3]	 
				 osg::Matrix::translate(0.0f,0.0f,30.5f));
    
    jBrazoIzq.trf->setMatrix(osg::Matrix::rotate(larm_joint[1],0.0,1.0,0.0) *  // LARM_JOINT[2]
			     osg::Matrix::rotate(larm_joint[0],1.0,0.0,0.0) *  // LARM_JOINT[1]
			     osg::Matrix::translate(17.0f,0.0f,37.5f));

    // Offsets
    larm_joint[0] = larm_joint[0] - PI/2;
    if (larm_joint[0]<-PI) larm_joint[0] += 2*PI;
    else if (larm_joint[0]>PI) larm_joint[0] -= 2*PI;

    return(larm_joint);
  }

  // Función para obtener alternativas a la posición del codo derecho, para el HOAP-1.
  // Las alternativas se ordenarán de acuerdo a esta posición, siendo la densidad de 
  // alternativas mayor en la zona cercana al codo
  // ENTRADAS: 
  //   int opciones - Número de alternativas que va a ofrecer la función
  // SALIDAS:
  //   Un vector con las posibles posiciones GLOBALES 3D, ordenadas desde las más cercanas a la
  //   posición de referencia a las más alejadas
  std::vector<osg::Vec3d>  modelobasicopersona::alternativasBrazoDer(int opciones, double angmax)
  {
    std::vector<osg::Vec3d> opc(opciones);
    bool par = true;
    double theta;

    // Posición de la mano en locales del hombro 
    Vec3d mano = jAntebrazoDer.dirsegmento * jAntebrazoDer.trf->getMatrix();
    mano.normalize();
    // Posición del codo en locales del hombro
    Vec3d codo = jBrazoDer.dirsegmento;
 
    if ( (opciones % 2) != 0)  // si es impar
      {
	opciones = opciones - 1; // se resta uno para el procesado del resto de alternativas
	par = false;
      }

    for (int i=0;i<(opciones/2);i++)
      {
	// Obtiene pareja de ángulos conjugados para el codo (positivo y negativo)
	// (DISTRIBUCION EXPONENCIAL más espesa en torno a la posición ref)
	theta = ( 1 / pow( (double)100,((double)(i+1)/(double)(opciones/2)) ) )*angmax;
	if (theta<-PI) theta += 2*PI;
	else if (theta>PI) theta -= 2*PI;

	// Calcula las x e y de las posic. alternativas
	opc[opciones - 2*i - 2] = codo * osg::Matrix::rotate(theta, mano) * computeLocalToWorld(jBrazoDer.nodepath);
	opc[opciones - 2*i - 1] = codo * osg::Matrix::rotate(-theta,mano) * computeLocalToWorld(jBrazoDer.nodepath);
      }

    if (!par) // si no era par, se añade un punto extra (al final) en el opuesto a ref
      {
	theta = -angmax;
	opc[opciones+1] = codo * osg::Matrix::rotate(theta, mano) * computeLocalToWorld(jBrazoDer.nodepath);
      }

    return (opc);
  }

  // Función para obtener alternativas a la posición del codo izquierdo, para el HOAP-1.
  // ENTRADAS: 
  //   int opciones - Número de alternativas que va a ofrecer la función
  //   Vec3d ref    - Posición de referencia. Las alternativas se ordenarán de acuerdo
  //                  a dicha posición
  // SALIDAS:
  //   Un vector con las posibles posiciones 3D, ordenadas desde las más cercanas a la
  //   posición de referencia a las más alejadas
  std::vector<osg::Vec3d>  modelobasicopersona::alternativasBrazoIzq(int opciones, double angmax)
  {
    std::vector<osg::Vec3d> opc(opciones);
    bool par = true;
    double theta;

    // Posición de la mano en locales del hombro 
    Vec3d mano = jAntebrazoIzq.dirsegmento * jAntebrazoIzq.trf->getMatrix();
    mano.normalize();
    // Posición del codo en locales del hombro
    Vec3d codo = jBrazoIzq.dirsegmento;

    if ( (opciones % 2) != 0)  // si es impar
      {
	opciones = opciones - 1; // se resta uno para el procesado del resto de alternativas
	par = false;
      }

    for (int i=0;i<(opciones/2);i++)
      {
	// Obtiene pareja de ángulos conjugados para el codo (positivo y negativo)
	// (DISTRIBUCION EXPONENCIAL más espesa en torno a la posición ref)
	theta = ( 1 / pow( (double)100,((double)(i+1)/(double)(opciones/2)) ) )*angmax;
	if (theta<-PI) theta += 2*PI;
	else if (theta>PI) theta -= 2*PI;

	// Calcula las x e y de las posic. alternativas
	opc[opciones - 2*i - 2] = codo * osg::Matrix::rotate(theta, mano) * computeLocalToWorld(jBrazoIzq.nodepath);
	opc[opciones - 2*i - 1] = codo * osg::Matrix::rotate(-theta,mano) * computeLocalToWorld(jBrazoIzq.nodepath);
      }

    if (!par) // si no era par, se añade un punto extra (al final) en el opuesto a ref
      {
	theta = angmax;
	opc[opciones+1] = codo * osg::Matrix::rotate(theta, mano) * computeLocalToWorld(jBrazoIzq.nodepath);
      }

    return (opc);
  }

  // Función para apuntar el brazo derecho, dadas las posiciones en globales del codo y el punto que se quiere alcanzar con
  // la mano (Persona)
  std::vector<double> modelobasicopersona::apuntaBrazoDer(const osg::Vec3d& codo, const osg::Vec3d& donde)
  {
    // Matrices de resultados parciales
    osg::Matrix homb;
    osg::Matrix cod;

    // Transformar a coordenadas locales del hombro el punto que
    // se quiere alcanzar. 
    Vec3d final_localhombro = donde * computeWorldToLocal(jBrazoDer.nodepath);

    // Si no es alcanzable, transformarlo en otro que sí lo sea
    Vec3d alcanzable(final_localhombro);
    double donde_llega = final_localhombro.length();
    // _TIEMPO
    double longitud_brazo = jBrazoDer.dirsegmento.length();
    double longitud_antebrazo = jAntebrazoDer.dirsegmento.length();
    double donde_puede_llegar =  longitud_brazo + longitud_antebrazo;
    if (donde_llega > donde_puede_llegar) alcanzable *= donde_puede_llegar/donde_llega;
    donde_llega = alcanzable.length();

    // Calcular el radio del circulo que describe el codo
    double radio_length =
      (( donde_llega  + longitud_brazo + longitud_antebrazo ) *
       (  -donde_llega + longitud_brazo + longitud_antebrazo ) *
       (  donde_llega  - longitud_brazo + longitud_antebrazo ) *
       (  donde_llega  + longitud_brazo - longitud_antebrazo )) /
      (4.0f * donde_llega * donde_llega);
    // ¿Cuando da esto negativo?...
    radio_length = (radio_length > 0.0f) ? sqrt(radio_length) : 0.0f;

    // Calcular la distancia del hombro a la que se dobla el codo
    double donde_dobla = sqrt(longitud_brazo*longitud_brazo - radio_length*radio_length);

    // Calcular la nueva direccion del eje del codo. Esta direccion esta en el plano
    // perpendicular a la linea que une el hombro y el punto de alcance (linea_brazo)
    // De todas las direcciones posibles, tomamos la que deja el codo lo más cerca posible
    // de la posición proporcionada
    Vec3d codo_local = codo * computeWorldToLocal(jBrazoDer.nodepath);
    Vec3d direccion_nueva = alcanzable ^ codo_local; // esta es la nueva dirección para la bisagra (eje X) del codo, en locales
    Vec3d direccion_codo = direccion_nueva ^ alcanzable; // vector que apunta al codo en el círculo de posiciones posibles, en locales
    
    // Paso a globales los vectores para posicionar el codo y la línea del brazo
    direccion_nueva = Matrixd::transform3x3(direccion_nueva, computeLocalToWorld(jBrazoDer.nodepath));
    direccion_codo = Matrixd::transform3x3(direccion_codo, computeLocalToWorld(jBrazoDer.nodepath));
    Vec3d linea_brazo = Matrixd::transform3x3(alcanzable, computeLocalToWorld(jBrazoDer.nodepath));

    // Calcular el punto donde se va el codo
    Vec3d codo_valido = Unit(linea_brazo) * donde_dobla + Unit(direccion_codo) * radio_length;

    // Calcular la matriz actualizada de rotacion del hombro
    Vec3d trans1 = jBrazoDer.trf->getMatrix().getTrans();
    Vec3d x1 = Unit(Matrixd::transform3x3(direccion_nueva,computeWorldToLocal(jPecho.nodepath)));
    Vec3d z1 = Unit(Matrixd::transform3x3(codo_valido,computeWorldToLocal(jPecho.nodepath))); // El brazo va por el eje z
    Vec3d y1 = z1 ^ x1;
    // Matriz de resultado parcial: Contiene los 3 DOF que el método de Joel Mitchelson asigna al hombro. 
    homb.set(Matrixd(x1._v[0],   x1._v[1],   x1._v[2],   0,
		     y1._v[0],   y1._v[1],   y1._v[2],   0,
		     z1._v[0],   z1._v[1],   z1._v[2],   0,
		     trans1._v[0],trans1._v[1],trans1._v[2],1));  

    // Calcular la matriz actualizada de rotacion del codo
    Vec3d trans2 = jAntebrazoDer.trf->getMatrix().getTrans();
    Vec3d x2(1,0,0); // Este no cambia, es el de la articulacion. 
    Vec3d z2 = Unit(Matrixd::transform3x3(Vec3d(linea_brazo - codo_valido),computeWorldToLocal(jBrazoDer.nodepath)));
    Vec3d y2 = z2 ^ x2;				    
    // Matriz de resultado parcial: contiene el único DOF que el método de Joel Mitchelson asigna al codo    
    cod.set(Matrixd(x2._v[0],   x2._v[1],   x2._v[2],   0,
		    y2._v[0],   y2._v[1],   y2._v[2],   0,
		    z2._v[0],   z2._v[1],   z2._v[2],   0,
		    trans2._v[0],trans2._v[1],trans2._v[2],1));

    // Angulo que gira el codo derecho
    Vec3d auxvec = Unit(Matrixd::transform3x3(Vec3d(linea_brazo - codo_valido),computeWorldToLocal(jBrazoDer.nodepath)));
    double anglo = -acos( Vec3d(0.0f,0.0f,1.0f) * auxvec);

    // Extracción de los ángulos a partir de la matriz de transformación
    std::vector<double> rarm_joint(4);

    // RARM_JOINT[1] del hoap:
    rarm_joint[0] = atan2(-homb(2,1),homb(2,2));
    // RARM_JOINT[2] del hoap
    rarm_joint[1] = atan2(homb(2,0),sqrt(homb(0,0)*homb(0,0) + homb(1,0)*homb(1,0)));
    // RARM_JOINT[3] del hoap
    rarm_joint[2] = atan2(-homb(1,0),homb(0,0));
    // RARM_JOINT[4] del hoap
    rarm_joint[3] = anglo;

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoDer.trf->setMatrix(osg::Matrix::rotate(rarm_joint[3], 1.0, 0.0, 0.0) * // RARM_JOINT[4]
				 osg::Matrix::rotate(rarm_joint[2],0.0,0.0,1.0) *  // RARM_JOINT[3]	    
				 osg::Matrix::translate(0.0f,0.0f,33.5f));//30.5f));

    jBrazoDer.trf->setMatrix(osg::Matrix::rotate(rarm_joint[1],0.0,1.0,0.0) *  // RARM_JOINT[2]
			     osg::Matrix::rotate(rarm_joint[0],1.0,0.0,0.0) *  // RARM_JOINT[1]
			     osg::Matrix::translate(-17.0f,0.0f,37.5f));

    // Offsets
    rarm_joint[0] = rarm_joint[0] - PI/2;
    if (rarm_joint[0]<-PI) rarm_joint[0] += 2*PI;
    else if (rarm_joint[0]>PI) rarm_joint[0] -= 2*PI;

    return(rarm_joint);
  }

  // Función para apuntar el brazo izquierdo, dadas las posiciones en globales del codo y el punto que se quiere alcanzar con
  // la mano (Persona)
  std::vector<double> modelobasicopersona::apuntaBrazoIzq(const osg::Vec3d& codo, const osg::Vec3d& donde)
  {
    // Matrices de resultados parciales
    osg::Matrix homb;
    osg::Matrix cod;

    // Transformar a coordenadas locales del hombro el punto que
    // se quiere alcanzar. 
    Vec3d final_localhombro = donde * computeWorldToLocal(jBrazoIzq.nodepath);

    // Si no es alcanzable, transformarlo en otro que sí lo sea
    Vec3d alcanzable(final_localhombro);
    double donde_llega = final_localhombro.length();
    // _TIEMPO
    double longitud_brazo = jBrazoIzq.dirsegmento.length();
    double longitud_antebrazo = jAntebrazoIzq.dirsegmento.length();
    double donde_puede_llegar =  longitud_brazo + longitud_antebrazo;
    if (donde_llega > donde_puede_llegar){ alcanzable *= donde_puede_llegar/donde_llega;} 
    donde_llega = alcanzable.length();

    // Calcular el radio del circulo que describe el codo
    double radio_length =
      (( donde_llega  + longitud_brazo + longitud_antebrazo ) *
       (  -donde_llega + longitud_brazo + longitud_antebrazo ) *
       (  donde_llega  - longitud_brazo + longitud_antebrazo ) *
       (  donde_llega  + longitud_brazo - longitud_antebrazo )) /
      (4.0f * donde_llega * donde_llega);
    // ¿Cuando da esto negativo?...
    radio_length = (radio_length > 0.0f) ? sqrt(radio_length) : 0.0f;

    // Calcular la distancia del hombro a la que se dobla el codo
    double donde_dobla = sqrt(longitud_brazo*longitud_brazo - radio_length*radio_length);
    
    // Calcular la nueva direccion del eje del codo. Esta direccion esta en el plano
    // perpendicular a la linea que une el hombro y el punto de alcance (linea_brazo)
    // De todas las direcciones posibles, tomamos la que deja el codo lo más cerca posible
    // de la posición proporcionada
    Vec3d codo_local = codo * computeWorldToLocal(jBrazoIzq.nodepath);
    Vec3d direccion_nueva = alcanzable ^ codo_local; // esta es la nueva dirección para la bisagra (eje X) del codo, en locales
    Vec3d direccion_codo = direccion_nueva ^ alcanzable; // vector que apunta al codo en el círculo de posiciones posibles, en locales
    
    // Paso a globales los vectores para posicionar el codo y la línea del brazo
    direccion_nueva = Matrixd::transform3x3(direccion_nueva, computeLocalToWorld(jBrazoIzq.nodepath));
    direccion_codo = Matrixd::transform3x3(direccion_codo, computeLocalToWorld(jBrazoIzq.nodepath));
    Vec3d linea_brazo = Matrixd::transform3x3(alcanzable, computeLocalToWorld(jBrazoIzq.nodepath));

    // Calcular el punto donde se va el codo
    Vec3d codo_valido = Unit(linea_brazo) * donde_dobla + Unit(direccion_codo) * radio_length;

    // Calcular la matriz actualizada de rotacion del hombro
    Vec3d trans1 = jBrazoIzq.trf->getMatrix().getTrans();
    Vec3d x1 = Unit(Matrixd::transform3x3(direccion_nueva,computeWorldToLocal(jPecho.nodepath)));
    Vec3d z1 = Unit(Matrixd::transform3x3(codo_valido,computeWorldToLocal(jPecho.nodepath))); // El brazo va por el eje z

    Vec3d y1 = z1 ^ x1;
    // Matriz de resultado parcial: Contiene los 3 DOF que el método de Joel Mitchelson asigna al hombro.
    homb.set(Matrixd(x1._v[0],   x1._v[1],   x1._v[2],   0,
		     y1._v[0],   y1._v[1],   y1._v[2],   0,
		     z1._v[0],   z1._v[1],   z1._v[2],   0,
		     trans1._v[0],trans1._v[1],trans1._v[2],1));  

    // Calcular la matriz actualizada de rotacion del codo
    Vec3d trans2 = jAntebrazoIzq.trf->getMatrix().getTrans();
    Vec3d x2(1,0,0); // Este no cambia, es el de la articulacion. 
    Vec3d z2 = Unit(Matrixd::transform3x3(Vec3d(linea_brazo - codo_valido),computeWorldToLocal(jBrazoIzq.nodepath)));
    
    Vec3d y2 = z2 ^ x2;				    
    // Matriz de resultado parcial: contiene el único DOF que el método de Joel Mitchelson asigna al codo    
    cod.set(Matrixd(x2._v[0],   x2._v[1],   x2._v[2],   0,
		    y2._v[0],   y2._v[1],   y2._v[2],   0,
		    z2._v[0],   z2._v[1],   z2._v[2],   0,
		    trans2._v[0],trans2._v[1],trans2._v[2],1));

    // Angulo que gira el codo derecho
    Vec3d auxvec = Unit(Matrixd::transform3x3(Vec3d(linea_brazo - codo_valido),computeWorldToLocal(jBrazoIzq.nodepath)));

    double anglo = -acos( Vec3d(0.0f,0.0f,1.0f) * auxvec);
      
    // Extracción de los ángulos a partir de la matriz de transformación
    std::vector<double> larm_joint(4);
    // LARM_JOINT[1] del hoap:
    larm_joint[0] = atan2(-homb(2,1),homb(2,2));
    // LARM_JOINT[2] del hoap
    larm_joint[1] = atan2(homb(2,0),sqrt(homb(0,0)*homb(0,0) + homb(1,0)*homb(1,0)));
    // LARM_JOINT[3] del hoap
    larm_joint[2] = atan2(-homb(1,0),homb(0,0));
    // LARM_JOINT[4] del hoap
    larm_joint[3] = anglo;

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::rotate(larm_joint[3],1.0,0.0,0.0) * // LARM_JOINT[4]
				 osg::Matrix::rotate(larm_joint[2],0.0,0.0,1.0) * // LARM_JOINT[3]	 
				 osg::Matrix::translate(0.0f,0.0f,33.5f));//30.5f));
    
    jBrazoIzq.trf->setMatrix(osg::Matrix::rotate(larm_joint[1],0.0,1.0,0.0) *  // LARM_JOINT[2]
			     osg::Matrix::rotate(larm_joint[0],1.0,0.0,0.0) *  // LARM_JOINT[1]
			     osg::Matrix::translate(17.0f,0.0f,37.5f));

    // Offsets
    larm_joint[0] = larm_joint[0] - PI/2;
    if (larm_joint[0]<-PI) larm_joint[0] += 2*PI;
    else if (larm_joint[0]>PI) larm_joint[0] -= 2*PI;

    return(larm_joint);
  }

  // Funciones para mover los brazos unos ciertos ángulos (expresados en radianes)
  void modelobasicopersona::giraBrazoIzq(const std::vector<double> & radianes)
  {
    assert(radianes.size() == 4);

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::rotate(radianes[3],1.0,0.0,0.0) *//LARMJOINT[4]
				 osg::Matrix::rotate(radianes[2],0.0,0.0,1.0) *//LARMJOINT[3] 
				 osg::Matrix::translate(0.0f,0.0f,30.5f));

    jBrazoIzq.trf->setMatrix(osg::Matrix::rotate(radianes[1],0.0,1.0,0.0) *  // LARMJOINT[2]
			     osg::Matrix::rotate(radianes[0],1.0,0.0,0.0) *  // LARMJOINT[1]
			     osg::Matrix::rotate(osg::inDegrees(90.0),1.0,0.0,0.0) *              
			     osg::Matrix::translate(17.0,0.0,37.5));
  }

  void modelobasicopersona::giraBrazoDer(const std::vector<double> & radianes)
  {
    assert(radianes.size() == 4);

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoDer.trf->setMatrix(osg::Matrix::rotate(radianes[3],1.0,0.0,0.0) *//RARMJOINT[4]
				 osg::Matrix::rotate(radianes[2],0.0,0.0,1.0) *//RARMJOINT[3] 
				 osg::Matrix::translate(0.0f,0.0f,30.5f));

    jBrazoDer.trf->setMatrix(osg::Matrix::rotate(radianes[1],0.0,1.0,0.0) *  // RARMJOINT[2]
			     osg::Matrix::rotate(radianes[0],1.0,0.0,0.0) *  // RARMJOINT[1]
			     osg::Matrix::rotate(osg::inDegrees(90.0),1.0,0.0,0.0) *              
			     osg::Matrix::translate(-17.0,0.0,37.5));
  }

  bool modelobasicopersona::mueveCabeza(const osg::Vec3d& cabeza)
  {

    // Pasos previos para considerar los cambios de escala. En primer lugar, se fija la escala del modelo a 1...
    cambiarTamanio(1.0,jCintura);  
    // ...en segunda lugar, se aplica el factor de escala inverso al punto 3D que se quiere alcanzar
    Vec3d cabezanorm(cabeza);
    cabezanorm = cabezanorm - jCintura.trf->getMatrix().getTrans();
    cabezanorm /= _scale;
    cabezanorm = cabezanorm + jCintura.trf->getMatrix().getTrans();
    
    // apunta el pecho en la dirección de la cabeza (si apuntase la cabeza tendríamos un modelo
    // con el cuerpo quieto y moviendo la cabeza de acá para allá como un loco)
    apunta(jPecho, cabezanorm);    
    
    // para que los hombros estén siempre apuntando hacia delante, se exploran las posibilidades cercanas a ver
    // si alguna de ellas los centra más (la idea es que, respecto a la cintura, el cuerpo esté siempre orientado
    // hacia delante. Así luego se podrá girar de acuerdo a las posiciones de los hombros de forma más coherente...
    osg::Vec3d hombroizq;
    osg::Vec3d hombroder;
  
    hombroizq = osg::Vec3d(17.0f,0.0f,37.5f) * computeWorldToLocal(jPecho.nodepath);
    hombroder = osg::Vec3d(-17.0f,0.0f,37.5f) * computeWorldToLocal(jPecho.nodepath);
    double alinDef = fabs(hombroizq.y() - hombroder.y());
    
    hombroizq = osg::Vec3d(17.0f,0.0f,37.5f) * osg::Matrix::rotate(0.01,0.0,0.0,1.0) * computeWorldToLocal(jPecho.nodepath);
    hombroder = osg::Vec3d(-17.0f,0.0f,37.5f) * osg::Matrix::rotate(0.01,0.0,0.0,1.0) * computeWorldToLocal(jPecho.nodepath);
    double alinA = fabs(hombroizq.y() - hombroder.y());
    
    hombroizq = osg::Vec3d(17.0f,0.0f,37.5f) * osg::Matrix::rotate(-0.01,0.0,0.0,1.0) * computeWorldToLocal(jPecho.nodepath);
    hombroder = osg::Vec3d(-17.0f,0.0f,37.5f) * osg::Matrix::rotate(-0.01,0.0,0.0,1.0) * computeWorldToLocal(jPecho.nodepath);
    double alinB = fabs(hombroizq.y() - hombroder.y());
    
    if ( (alinA<alinDef)&&(alinA<alinB) )
      jPecho.trf->preMult(osg::Matrix::rotate(-0.01f,0.0f,0.0f,1.0f));
    if ( (alinB<alinDef)&&(alinB<alinA) )
      jPecho.trf->preMult(osg::Matrix::rotate(0.01f,0.0f,0.0f,1.0f));
       
    // Antes de volver, se deshace el cambio de escala
    cambiarTamanio(_scale,jCintura);

    return(true);

  }

  // Función para alinear el torso en la postura dada por el eje que forman los hombros
  bool modelobasicopersona::alineaHombros(const osg::Vec3d& hombroizq, const osg::Vec3d& hombroder)
  {
    osg::Vec3d hi_locales = hombroizq * computeWorldToLocal(jPecho.nodepath);
    osg::Vec3d hd_locales = hombroder * computeWorldToLocal(jPecho.nodepath);

    // Proyectamos los hombros (en locales del torso) sobre Z = 0 (plano transversal)
    hi_locales.z() = 0;
    hd_locales.z() = 0;

    // calculamos el eje que va de un hombro a otro, y el giro necesario para alinear
    // los hombros del modelo con dicho eje
    osg::Vec3d eje = hi_locales - hd_locales;
    eje.normalize();
    double angulo = atan(eje.y()/eje.x());

    // aplicamos dicho giro al eje Z del torso
    jPecho.trf->preMult(osg::Matrix::rotate(angulo,0.0f,0.0f,1.0f));

    return(true);
  }

  bool modelobasicopersona::mueveBrazoIzq(const osg::Vec3d& manoizq, const osg::Vec3d& codoizq)
  {
    int ki;
    bool angulo_valido[4];
    std::vector<double> angulos_brazo_izq(4);
    std::vector<double> angulos_aux_izq(4);
    std::vector<osg::Vec3d> alternatas(30);
    int alter = 20;
    int par_colisionado = -1;

    bool result = true;
    bool fuera_limites = false;
    bool fuera_limites_alt = false;
    bool encuentra_alternativa;
    MatrixTransform* ultimaBrazo = new MatrixTransform;
    MatrixTransform* ultimaAntebrazo = new MatrixTransform;
    
    // Pasos previos para considerar los cambios de escala. En primer lugar, se fija la escala del modelo a 1...
    cambiarTamanio(1.0,jCintura);
    // ...en segunda lugar, se aplica el factor de escala inverso al punto 3D que se quiere alcanzar
    Vec3d manoizqnorm(manoizq);
    manoizqnorm = manoizqnorm - jCintura.trf->getMatrix().getTrans();
    manoizqnorm /= _scale;
    manoizqnorm = manoizqnorm + jCintura.trf->getMatrix().getTrans();
    
    // Lo primero, comprobar que hay un observador de colisiones
    assert(_pobs!=NULL);

    // Salva la posición anterior, por si es necesario volver a ella
    ultimaBrazo->setMatrix( this->jBrazoIzq.trf->getMatrix() );
    ultimaAntebrazo->setMatrix( this->jAntebrazoIzq.trf->getMatrix() );
    for (ki=0; ki<4; ki++)
      {
	angulo_valido[ki] = true;
	angulos_aux_izq[ki] = angulos_brazo_izq[ki];
      }
    
    // Calcula nuevos ángulos. Según tengamos pose estimada para el codo o no, se utiliza dicha información para situar el codo
    if (codoizq.length2()==0)
      angulos_brazo_izq = apuntaIKBrazoIzqAn(manoizqnorm);
    else
      angulos_brazo_izq = apuntaBrazoIzq(codoizq, manoizqnorm);
    
    // Comprobar límites
    if ( !enlimites(16, angulos_brazo_izq[0]*180/PI) ) 
      {angulo_valido[0] = false; fuera_limites = true;}//cout<<"Límite alcanzado en LJOINT1"<<endl;}
    if ( !enlimites(17, angulos_brazo_izq[1]*180/PI) ) 
      {angulo_valido[1] = false; fuera_limites = true;}//cout<<"Límite alcanzado en LJOINT2"<<endl;}
    if ( !enlimites(18, angulos_brazo_izq[2]*180/PI) ) 
      {angulo_valido[2] = false; fuera_limites = true;}//cout<<"Límite alcanzado en LJOINT3"<<endl;}
    if ( !enlimites(19, angulos_brazo_izq[3]*180/PI) ) 
      {angulo_valido[3] = false; fuera_limites = true;}//cout<<"Límite alcanzado en LJOINT4"<<endl;}
    
    // Se actualiza el buffer de ángulos auxiliares, por si no hubiera alternativa después y hubiera
    // que echar mano de él
    for (ki=0; ki<4; ki++)
      {
	if (angulo_valido[ki])
	  angulos_aux_izq[ki] = angulos_brazo_izq[ki]; // ...actualiza los ángulos que están dentro de límites
      }
    
    // Comprobar si ha habido colisiones
    _pobs->limpiarLinks();
    linksMovidos(16, &_pobs->_links_sospechosos);
    this->changed();
    if (!_pobs->no_hay_colision)
      {
	par_colisionado = _pobs->par_colisionado;
	
	//      cout <<"COLISION entre " 
	//	   << this->_pares_colision[par_colisionado].first->trf->getName() 
	//	   <<" y " 
	//	   << this->_pares_colision[par_colisionado].second->trf->getName() << std::endl;
	
	this->cambiaColor(*(this->_pares_colision[par_colisionado].first));
	this->cambiaColor(*(this->_pares_colision[par_colisionado].second));	
      }
    else
      {
	par_colisionado = -1;
	// PRUEBA. TAMBIEN SACA ALTERNATIVAS, PA VER... PERO LAS SACA CON BOLAS 3D Y TO!!!! OOOOOMA MORENOOOOOO!!!
	/*alternatas = this->alternativasBrazoIzq(alter);   
	for (int kk=0;kk<alter;kk++)
	{
	if (primen){
	abolas[kk] = mostrarAlternativa(alternatas[kk]);
	jCintura.trf->addChild( abolas[kk] );
	}
	else{
	abolas[kk]->setMatrix(osg::Matrix::translate(alternatas[kk]));
	}
	}
	primen = false;*/
      }
    if ( (par_colisionado!=-1)||(fuera_limites) ) // Si la posición no es válida...
      {
	// ...busca posiciones alternativas a aquella que está dando problemas	    
	encuentra_alternativa = false;
	alternatas = this->alternativasBrazoIzq (alter);
	// Prueba las alternativas, empezando desde la más cercana, hasta que encuentra una que no produce problemas
	for (int kk=0;kk<alter;kk++)
	  {
	    // Recoloca el brazo según la posición alternativa del codo
	    angulos_brazo_izq = apuntaBrazoIzq(alternatas[kk], manoizqnorm);
	    
	    // Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
	    // Comprobar límites
	    fuera_limites_alt = false;
	    if ( !enlimites(16, angulos_brazo_izq[0]*180/PI) ) fuera_limites_alt = true;
	    if ( !enlimites(17, angulos_brazo_izq[1]*180/PI) ) fuera_limites_alt = true;
	    if ( !enlimites(18, angulos_brazo_izq[2]*180/PI) ) fuera_limites_alt = true;
	    if ( !enlimites(19, angulos_brazo_izq[3]*180/PI) ) fuera_limites_alt = true;
	    // Comprobar si ha habido colisiones
	    _pobs->limpiarLinks();
	    linksMovidos(16, &_pobs->_links_sospechosos);
	    this->changed();
	    if ( (_pobs->no_hay_colision)&&(!fuera_limites_alt) )
	      {
		encuentra_alternativa = true;
		break; //sale del bucle for, con lo que la última alternativa es la que finalmente se selecciona
	      }
	  }
	// Si no hay alternativas, se vuelve a la última válida conocida
	if (!encuentra_alternativa)
	  {
	    // Se indica que no se pudo alcanzar la posición
	    result = false;
	    
	    // Si el problema es por una salida de límites, se actualizan los ángulos que no se salieron
	    // Así, al menos, el movimiento continúa aunque no siga perfectamente el movimiento deseado
	    if (fuera_limites)  
	      {
		// A ver si depuramos esto, oño
		for (ki=0; ki<4; ki++)
		  angulos_brazo_izq[ki] = angulos_aux_izq[ki];
		
		giraBrazoIzq(angulos_brazo_izq);
		
	      }
	    else
	      {
		// ... si no hay por donde pillarlo, se vuelve a la última posición válida
		this->jBrazoIzq.trf->setMatrix( ultimaBrazo->getMatrix() );
		this->jAntebrazoIzq.trf->setMatrix( ultimaAntebrazo->getMatrix() );
	      }
	  }
      }
    // Si estamos dentro de los límites, se miran sólo las dos alternativas más cercanas, buscando una posición
    // más baja (más "relajada y natural") para el codo
    else
      {
	if (fabs(angulos_brazo_izq[3])>0.1)
	  {
	    
	    // Salva la posición recién calculada, que a fin de cuentas es válida y probablemente haya que volver a ella
	    ultimaBrazo->setMatrix( this->jBrazoIzq.trf->getMatrix() );
	    ultimaAntebrazo->setMatrix( this->jAntebrazoIzq.trf->getMatrix() );
	    
	    // ...busca posiciones alternativas a la actual	    
	    encuentra_alternativa = false;
	    alternatas = this->alternativasBrazoIzq (alter);
	    // Prueba las dos alternativas más cercanas
	    Vec3d codo = this->jBrazoIzq.dirsegmento;
	    codo = codo * computeLocalToWorld(this->jBrazoIzq.nodepath);
	    int alternata_def;
	    if (alternatas[0].z()<alternatas[1].z()) alternata_def = 0;
	    else alternata_def = 1;
	    
	    // Si la alternativa escogida da una posición con el codo más bajo...
	    if (alternatas[alternata_def].z()<codo.z())
	      {
		// Recoloca el brazo según la posición alternativa del codo
		angulos_brazo_izq = apuntaBrazoIzq(alternatas[alternata_def], manoizqnorm);
		
		// Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
		// Comprobar límites
		fuera_limites_alt = false;
		if ( !enlimites(16, angulos_brazo_izq[0]*180/PI) ) fuera_limites_alt = true;
		if ( !enlimites(17, angulos_brazo_izq[1]*180/PI) ) fuera_limites_alt = true;
		if ( !enlimites(18, angulos_brazo_izq[2]*180/PI) ) fuera_limites_alt = true;
		if ( !enlimites(19, angulos_brazo_izq[3]*180/PI) ) fuera_limites_alt = true;
		// Comprobar si ha habido colisiones
		_pobs->limpiarLinks();
		linksMovidos(16, &_pobs->_links_sospechosos);
		this->changed();
		if ( (_pobs->no_hay_colision)&&(!fuera_limites_alt) )    
		  {
		    encuentra_alternativa = true;
		  }
	      }
	    // Si no hay alternativas, se vuelve a la última válida conocida
	    if (!encuentra_alternativa)
	      {
		this->jBrazoIzq.trf->setMatrix( ultimaBrazo->getMatrix() );
		this->jAntebrazoIzq.trf->setMatrix( ultimaAntebrazo->getMatrix() );
	      }
	  }
      }

    // Antes de volver, se deshace el cambio de escala
    cambiarTamanio(_scale,jCintura);

    return(result);
    
  }

  bool modelobasicopersona::mueveBrazoDer(const osg::Vec3d& manoder, const osg::Vec3d& cododer)
  {
    int ki;
    bool angulo_valido[4];
    std::vector<double> angulos_brazo_der(4);
    std::vector<double> angulos_aux_der(4);
    std::vector<osg::Vec3d> alternatas(30);
    int alter = 20;
    int par_colisionado = -1;

    bool result = true;
    bool fuera_limites = false;
    bool fuera_limites_alt = false;
    bool encuentra_alternativa;
    MatrixTransform* ultimaBrazo = new MatrixTransform;
    MatrixTransform* ultimaAntebrazo = new MatrixTransform;

    // Pasos previos para considerar los cambios de escala. En primer lugar, se fija la escala del modelo a 1...
    cambiarTamanio(1.0,jCintura);
    // ...en segunda lugar, se aplica el factor de escala inverso al punto 3D que se quiere alcanzar
    Vec3d manodernorm(manoder);
    manodernorm = manodernorm - jCintura.trf->getMatrix().getTrans();
    manodernorm /= _scale;
    manodernorm = manodernorm + jCintura.trf->getMatrix().getTrans();
    
    // Lo primero, comprobar que hay un observador de colisiones
    assert(_pobs!=NULL);
    
    // Salva la posición anterior, por si es necesario volver a ella
    ultimaBrazo->setMatrix( this->jBrazoDer.trf->getMatrix() );
    ultimaAntebrazo->setMatrix( this->jAntebrazoDer.trf->getMatrix() );
    for (ki=0; ki<4; ki++)
      {
	angulo_valido[ki] = true;
	angulos_aux_der[ki] = angulos_brazo_der[ki];
      }
    
    // Calcula nuevos ángulos. Según tengamos pose estimada para el codo o no, se utiliza dicha información para situar el codo
    if (cododer.length2()==0)
      angulos_brazo_der = apuntaIKBrazoDerAn(manodernorm);
    else
      angulos_brazo_der = apuntaBrazoDer(cododer, manodernorm);
    
    // Comprobar límites
    if ( !enlimites(6, angulos_brazo_der[0]*180/PI) ) 
      {angulo_valido[0] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT1"<<endl;}
    if ( !enlimites(7, angulos_brazo_der[1]*180/PI) ) 
      {angulo_valido[1] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT2"<<endl;}
    if ( !enlimites(8, angulos_brazo_der[2]*180/PI) ) 
      {angulo_valido[2] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT3"<<endl;}
    if ( !enlimites(9, angulos_brazo_der[3]*180/PI) ) 
      {angulo_valido[3] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT4"<<endl;}
    
    // Se actualiza el buffer de ángulos auxiliares, por si no hubiera alternativa después y hubiera
    // que echar mano de él
    for (ki=0; ki<4; ki++)
      {
	if (angulo_valido[ki])
	  angulos_aux_der[ki] = angulos_brazo_der[ki]; // ...actualiza los ángulos que están dentro de límites
      }
    
    // Comprobar si ha habido colisiones
    _pobs->limpiarLinks();
    linksMovidos(6, &_pobs->_links_sospechosos);
    this->changed();
    if (!_pobs->no_hay_colision)
      {
	par_colisionado = _pobs->par_colisionado;
	
	//      cout <<"COLISION entre " 
	//	   << this->_pares_colision[par_colisionado].first->trf->getName() 
	//	   <<" y " 
	//	   << this->_pares_colision[par_colisionado].second->trf->getName() << std::endl;
	
	this->cambiaColor(*(this->_pares_colision[par_colisionado].first));
	this->cambiaColor(*(this->_pares_colision[par_colisionado].second));	
      }
    else
      {
	par_colisionado = -1;
	// PRUEBA. TAMBIEN SACA ALTERNATIVAS, PA VER... PERO LAS SACA CON BOLAS 3D Y TO!!!! OOOOOMA MORENOOOOOO!!!
	/*alternatas = this->alternativasBrazoDerHOAP (alter);   
	  for (int kk=0;kk<alter;kk++)
	  {
	  if (primen){
	  bolas[kk] = mostrarAlternativa(alternatas[kk]);
	  escena->addChild( bolas[kk] );
	  }
	  else{
	  bolas[kk]->setMatrix(osg::Matrix::translate(alternatas[kk]));
	  }
	  }
	  primen = false;*/
      }
//     // Si ha habido colisión o nos hemos salido de algún límite, se buscan posiciones alternativas
//     if ( (par_colisionado!=-1)||(fuera_limites) )
//       {
// 	// ...busca posiciones alternativas a aquella que está dando problemas	    
// 	encuentra_alternativa = false;
// 	alternatas = this->alternativasBrazoDer (alter);
// 	// Prueba las alternativas, empezando desde la más cercana, hasta que encuentra una que no produce problemas
// 	for (int kk=0;kk<alter;kk++)
// 	  {
// 	    // Recoloca el brazo según la posición alternativa del codo
// 	    angulos_brazo_der = apuntaBrazoDer(alternatas[kk], manodernorm);
// 	    
// 	    // Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
// 	    // Comprobar límites
// 	    fuera_limites_alt = false;
// 	    if ( !enlimites(6, angulos_brazo_der[0]*180/PI) ) fuera_limites_alt = true;
// 	    if ( !enlimites(7, angulos_brazo_der[1]*180/PI) ) fuera_limites_alt = true;
// 	    if ( !enlimites(8, angulos_brazo_der[2]*180/PI) ) fuera_limites_alt = true;
// 	    if ( !enlimites(9, angulos_brazo_der[3]*180/PI) ) fuera_limites_alt = true;
// 	    // Comprobar si ha habido colisiones
// 	    _pobs->limpiarLinks();
// 	    linksMovidos(6, &_pobs->_links_sospechosos);
// 	    this->changed();
// 	    if ( (_pobs->no_hay_colision)&&(!fuera_limites_alt) )    
// 	      {
// 		encuentra_alternativa = true;
// 		break; //sale del bucle for, con lo que la última alternativa es la que finalmente se selecciona
// 	      }
// 	  }
// 	// Si no hay alternativas, se vuelve a la última válida conocida
// 	if (!encuentra_alternativa)
// 	  {
// 	    // Se indica que no se pudo alcanzar la posición
// 	    result = false;
// 	    
// 	    // Si el problema es por una salida de límites, se actualizan los ángulos que no se salieron
// 	    // Así, al menos, el movimiento continúa aunque no siga perfectamente el movimiento deseado
// 	    if (fuera_limites)
// 	      {
// 		for (ki=0; ki<4; ki++)
// 		  angulos_brazo_der[ki] = angulos_aux_der[ki];
// 		
// 		giraBrazoDer(angulos_brazo_der);
// 		
// 	      }
// 	    else
// 	      {
// 		// ... si no hay por donde pillarlo, se vuelve a la última posición válida
// 		this->jBrazoDer.trf->setMatrix( ultimaBrazo->getMatrix() );
// 		this->jAntebrazoDer.trf->setMatrix( ultimaAntebrazo->getMatrix() );
// 	      }
// 	  }
//       }
    // Si estamos dentro de los límites, se miran sólo las dos alternativas más cercanas, buscando una posición
    // más baja (más "relajada y natural") para el codo
//     else
//       {
// 	if (fabs(angulos_brazo_der[3])>0.1)
// 	  {
// 	    
// 	    // Salva la posición recién calculada, que a fin de cuentas es válida y probablemente haya que volver a ella
// 	    ultimaBrazo->setMatrix( this->jBrazoDer.trf->getMatrix() );
// 	    ultimaAntebrazo->setMatrix( this->jAntebrazoDer.trf->getMatrix() );
// 	    
// 	    // ...busca posiciones alternativas a la actual	    
// 	    encuentra_alternativa = false;
// 	    alternatas = this->alternativasBrazoDer (alter);
// 	    // Prueba las dos alternativas más cercanas
// 	    Vec3d codo = this->jBrazoDer.dirsegmento;
// 	    codo = codo * computeLocalToWorld(this->jBrazoDer.nodepath);
// 	    int alternata_def;
// 	    if (alternatas[0].z()<alternatas[1].z()) alternata_def = 0;
// 	    else alternata_def = 1;
// 	    
// 	    // Si la alternativa escogida da una posición con el codo más bajo...
// 	    if (alternatas[alternata_def].z()<codo.z())
// 	      {
// 		// Recoloca el brazo según la posición alternativa del codo
// 		angulos_brazo_der = apuntaBrazoDer(alternatas[alternata_def], manodernorm);
// 		
// 		// Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
// 		// Comprobar límites
// 		fuera_limites_alt = false;
// 		if ( !enlimites(6, angulos_brazo_der[0]*180/PI) ) fuera_limites_alt = true;
// 		if ( !enlimites(7, angulos_brazo_der[1]*180/PI) ) fuera_limites_alt = true;
// 		if ( !enlimites(8, angulos_brazo_der[2]*180/PI) ) fuera_limites_alt = true;
// 		if ( !enlimites(9, angulos_brazo_der[3]*180/PI) ) fuera_limites_alt = true;
// 		// Comprobar si ha habido colisiones
// 		_pobs->limpiarLinks();
// 		linksMovidos(6, &_pobs->_links_sospechosos);
// 		this->changed();
// 		if ( (_pobs->no_hay_colision)&&(!fuera_limites_alt) )    
// 		  {
// 		    encuentra_alternativa = true;
// 		  }
// 	      }
// 	    // Si no hay alternativas, se vuelve a la última válida conocida
// 	    if (!encuentra_alternativa)
// 	      {
// 		this->jBrazoDer.trf->setMatrix( ultimaBrazo->getMatrix() );
// 		this->jAntebrazoDer.trf->setMatrix( ultimaAntebrazo->getMatrix() );
// 	      }
// 	  }
//       }

    // Antes de volver, se deshace el cambio de escala
    cambiarTamanio(_scale,jCintura);

    return(result);
    
  }



}
