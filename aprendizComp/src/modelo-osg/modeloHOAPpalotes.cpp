// -*-c++-*-
// Esta clase es idéntica a 'modeloHOAP', pero emplea un modelo de palotes (sticky figure) en lugar de un modelo 
// de mallas 3D 

#include <osgDB/ReadFile>
#include <string>
#include <cassert>
#include <algorithm>
#include <modelo-osg/modeloHOAPpalotes.h>
#include <modelo-osg/utils.h>

using namespace std;

namespace osgDTE {

  void modeloHOAPpalotes::crearsegmentos()
  {
    // Representar ZMP del HOAPpalotes
    osg::Sphere* unitSphere = new osg::Sphere(osg::Vec3(0,0,0), 5.0);
    osg::ShapeDrawable* unitSphereDrawable = new osg::ShapeDrawable(unitSphere);

    EsferaZMP = new osg::MatrixTransform();
    EsferaZMP->setMatrix(osg::Matrix::translate(ZMP.x(),ZMP.y(),ZMP.z()));
    this->addChild(EsferaZMP);
    osg::Geode* unitSphereGeode = new osg::Geode();
    EsferaZMP->addChild(unitSphereGeode);
    unitSphereDrawable->setColor(Vec4(0.0,1.0,0.0,1.0));
    unitSphereGeode->addDrawable(unitSphereDrawable);

    // Cintura
    jCintura.trf->setName("jCintura");
    jCintura.trf->setMatrix(osg::Matrix::translate(0.0,0.0,237.0));
    this->addChild(jCintura.trf.get());
    osg::ref_ptr<osg::Node> node = cajaGrupo(osg::Vec3d(0.0f,0.0f,15.25f),2.5f,2.5f,30.5f,ROJO);
    if (node.valid()) jCintura.trf->addChild(node.get());
    jCintura.dirsegmento.set(0.0f, -2.0f, 30.5f);
    jCintura.nodepath.push_back(jCintura.trf.get());

    jCintura.trf->addChild(crearEjeCoordenadas(8.0));

    // Centro de masas
    jCintura.masa = 2.3674;
    jCintura.posCMlocales = Vec3d(0.0,27.0,114.0);

    // Esto es necesario para que, cuando se modifique el tamanio del
    // modelo, no se oscurezca o se aclare su color.
    jCintura.trf->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);


    // Pecho
    jPecho.trf->setName("jPecho");
    jPecho.trf->setMatrix(osg::Matrix::translate(0.0f, -2.0f, 30.0f));
    jCintura.trf->addChild(jPecho.trf.get());
    node = crearT(osg::Vec3d(0.0f,0.0f,0.0f),
		  osg::Vec3d(90.0f,0.0f,120.0f),
		  osg::Vec3d(-90.0f,0.0f,120.0f),
		  2.5f, ROJO);
    if (node.valid()) {
      jPecho.trf->addChild(node.get());
    }
    jPecho.dirsegmento.set(0.0f, 0.0f, 145.5f);
    jPecho.nodepath.push_back(jCintura.trf.get());
    jPecho.nodepath.push_back(jPecho.trf.get());  

    // Centro de masas (la masa del pecho está incluida en la cintura)
    jPecho.masa = 0.0;
    jPecho.posCMlocales = Vec3d(0.0,0.0,0.0);

    // Cabeza
    jCabeza.trf->setName("jCabeza");
    jCabeza.trf->setMatrix(osg::Matrix::translate(0.0f, 0.0f, 145.5f));
    jPecho.trf->addChild(jCabeza.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,20.0f),2.5f,2.5f,40.0f,ROJO);
    if (node.valid()) jCabeza.trf->addChild(node.get());
    jCabeza.dirsegmento.set(0.0f,0.0f,1.0f); // OJO: ESTE NO LO SE, PONGo PARRIBA
    jCabeza.nodepath.push_back(jCintura.trf.get());
    jCabeza.nodepath.push_back(jPecho.trf.get());
    jCabeza.nodepath.push_back(jCabeza.trf.get());

    jCabeza.trf->addChild(crearEjeCoordenadas(8.0));

    // Centro de masas (la masa de la cabeza está incluida en la cintura)
    jCabeza.masa = 0.0;
    jCabeza.posCMlocales = Vec3d(0.0,0.0,0.0);

    // Brazo izquierdo
    jBrazoIzq.trf->setName("jBrazoIzq");
    jBrazoIzq.trf->setMatrix(osg::Matrix::translate(90.0f,0.0f,120.0f));
    jPecho.trf->addChild(jBrazoIzq.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,57.5f),2.5f,2.5f,115.0f,ROJO);
    if (node.valid()) jBrazoIzq.trf->addChild(node.get());
    jBrazoIzq.dirsegmento.set(0.0f,0.0f,115.0f); // ojo, igual falta un desplazamiento (x=-10 en JP's)  
    jBrazoIzq.nodepath.push_back(jCintura.trf.get());
    jBrazoIzq.nodepath.push_back(jPecho.trf.get());  
    jBrazoIzq.nodepath.push_back(jBrazoIzq.trf.get());

    // Centro de masas
    jBrazoIzq.masa = 0.314;
    jBrazoIzq.posCMlocales = Vec3d(-27.0,-16.2,6.7);

    jBrazoIzq.trf->addChild(crearEjeCoordenadas(8.0));

    // Antebrazo izquierdo
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::translate(0.0f,0.0f,115.0f));
    jAntebrazoIzq.trf->setName("jAntebrazoIzq");
    jBrazoIzq.trf->addChild(jAntebrazoIzq.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,36.0f),2.5f,2.5f,72.0f,ROJO);
    if (node.valid()) jAntebrazoIzq.trf->addChild(node.get());
    jAntebrazoIzq.dirsegmento.set(0.0f,0.0f,72.0f); // OJO ESTE NO COINCIDE CON EL DEL HOAPpalotes (hay que retocar el modelo 3D)...
    jAntebrazoIzq.nodepath.push_back(jCintura.trf.get());
    jAntebrazoIzq.nodepath.push_back(jPecho.trf.get());
    jAntebrazoIzq.nodepath.push_back(jBrazoIzq.trf.get());
    jAntebrazoIzq.nodepath.push_back(jAntebrazoIzq.trf.get());

    // Centro de masas
    jAntebrazoIzq.masa = 0.121;
    jAntebrazoIzq.posCMlocales = Vec3d(0.0,0.0,-22.6);

    jAntebrazoIzq.trf->addChild(crearEjeCoordenadas(8.0));

    // Brazo derecho
    jBrazoDer.trf->setName("jBrazoDer");
    jBrazoDer.trf->setMatrix(//osg::Matrix::rotate(osg::inDegrees(180.0f),0.0f,1.0f,0.0f) *
			     osg::Matrix::translate(-90.0f,0.0f,120.0f));
    jPecho.trf->addChild(jBrazoDer.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,57.5f),2.5f,2.5f,115.0f,ROJO);
    if (node.valid()) jBrazoDer.trf->addChild(node.get());
    jBrazoDer.dirsegmento.set(0.0f,0.0f,115.0f); 
    jBrazoDer.nodepath.push_back(jCintura.trf.get());
    jBrazoDer.nodepath.push_back(jPecho.trf.get());

    jBrazoDer.nodepath.push_back(jBrazoDer.trf.get());

    // Centro de masas
    jBrazoDer.masa = 0.314;
    jBrazoDer.posCMlocales = Vec3d(27.0,-16.2,6.7);

    jBrazoDer.trf->addChild(crearEjeCoordenadas(8.0));

    // Antebrazo derecho
    jAntebrazoDer.trf->setMatrix(osg::Matrix::rotate(osg::inDegrees(180.0), 0.0f, 0.0f, 1.0f) * osg::Matrix::translate(0.0f,0.0f,115.0f));
    jAntebrazoDer.trf->setName("jAntebrazoDer");
    jBrazoDer.trf->addChild(jAntebrazoDer.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,36.0f),2.5f,2.5f,72.0f,ROJO);
    if (node.valid()) jAntebrazoDer.trf->addChild(node.get());
    jAntebrazoDer.dirsegmento.set(0.0f,0.0f,72.0f); // OJO ESTE NO COINCIDE CON EL DEL HOAPpalotes (hay que retocar el modelo 3D)...
    jAntebrazoDer.nodepath.push_back(jCintura.trf.get());
    jAntebrazoDer.nodepath.push_back(jPecho.trf.get());
    jAntebrazoDer.nodepath.push_back(jBrazoDer.trf.get());
    jAntebrazoDer.nodepath.push_back(jAntebrazoDer.trf.get());

    // Centro de masas
    jAntebrazoDer.masa = 0.121;
    jAntebrazoDer.posCMlocales = Vec3d(0.0,0.0,-22.6);

    jAntebrazoDer.trf->addChild(crearEjeCoordenadas(8.0));

    // Muslo Izquierdo
    jMusloIzq.trf->setMatrix(osg::Matrix::rotate(osg::inDegrees(180.0), 1.0, 0.0, 0.0 ) * osg::Matrix::translate(39.0, 0.0, 0.0));
    jMusloIzq.trf->setName("jMusloIzq");
    jCintura.trf->addChild(jMusloIzq.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,48.5f),2.5f,2.5f,97.0f,ROJO);
    if (node.valid()) jMusloIzq.trf->addChild(node.get());
    jMusloIzq.dirsegmento.set(0.0, 0.0, 97.0);
    jMusloIzq.nodepath.push_back(jCintura.trf.get());
    jMusloIzq.nodepath.push_back(jMusloIzq.trf.get());

    jMusloIzq.trf->addChild(crearEjeCoordenadas(8.0));

    // Centro de masas
    jMusloIzq.masa = 0.717;
    jMusloIzq.posCMlocales = Vec3d(0.7957,4.26,48.1);

    // Pantorrilla Izquierda
    jPantorrillaIzq.trf->setMatrix(osg::Matrix::translate(0.0, 0.0, 97.0));
    jPantorrillaIzq.trf->setName("jPantorrillaIzq");
    jMusloIzq.trf->addChild(jPantorrillaIzq.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,50.0f),2.5f,2.5f,100.0f,ROJO);
    if (node.valid()) jPantorrillaIzq.trf->addChild(node.get());
    jPantorrillaIzq.dirsegmento.set(0.0, 0.0, 0.0); // OJO OJO OJO 
    jPantorrillaIzq.nodepath.push_back(jCintura.trf.get());
    jPantorrillaIzq.nodepath.push_back(jMusloIzq.trf.get());
    jPantorrillaIzq.nodepath.push_back(jPantorrillaIzq.trf.get());

    jPantorrillaIzq.trf->addChild(crearEjeCoordenadas(8.0));

    // Centro de masas
    jPantorrillaIzq.masa = 0.286;
    jPantorrillaIzq.posCMlocales = Vec3d(-0.4,10.6,42.6);

    // Pie Izquierdo
    jPieIzq.trf->setMatrix(osg::Matrix::rotate(inDegrees(180.0),0.0,0.0,1.0) * osg::Matrix::translate(0.0, 0.0, 100.0));
    jPieIzq.trf->setName("jPieIzq");
    jPantorrillaIzq.trf->addChild(jPieIzq.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,20.0f),2.5f,2.5f,40.0f,ROJO);
    if (node.valid()) jPieIzq.trf->addChild(node.get());
    jPieIzq.dirsegmento.set(0.0, 0.0, 0.0); 
    jPieIzq.nodepath.push_back(jCintura.trf.get());
    jPieIzq.nodepath.push_back(jMusloIzq.trf.get());
    jPieIzq.nodepath.push_back(jPantorrillaIzq.trf.get());
    jPieIzq.nodepath.push_back(jPieIzq.trf.get());

    jPieIzq.trf->addChild(crearEjeCoordenadas(8.0));

    // Centro de masas
    jPieIzq.masa = 0.321;
    jPieIzq.posCMlocales = Vec3d(-0.3,-3.1,8.4);

    // Muslo Derecho
    jMusloDer.trf->setMatrix(osg::Matrix::rotate(osg::inDegrees(180.0), 1.0, 0.0, 0.0 ) * osg::Matrix::translate(-39.0, 0.0, 0.0));
    jMusloDer.trf->setName("jMusloDer");
    jCintura.trf->addChild(jMusloDer.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,48.5f),2.5f,2.5f,97.0f,ROJO);
    if (node.valid()) jMusloDer.trf->addChild(node.get());
    jMusloDer.dirsegmento.set(0.0, 0.0, 97.0);
    jMusloDer.nodepath.push_back(jCintura.trf.get());
    jMusloDer.nodepath.push_back(jMusloDer.trf.get());

    jMusloDer.trf->addChild(crearEjeCoordenadas(8.0));

    // Centro de masas
    jMusloDer.masa = 0.717;
    jMusloDer.posCMlocales = Vec3d(-0.7957,4.26,48.1);

    // Pantorrilla Derecha
    jPantorrillaDer.trf->setMatrix(osg::Matrix::translate(0.0, 0.0, 97.0));
    jPantorrillaDer.trf->setName("jPantorrillaDer");
    jMusloDer.trf->addChild(jPantorrillaDer.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,50.0f),2.5f,2.5f,100.0f,ROJO);
    if (node.valid()) jPantorrillaDer.trf->addChild(node.get());
    jPantorrillaDer.dirsegmento.set(0.0, 0.0, 0.0); // OJO OJO OJO 
    jPantorrillaDer.nodepath.push_back(jCintura.trf.get());
    jPantorrillaDer.nodepath.push_back(jMusloDer.trf.get());
    jPantorrillaDer.nodepath.push_back(jPantorrillaDer.trf.get());

    jPantorrillaDer.trf->addChild(crearEjeCoordenadas(8.0));

    // Centro de masas
    jPantorrillaDer.masa = 0.286;
    jPantorrillaDer.posCMlocales = Vec3d(0.4,10.6,42.6);

    // Pie Derecho
    jPieDer.trf->setMatrix(osg::Matrix::rotate(inDegrees(180.0),0.0,0.0,1.0) * osg::Matrix::translate(0.0, 0.0, 100.0));
    jPieDer.trf->setName("jPieDer");
    jPantorrillaDer.trf->addChild(jPieDer.trf.get());
    node = cajaGrupo(osg::Vec3d(0.0f,0.0f,20.0f),2.5f,2.5f,40.0f,ROJO);
    if (node.valid()) jPieDer.trf->addChild(node.get());
    jPieDer.dirsegmento.set(0.0, 0.0, 0.0); 
    jPieDer.nodepath.push_back(jCintura.trf.get());
    jPieDer.nodepath.push_back(jMusloDer.trf.get());
    jPieDer.nodepath.push_back(jPantorrillaDer.trf.get());
    jPieDer.nodepath.push_back(jPieDer.trf.get());

    jPieDer.trf->addChild(crearEjeCoordenadas(8.0));

    // Centro de masas
    jPieDer.masa = 0.321;
    jPieDer.posCMlocales = Vec3d(0.3,-3.1,8.4);
  }
  
  // Función para acceder a un joint del modelo
  modelo::joint* modeloHOAPpalotes::accedeJoint(std::string nombre_joint)
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
      return &jAntebrazoIzq;
    if (nombre_joint == "jBrazoDer")
      return &jBrazoDer;
    if (nombre_joint == "jAntebrazoDer")
      return &jAntebrazoDer;
    if (nombre_joint == "jMusloDer")
      return &jMusloDer;
    if (nombre_joint == "jPantorrillaDer")
      return &jPantorrillaDer;
    if (nombre_joint == "jPieDer")
      return &jPieDer;
    if (nombre_joint == "jMusloIzq")
      return &jMusloIzq;
    if (nombre_joint == "jPantorrillaIzq")
      return &jPantorrillaIzq;
    if (nombre_joint == "jPieIzq")
      return &jPieIzq;
    
    cout<<"AVISO: No se ha encontrado el joint "<<nombre_joint<<" en el modelo del HOAPpalotes"<<endl;
    return 0;
  }
  
  void modeloHOAPpalotes::mueveModelo(const osg::Vec3d& donde)
  {
    jCintura.trf->setMatrix(osg::Matrix::translate(donde));
  }
  
  void modeloHOAPpalotes::reset()
  {
    jCintura.trf->setMatrix(osg::Matrix::translate(0.0,0.0,237.0));
    jPecho.trf->setMatrix(osg::Matrix::translate(0.0f,-2.0f, 30.0f));
    jCabeza.trf->setMatrix(osg::Matrix::translate(0.0f,0.0f,145.5f));  		 	
    jBrazoIzq.trf->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0f),1.0f,0.0f,0.0f) * 
			     osg::Matrix::translate(90.0f,0.0f,120.0f));
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::translate(0.0f,0.0f,115.0f));
    jBrazoDer.trf->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0f),1.0f,0.0f,0.0f) *
			     osg::Matrix::translate(-90.0f,0.0f,120.0f));
    jAntebrazoDer.trf->setMatrix(osg::Matrix::translate(0.0f,0.0f,115.0f));
  }

  /// Función para proporcionar una lista de links sospechosos de producir colisión dado el movimiento de un joint
  void modeloHOAPpalotes::linksMovidos(int i, std::vector<std::string>* lista)
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

      case 15:
      case 14:
	lista->push_back(jPieIzq.trf->getName());
	break;

      case 5:
      case 4:
	lista->push_back(jPieDer.trf->getName());
	break;

      case 13:
	lista->push_back(jPieIzq.trf->getName());
	lista->push_back(jPantorrillaIzq.trf->getName());
	break;
	
      case 3:
	lista->push_back(jPieDer.trf->getName());
	lista->push_back(jPantorrillaDer.trf->getName());
	break;

      case 12:
      case 11:
      case 10:
	lista->push_back(jPieIzq.trf->getName());
	lista->push_back(jPantorrillaIzq.trf->getName());
	lista->push_back(jMusloIzq.trf->getName());
	break;
	
      case 2:
      case 1:
      case 0:
	lista->push_back(jPieDer.trf->getName());
	lista->push_back(jPantorrillaDer.trf->getName());
	lista->push_back(jMusloDer.trf->getName());
	break;

      default:
	cerr<<"Índice de articulación no válido para el robot HOAPpalotes-1"<<endl;
	break;
      }

  }

  void modeloHOAPpalotes::limitesDefecto()
  {
    jMusloIzq.limiteeje[2].bajo = -30;   // LLEG_JOINT[1]
    jMusloIzq.limiteeje[2].alto = 90;
    jMusloIzq.limiteeje[1].bajo = -20;   // LLEG_JOINT[2]
    jMusloIzq.limiteeje[1].alto = 30;
    jMusloIzq.limiteeje[0].bajo = -115;  // LLEG_JOINT[3]
    jMusloIzq.limiteeje[0].alto = 70;

    jMusloDer.limiteeje[2].bajo = -90;   // RLEG_JOINT[1]
    jMusloDer.limiteeje[2].alto = 30;
    jMusloDer.limiteeje[1].bajo = -30;   // RLEG_JOINT[2]
    jMusloDer.limiteeje[1].alto = 20;
    jMusloDer.limiteeje[0].bajo = -115;  // RLEG_JOINT[3]
    jMusloDer.limiteeje[0].alto = 70;

    jPantorrillaIzq.limiteeje[2].bajo = 0;
    jPantorrillaIzq.limiteeje[2].alto = 0;
    jPantorrillaIzq.limiteeje[1].bajo = 0;
    jPantorrillaIzq.limiteeje[1].alto = 0;
    jPantorrillaIzq.limiteeje[0].bajo = 0;   // LLEG_JOINT[4]
    jPantorrillaIzq.limiteeje[0].alto = 120;

    jPantorrillaDer.limiteeje[2].bajo = 0;
    jPantorrillaDer.limiteeje[2].alto = 0;
    jPantorrillaDer.limiteeje[1].bajo = 0;
    jPantorrillaDer.limiteeje[1].alto = 0;
    jPantorrillaDer.limiteeje[0].bajo = 0;   // RLEG_JOINT[4]
    jPantorrillaDer.limiteeje[0].alto = 120;

    jPieIzq.limiteeje[2].bajo = 0;
    jPieIzq.limiteeje[2].alto = 0;
    jPieIzq.limiteeje[1].bajo = -18;   // LLEG_JOINT[6]
    jPieIzq.limiteeje[1].alto = 18;
    jPieIzq.limiteeje[0].bajo = -60;   // LLEG_JOINT[5]
    jPieIzq.limiteeje[0].alto = 60;

    jPieDer.limiteeje[2].bajo = 0;
    jPieDer.limiteeje[2].alto = 0;
    jPieDer.limiteeje[1].bajo = -18;   // RLEG_JOINT[6]
    jPieDer.limiteeje[1].alto = 18;
    jPieDer.limiteeje[0].bajo = -60;   // RLEG_JOINT[5]
    jPieDer.limiteeje[0].alto = 60;

    jBrazoIzq.limiteeje[2].bajo = 0;
    jBrazoIzq.limiteeje[2].alto = 0;
    jBrazoIzq.limiteeje[1].bajo = 0;    // LARM_JOINT[2]
    jBrazoIzq.limiteeje[1].alto = 95;
    jBrazoIzq.limiteeje[0].bajo = -90;  // LARM_JOINT[1]
    jBrazoIzq.limiteeje[0].alto = 150;

    jBrazoDer.limiteeje[2].bajo = 0;
    jBrazoDer.limiteeje[2].alto = 0;
    jBrazoDer.limiteeje[1].bajo = -95;  // RARM_JOINT[2]
    jBrazoDer.limiteeje[1].alto = 0;
    jBrazoDer.limiteeje[0].bajo = -90;  // RARM_JOINT[1]
    jBrazoDer.limiteeje[0].alto = 150;

    jAntebrazoIzq.limiteeje[2].bajo = -90; // LARM_JOINT[3]
    jAntebrazoIzq.limiteeje[2].alto = 90;
    jAntebrazoIzq.limiteeje[1].bajo = 0;
    jAntebrazoIzq.limiteeje[1].alto = 0;
    jAntebrazoIzq.limiteeje[0].bajo = -90; // LARM_JOINT[4] 
    jAntebrazoIzq.limiteeje[0].alto = 1;

    jAntebrazoDer.limiteeje[2].bajo = -90; // RARM_JOINT[3]
    jAntebrazoDer.limiteeje[2].alto = 90;
    jAntebrazoDer.limiteeje[1].bajo = 0;
    jAntebrazoDer.limiteeje[1].alto = 0;
    jAntebrazoDer.limiteeje[0].bajo = -90; // RARM_JOINT[4]
    jAntebrazoDer.limiteeje[0].alto = 1;
  }

  // Función para fijar las colisiones por defecto
  void modeloHOAPpalotes::colisionesDefecto()
  {
    // No tiene sentido tener colisiones por defecto en una sticky figure
  }

//  // Función para acceder a un joint del modelo
//  joint modeloHOAPpalotes::accedeJoint(std::string nombre_joint)
//  {
//    if (nombre_joint == "jCintura")
//      return jCintura;
//    if (nombre_joint =="jPecho")
//      return jPecho;
//    if (nombre_joint == "jCabeza")
//      return jCabeza;
//    if (nombre_joint == "jBrazoIzq")
//      return jBrazoIzq;
//    if (nombre_joint == "jAntebrazoIzq")
//      return jBrazoIzq;
//    if (nombre_joint == "jBrazoDer")
//      return jBrazoDer;
//    if (nombre_joint == "jAntebrazoDer")
//      return jAntebrazoDer;
//    if (nombre_joint == "jMusloDer")
//      return jMusloDer;
//    if (nombre_joint == "jPantorrillaDer")
//      return jPantorrillaDer;
//    if (nombre_joint == "jPieDer")
//      return jPieDer;
//    if (nombre_joint == "jMusloIzq")
//      return jMusloIzq;
//    if (nombre_joint == "jPantorrillaIzq")
//      return jPantorrillaIzq;
//    if (nombre_joint == "jPieIzq")
//      return jPieIzq;
//    
//    cout<<"AVISO: No se ha encontrado el joint "<<nombre_joint<<" en el modelo del HOAPpalotes"<<endl;
//    return NULL;
//  }
  
  // Comprobación de ángulos para el modelo de HOAPpalotes
  double modeloHOAPpalotes::enlimitesInt(int articulacion, double angulo)
  {
    switch (articulacion)
      {
      case 6:
	if (angulo<jBrazoDer.limiteeje[0].bajo) return ((jBrazoDer.limiteeje[0].bajo)*PI/180);
        if (angulo>jBrazoDer.limiteeje[0].alto) return ((jBrazoDer.limiteeje[0].alto)*PI/180);
	  
	break;
      case 7:
	if (angulo<jBrazoDer.limiteeje[1].bajo) return ((jBrazoDer.limiteeje[1].bajo)*PI/180);
        if (angulo>jBrazoDer.limiteeje[1].alto) return ((jBrazoDer.limiteeje[1].alto)*PI/180);
	  
	break;
      case 8:
	if (angulo<jAntebrazoDer.limiteeje[2].bajo) return ((jAntebrazoDer.limiteeje[2].bajo)*PI/180);
        if (angulo>jAntebrazoDer.limiteeje[2].alto) return ((jAntebrazoDer.limiteeje[2].alto)*PI/180);
	  
	break;
      case 9:
	if (angulo<jAntebrazoDer.limiteeje[0].bajo) return ((jAntebrazoDer.limiteeje[0].bajo)*PI/180);
        if (angulo>jAntebrazoDer.limiteeje[0].alto) return ((jAntebrazoDer.limiteeje[0].alto)*PI/180);
	  
	break;

      case 16:
	if (angulo<jBrazoIzq.limiteeje[0].bajo) return ((jBrazoIzq.limiteeje[0].bajo)*PI/180);
        if (angulo>jBrazoIzq.limiteeje[0].alto) return ((jBrazoIzq.limiteeje[0].alto)*PI/180);
	  
	break;
      case 17:
	if (angulo<jBrazoIzq.limiteeje[1].bajo) return ((jBrazoIzq.limiteeje[1].bajo)*PI/180);
        if (angulo>jBrazoIzq.limiteeje[1].alto) return ((jBrazoIzq.limiteeje[1].alto)*PI/180);
	  
	break;
      case 18:
	if (angulo<jAntebrazoIzq.limiteeje[2].bajo) return ((jAntebrazoIzq.limiteeje[2].bajo)*PI/180);
        if (angulo>jAntebrazoIzq.limiteeje[2].alto) return ((jAntebrazoIzq.limiteeje[2].alto)*PI/180);
	  
	break;
      case 19:
	if (angulo<jAntebrazoIzq.limiteeje[0].bajo) return ((jAntebrazoIzq.limiteeje[0].bajo)*PI/180);
        if (angulo>jAntebrazoIzq.limiteeje[0].alto) return ((jAntebrazoIzq.limiteeje[0].alto)*PI/180);
	  
	break;
	
      default:
	cerr<<"modeloHOAPpalotes: Indice de articulacion no valido"<<endl; 
	break;
      } 

    return 1000;
  }

  // Comprobación de ángulos para el HOAPpalotes
  bool modeloHOAPpalotes::enlimites(int articulacion, double angulo)
  {
    switch (articulacion)
      {
      case 0:
	if ( (angulo<jMusloDer.limiteeje[2].bajo)||(angulo>jMusloDer.limiteeje[2].alto) )
	  return false;
	break;
      case 1:
	if ( (angulo<jMusloDer.limiteeje[1].bajo)||(angulo>jMusloDer.limiteeje[1].alto) )
	  return false;
	break;
      case 2:
	if ( (angulo<jMusloDer.limiteeje[0].bajo)||(angulo>jMusloDer.limiteeje[0].alto) )
	  return false;
	break;
      case 3:
	if ( (angulo<jPantorrillaDer.limiteeje[0].bajo)||(angulo>jPantorrillaDer.limiteeje[0].alto) )
	  return false;
	break;
      case 4:
	if ( (angulo<jPieDer.limiteeje[0].bajo)||(angulo>jPieDer.limiteeje[0].alto) )
	  return false;
	break;
      case 5:
	if ( (angulo<jPieDer.limiteeje[1].bajo)||(angulo>jPieDer.limiteeje[1].alto) )
	  return false;
	break;
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

      case 10:
	if ( (angulo<jMusloIzq.limiteeje[2].bajo)||(angulo>jMusloIzq.limiteeje[2].alto) )
	  return false;
	break;
      case 11:
	if ( (angulo<jMusloIzq.limiteeje[1].bajo)||(angulo>jMusloIzq.limiteeje[1].alto) )
	  return false;
	break;
      case 12:
	if ( (angulo<jMusloIzq.limiteeje[0].bajo)||(angulo>jMusloIzq.limiteeje[0].alto) )
	  return false;
	break;
      case 13:
	if ( (angulo<jPantorrillaIzq.limiteeje[0].bajo)||(angulo>jPantorrillaIzq.limiteeje[0].alto) )
	  return false;
	break;
      case 14:
	if ( (angulo<jPieIzq.limiteeje[0].bajo)||(angulo>jPieIzq.limiteeje[0].alto) )
	  return false;
	break;
      case 15:
	if ( (angulo<jPieIzq.limiteeje[1].bajo)||(angulo>jPieIzq.limiteeje[1].alto) )
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
	cerr<<"Indice de articulacion no valido"<<endl; 
	break;
      }
    return true;
  }

  void modeloHOAPpalotes::calculaZMP(double intervalo)
  {
    // Capturar el tiempo que ha transcurrido desde el frame anterior y
    // actualizar posiciones, velocidades y aceleraciones
    actualizadatos(intervalo);
    
    double g;

    g = -9800; // mm/sg2
    
    // Fórmula del ZMP
    double denom;
    double anumx;
    double anumy;
    double bnumx;
    double bnumy;
    
    denom = jCintura.masa*(jCintura.aceCM.z() + g) + 
      jPecho.masa*(jPecho.aceCM.z() + g) +
      jCabeza.masa*(jCabeza.aceCM.z() + g) +
      jBrazoDer.masa*(jBrazoDer.aceCM.z() + g) +
      jAntebrazoDer.masa*(jAntebrazoDer.aceCM.z() + g) +
      jBrazoIzq.masa*(jBrazoIzq.aceCM.z() + g) +
      jAntebrazoIzq.masa*(jAntebrazoIzq.aceCM.z() + g) +
      jMusloDer.masa*(jMusloDer.aceCM.z() + g) +
      jPantorrillaDer.masa*(jPantorrillaDer.aceCM.z() + g) +
      jPieDer.masa*(jPieDer.aceCM.z() + g) +
      jMusloIzq.masa*(jMusloIzq.aceCM.z() + g) +
      jPantorrillaIzq.masa*(jPantorrillaIzq.aceCM.z() + g) +
      jPieIzq.masa*(jPieIzq.aceCM.z() + g);
    
    anumx = jCintura.masa*(jCintura.aceCM.z() + g)*jCintura.posCM.x() + 
      jPecho.masa*(jPecho.aceCM.z() + g)*jPecho.posCM.x() +
      jCabeza.masa*(jCabeza.aceCM.z() + g)*jCabeza.posCM.x() +
      jBrazoDer.masa*(jBrazoDer.aceCM.z() + g)*jBrazoDer.posCM.x() +
      jAntebrazoDer.masa*(jAntebrazoDer.aceCM.z() + g)*jAntebrazoDer.posCM.x() +
      jBrazoIzq.masa*(jBrazoIzq.aceCM.z() + g)*jBrazoIzq.posCM.x() +
      jAntebrazoIzq.masa*(jAntebrazoIzq.aceCM.z() + g)*jAntebrazoIzq.posCM.x() +
      jMusloDer.masa*(jMusloDer.aceCM.z() + g)*jMusloDer.posCM.x() +
      jPantorrillaDer.masa*(jPantorrillaDer.aceCM.z() + g)*jPantorrillaDer.posCM.x() +
      jPieDer.masa*(jPieDer.aceCM.z() + g)*jPieDer.posCM.x() +
      jMusloIzq.masa*(jMusloIzq.aceCM.z() + g)*jMusloIzq.posCM.x() +
      jPantorrillaIzq.masa*(jPantorrillaIzq.aceCM.z() + g)*jPantorrillaIzq.posCM.x() +
      jPieIzq.masa*(jPieIzq.aceCM.z() + g)*jPieIzq.posCM.x();
    
    anumy = jCintura.masa*(jCintura.aceCM.z() + g)*jCintura.posCM.y() + 
      jPecho.masa*(jPecho.aceCM.z() + g)*jPecho.posCM.y() +
      jCabeza.masa*(jCabeza.aceCM.z() + g)*jCabeza.posCM.y() +
      jBrazoDer.masa*(jBrazoDer.aceCM.z() + g)*jBrazoDer.posCM.y() +
      jAntebrazoDer.masa*(jAntebrazoDer.aceCM.z() + g)*jAntebrazoDer.posCM.y() +
      jBrazoIzq.masa*(jBrazoIzq.aceCM.z() + g)*jBrazoIzq.posCM.y() +
      jAntebrazoIzq.masa*(jAntebrazoIzq.aceCM.z() + g)*jAntebrazoIzq.posCM.y() +
      jMusloDer.masa*(jMusloDer.aceCM.z() + g)*jMusloDer.posCM.y() +
      jPantorrillaDer.masa*(jPantorrillaDer.aceCM.z() + g)*jPantorrillaDer.posCM.y() +
      jPieDer.masa*(jPieDer.aceCM.z() + g)*jPieDer.posCM.y() +
      jMusloIzq.masa*(jMusloIzq.aceCM.z() + g)*jMusloIzq.posCM.y() +
      jPantorrillaIzq.masa*(jPantorrillaIzq.aceCM.z() + g)*jPantorrillaIzq.posCM.y() +
      jPieIzq.masa*(jPieIzq.aceCM.z() + g)*jPieIzq.posCM.y();

    bnumx = jCintura.masa*jCintura.aceCM.x()*jCintura.posCM.z() + 
      jPecho.masa*jPecho.aceCM.x()*jPecho.posCM.z() +
      jCabeza.masa*jCabeza.aceCM.x()*jCabeza.posCM.z() +
      jBrazoDer.masa*jBrazoDer.aceCM.x()*jBrazoDer.posCM.z() +
      jAntebrazoDer.masa*jAntebrazoDer.aceCM.x()*jAntebrazoDer.posCM.z() +
      jBrazoIzq.masa*jBrazoIzq.aceCM.x()*jBrazoIzq.posCM.z() +
      jAntebrazoIzq.masa*jAntebrazoIzq.aceCM.x()*jAntebrazoIzq.posCM.z() +
      jMusloDer.masa*jMusloDer.aceCM.x()*jMusloDer.posCM.z() +
      jPantorrillaDer.masa*jPantorrillaDer.aceCM.x()*jPantorrillaDer.posCM.z() +
      jPieDer.masa*jPieDer.aceCM.x()*jPieDer.posCM.z() +
      jMusloIzq.masa*jMusloIzq.aceCM.x()*jMusloIzq.posCM.z() +
      jPantorrillaIzq.masa*jPantorrillaIzq.aceCM.x()*jPantorrillaIzq.posCM.z() +
      jPieIzq.masa*jPieIzq.aceCM.x()*jPieIzq.posCM.z();
    
    bnumy = jCintura.masa*jCintura.aceCM.y()*jCintura.posCM.z() + 
      jPecho.masa*jPecho.aceCM.y()*jPecho.posCM.z() +
      jCabeza.masa*jCabeza.aceCM.y()*jCabeza.posCM.z() +
      jBrazoDer.masa*jBrazoDer.aceCM.y()*jBrazoDer.posCM.z() +
      jAntebrazoDer.masa*jAntebrazoDer.aceCM.y()*jAntebrazoDer.posCM.z() +
      jBrazoIzq.masa*jBrazoIzq.aceCM.y()*jBrazoIzq.posCM.z() +
      jAntebrazoIzq.masa*jAntebrazoIzq.aceCM.y()*jAntebrazoIzq.posCM.z() +
      jMusloDer.masa*jMusloDer.aceCM.y()*jMusloDer.posCM.z() +
      jPantorrillaDer.masa*jPantorrillaDer.aceCM.y()*jPantorrillaDer.posCM.z() +
      jPieDer.masa*jPieDer.aceCM.y()*jPieDer.posCM.z() +
      jMusloIzq.masa*jMusloIzq.aceCM.y()*jMusloIzq.posCM.z() +
      jPantorrillaIzq.masa*jPantorrillaIzq.aceCM.y()*jPantorrillaIzq.posCM.z() +
      jPieIzq.masa*jPieIzq.aceCM.y()*jPieIzq.posCM.z();

    ZMP.x() = (anumx - bnumx)/denom;
    ZMP.y() = (anumy - bnumy)/denom;
    ZMP.z() = 0.0;
    
  }

  void modeloHOAPpalotes::actualizadatos(double intervalo)
  {
    // Actualiza posiciones globales de los segmentos
    jCintura.actualizadatosjoint(intervalo);
    jPecho.actualizadatosjoint(intervalo);
    jCabeza.actualizadatosjoint(intervalo);
    jBrazoIzq.actualizadatosjoint(intervalo);
    jAntebrazoIzq.actualizadatosjoint(intervalo);
    jBrazoDer.actualizadatosjoint(intervalo);
    jAntebrazoDer.actualizadatosjoint(intervalo);
    jMusloIzq.actualizadatosjoint(intervalo);
    jPantorrillaIzq.actualizadatosjoint(intervalo);
    jPieIzq.actualizadatosjoint(intervalo);
    jMusloDer.actualizadatosjoint(intervalo);
    jPantorrillaDer.actualizadatosjoint(intervalo);
    jPieDer.actualizadatosjoint(intervalo);
  }

  // Apuntar el brazo derecho utilizando método geométrico 
  //
  //  Ver tesis de Joel Mitchelson (2003)
  //
  // PARA EL HOAPpalotes-1
  // Devuelve un vector de cuatro flotantes con los ángulos para RARM_JOINT[1]...[4]
  std::vector<double> modeloHOAPpalotes::apuntaIKBrazoDerAn(const osg::Vec3d& donde)
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
    Vec3d trans1 =  jBrazoDer.trf->getMatrix().getTrans();
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

    // El ángulo que gira el codo es el ángulo que hay entre el vector (linea_brazo - codo) 
    // y el Eje Z del hombro, vector (0,0,1).
    // El giro en Z necesario para alinear el eje de giro del antebrazo con el eje X local 
    // del Antebrazo se ha calculado en la matriz de resultado parcial del hombro
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
				 osg::Matrix::translate(0.0f,0.0f,115.0f));

    jBrazoDer.trf->setMatrix(osg::Matrix::rotate(rarm_joint[1],0.0,1.0,0.0) *  // RARM_JOINT[2]
			     osg::Matrix::rotate(rarm_joint[0],1.0,0.0,0.0) *  // RARM_JOINT[1]
			     osg::Matrix::translate(-90.0,0.0,120.0));

    // OFFSETS PARA EL HOAPpalotes-1
    // Como los ejes de giro del HOAPpalotes son un poco distintos a los de coordenadas locales, y como
    // los ángulos iniciales también son distintos, es necesario utilizar offsets para adaptar 
    // los valores anteriores al HOAPpalotes-1
    rarm_joint[0] = rarm_joint[0] - PI/2;
    if (rarm_joint[0]<-PI) rarm_joint[0] += 2*PI;
    else if (rarm_joint[0]>PI) rarm_joint[0] -= 2*PI;

    return (rarm_joint);
  }
  
  // Apuntar el brazo izquierdo utilizando método geométrico 
  //
  //  Ver tesis de Joel Mitchelson (2003)
  //
  // PARA EL HOAPpalotes-1
  // Devuelve un vector de cuatro flotantes con los ángulos para LARM_JOINT[1]...[4]
  std::vector<double> modeloHOAPpalotes::apuntaIKBrazoIzqAn(const osg::Vec3d& donde)
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

    // El ángulo que gira el codo es el ángulo que hay entre el vector (linea_brazo - codo) 
    // y el Eje Z del hombro, vector (0,0,1).
    // El giro en Z necesario para alinear el eje de giro del antebrazo con el eje X local 
    // del Antebrazo se ha calculado en la matriz de resultado parcial del hombro
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
				 osg::Matrix::translate(0.0f,0.0f,115.0f));

    jBrazoIzq.trf->setMatrix(osg::Matrix::rotate(larm_joint[1],0.0,1.0,0.0) *  // LARM_JOINT[2]
			     osg::Matrix::rotate(larm_joint[0],1.0,0.0,0.0) *  // LARM_JOINT[1]
			     osg::Matrix::translate(90.0,0.0,120.0));

    // OFFSETS PARA EL HOAPpalotes-1
    // Como los ejes de giro del HOAPpalotes son un poco distintos a los de coordenadas locales, y como
    // los ángulos iniciales también son distintos, es necesario utilizar offsets para adaptar 
    // los valores anteriores al HOAPpalotes-1
    larm_joint[0] = larm_joint[0] - PI/2;
    if (larm_joint[0]<-PI) larm_joint[0] += 2*PI;
    else if (larm_joint[0]>PI) larm_joint[0] -= 2*PI;

    return(larm_joint);
  }

  // Función para obtener alternativas a la posición del codo derecho, para el HOAPpalotes-1.
  // Las alternativas se ordenarán de acuerdo a esta posición, siendo la densidad de 
  // alternativas mayor en la zona cercana al codo
  // ENTRADAS: 
  //   int opciones - Número de alternativas que va a ofrecer la función
  // SALIDAS:
  //   Un vector con las posibles posiciones GLOBALES 3D, ordenadas desde las más cercanas a la
  //   posición de referencia a las más alejadas
  std::vector<osg::Vec3d>  modeloHOAPpalotes::alternativasBrazoDer(int opciones, double angmax)
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

  // Función para obtener alternativas a la posición del codo izquierdo, para el HOAPpalotes-1.
  // ENTRADAS: 
  //   int opciones - Número de alternativas que va a ofrecer la función
  //   Vec3d ref    - Posición de referencia. Las alternativas se ordenarán de acuerdo
  //                  a dicha posición
  // SALIDAS:
  //   Un vector con las posibles posiciones 3D, ordenadas desde las más cercanas a la
  //   posición de referencia a las más alejadas
  std::vector<osg::Vec3d>  modeloHOAPpalotes::alternativasBrazoIzq(int opciones, double angmax)
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
  std::vector<double> modeloHOAPpalotes::apuntaBrazoDer(const osg::Vec3d& codo, const osg::Vec3d& donde)
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
    // RARM_JOINT[1] del NOMAD:
    rarm_joint[0] = atan2(-homb(2,1),homb(2,2));
    // RARM_JOINT[2] del NOMAD
    rarm_joint[1] = atan2(homb(2,0),sqrt(homb(0,0)*homb(0,0) + homb(1,0)*homb(1,0)));
    // RARM_JOINT[3] del NOMAD
    rarm_joint[2] = atan2(-homb(1,0),homb(0,0));
    // RARM_JOINT[4] del NOMAD
    rarm_joint[3] = anglo;

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoDer.trf->setMatrix(osg::Matrix::rotate(rarm_joint[3], 1.0, 0.0, 0.0) * // RARM_JOINT[4]
				 osg::Matrix::rotate(rarm_joint[2],0.0,0.0,1.0) *  // RARM_JOINT[3]	    
				 osg::Matrix::translate(0.0f,0.0f,115.0f));//30.5f));

    jBrazoDer.trf->setMatrix(osg::Matrix::rotate(rarm_joint[1],0.0,1.0,0.0) *  // RARM_JOINT[2]
			     osg::Matrix::rotate(rarm_joint[0],1.0,0.0,0.0) *  // RARM_JOINT[1]
			     osg::Matrix::translate(-90.0f,0.0f,120.0f));

    // Offsets
    rarm_joint[0] = rarm_joint[0] - PI/2;
    if (rarm_joint[0]<-PI) rarm_joint[0] += 2*PI;
    else if (rarm_joint[0]>PI) rarm_joint[0] -= 2*PI;

    return(rarm_joint);
  }

  // Función para apuntar el brazo izquierdo, dadas las posiciones en globales del codo y el punto que se quiere alcanzar con
  // la mano (Persona)
  std::vector<double> modeloHOAPpalotes::apuntaBrazoIzq(const osg::Vec3d& codo, const osg::Vec3d& donde)
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
    // LARM_JOINT[1] del NOMAD:
    larm_joint[0] = atan2(-homb(2,1),homb(2,2));
    // LARM_JOINT[2] del NOMAD
    larm_joint[1] = atan2(homb(2,0),sqrt(homb(0,0)*homb(0,0) + homb(1,0)*homb(1,0)));
    // LARM_JOINT[3] del NOMAD
    larm_joint[2] = atan2(-homb(1,0),homb(0,0));
    // LARM_JOINT[4] del NOMAD
    larm_joint[3] = anglo;

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::rotate(larm_joint[3],1.0,0.0,0.0) * // LARM_JOINT[4]
				 osg::Matrix::rotate(larm_joint[2],0.0,0.0,1.0) * // LARM_JOINT[3]	 
				 osg::Matrix::translate(0.0f,0.0f,115.0f));//30.5f));
    
    jBrazoIzq.trf->setMatrix(osg::Matrix::rotate(larm_joint[1],0.0,1.0,0.0) *  // LARM_JOINT[2]
			     osg::Matrix::rotate(larm_joint[0],1.0,0.0,0.0) *  // LARM_JOINT[1]
			     osg::Matrix::translate(90.0f,0.0f,120.0f));

    // Offsets
    larm_joint[0] = larm_joint[0] - PI/2;
    if (larm_joint[0]<-PI) larm_joint[0] += 2*PI;
    else if (larm_joint[0]>PI) larm_joint[0] -= 2*PI;

    return(larm_joint);
  }

  void modeloHOAPpalotes::giraBrazoIzq(const std::vector<double> & radianes)
  {
    assert(radianes.size() == 4);

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::rotate(radianes[3],1.0,0.0,0.0) *//LARMJOINT[4]
				 osg::Matrix::rotate(radianes[2],0.0,0.0,1.0) *//LARMJOINT[3] 
				 osg::Matrix::translate(0.0f,0.0f,115.0f));

    jBrazoIzq.trf->setMatrix(osg::Matrix::rotate(radianes[1],0.0,1.0,0.0) *  // LARMJOINT[2]
			     osg::Matrix::rotate(radianes[0],1.0,0.0,0.0) *  // LARMJOINT[1]
			     osg::Matrix::rotate(osg::inDegrees(90.0),1.0,0.0,0.0) *              
			     osg::Matrix::translate(90.0,0.0,120.0));
  }

  void modeloHOAPpalotes::giraBrazoDer(const std::vector<double> & radianes)
  {
    assert(radianes.size() == 4);

    // Aplica las transformaciones a las articulaciones de hombro y codo, actualizando sus 
    // matrices de transformación locales
    jAntebrazoDer.trf->setMatrix(osg::Matrix::rotate(radianes[3],1.0,0.0,0.0) *//RARMJOINT[4]
				 osg::Matrix::rotate(radianes[2],0.0,0.0,1.0) *//RARMJOINT[3] 
				 osg::Matrix::translate(0.0f,0.0f,115.0f));

    jBrazoDer.trf->setMatrix(osg::Matrix::rotate(radianes[1],0.0,1.0,0.0) *  // RARMJOINT[2]
			     osg::Matrix::rotate(radianes[0],1.0,0.0,0.0) *  // RARMJOINT[1]
			     osg::Matrix::rotate(osg::inDegrees(90.0),1.0,0.0,0.0) *              
			     osg::Matrix::translate(-90.0,0.0,120.0));

  }

  bool modeloHOAPpalotes::mueveCabeza(const osg::Vec3d& cabeza)
  {
    // Pasos previos para considerar los cambios de escala. En primer lugar, se fija la escala del modelo a 1...
    cambiarTamanio(1.0,jCintura);
    // ...en segunda lugar, se aplica el factor de escala inverso al punto 3D que se quiere alcanzar
    Vec3d cabezanorm(cabeza);
    cabezanorm /= _scale;

    // apunta el pecho en la dirección de la cabeza (si apuntase la cabeza tendríamos un modelo
    // con el cuerpo quieto y moviendo la cabeza de acá para allá como un loco)
    apunta(jPecho, cabezanorm);

    // Antes de volver, se deshace el cambio de escala
    cambiarTamanio(_scale,jCintura);

    return(true);
  }

  bool modeloHOAPpalotes::mueveBrazoIzq(const osg::Vec3d& manoizq, const osg::Vec3d& codoizq)
  {
    int ki;
    bool angulo_valido[4];
    std::vector<double> angulos_brazo_izq(4);
    std::vector<double> angulos_aux_izq(4);
    int alter = numero_alternativas;
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
    manoizqnorm /= _scale;
    
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
    if ( (enlimitesInt(16, angulos_brazo_izq[0]*180/PI))<1000 ) 
      {angulo_valido[0] = false; fuera_limites = true;}// cout<<"Límite alcanzado en LJOINT0 de HOAPpalotes"<<endl;}
    if ( (enlimitesInt(17, angulos_brazo_izq[1]*180/PI))<1000 )  
      {angulo_valido[1] = false; fuera_limites = true;}// cout<<"Límite alcanzado en LJOINT1 de HOAPpalotes"<<endl;}
    if ( (enlimitesInt(18, angulos_brazo_izq[2]*180/PI))<1000 ) 
      {angulo_valido[2] = false; fuera_limites = true;}// cout<<"Límite alcanzado en LJOINT2 de HOAPpalotes"<<endl;}
    double ang_LJOINT4;
    // Si se ha salido del límite del codo de 90, se indica
    if ( ( (ang_LJOINT4 = enlimitesInt(19, angulos_brazo_izq[3]*180/PI))<1000 ) && (ang_LJOINT4*180/PI > 0.0) ) 
      {angulo_valido[3] = false; fuera_limites = true;}// cout<<"Límite alcanzado en LJOINT3 de HOAP"<<endl;}
    // pero si se ha salido del límite del codo de -90, nos limitamos a colocar el codo en -90 (problemas con singularidades de matrices)
    if ( ( (ang_LJOINT4 = enlimitesInt(19, angulos_brazo_izq[3]*180/PI))<1000 ) && (ang_LJOINT4*180/PI < 0.0) )
      {
	angulo_valido[3] = true; 
	angulos_brazo_izq[3] = -PI/2;
	if (!fuera_limites) // si es el único que se salió de límites, se actualiza la pose
	  giraBrazoIzq(angulos_brazo_izq);
      }
    //if ( !enlimites(16, angulos_brazo_izq[0]*180/PI) ) 
    //  {angulo_valido[0] = false; fuera_limites = true;}//cout<<"Límite alcanzado en LJOINT1"<<endl;}
    //if ( !enlimites(17, angulos_brazo_izq[1]*180/PI) ) 
    //  {angulo_valido[1] = false; fuera_limites = true;}//cout<<"Límite alcanzado en LJOINT2"<<endl;}
    //if ( !enlimites(18, angulos_brazo_izq[2]*180/PI) ) 
    //  {angulo_valido[2] = false; fuera_limites = true;}//cout<<"Límite alcanzado en LJOINT3"<<endl;}
    //if ( !enlimites(19, angulos_brazo_izq[3]*180/PI) ) 
    //  {angulo_valido[3] = false; fuera_limites = true;}//cout<<"Límite alcanzado en LJOINT4"<<endl;}

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
	/*alternatas = this->alternativasBrazoIzqHOAPpalotes (alter);   
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
    if ( (par_colisionado!=-1)||(fuera_limites) ) // Si la posición no es válida...
      {
	// ...busca posiciones alternativas a aquella que está dando problemas	    
	encuentra_alternativa = false;
	alternatas_izq = this->alternativasBrazoIzq (alter, angmax);
	// Prueba las alternativas, empezando desde la más cercana, hasta que encuentra una que no produce problemas
	for (int kk=0;kk<alter;kk++)
	  {
	    // Recoloca el brazo según la posición alternativa del codo
	    angulos_brazo_izq = apuntaBrazoIzq(alternatas_izq[kk], manoizqnorm);
	    
	    // Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
	    // Comprobar límites
	    fuera_limites_alt = false;
	    if ( (enlimitesInt(16, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
	    if ( (enlimitesInt(17, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
	    if ( (enlimitesInt(18, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
	    if ( (enlimitesInt(19, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
	    //if ( !enlimites(16, angulos_brazo_izq[0]*180/PI) ) fuera_limites_alt = true;
	    //if ( !enlimites(17, angulos_brazo_izq[1]*180/PI) ) fuera_limites_alt = true;
	    //if ( !enlimites(18, angulos_brazo_izq[2]*180/PI) ) fuera_limites_alt = true;
	    //if ( !enlimites(19, angulos_brazo_izq[3]*180/PI) ) fuera_limites_alt = true;
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
		// ... si no hay por donde pillarlo, se vuelve a la última posición válida
                for (ki=0; ki<4; ki++)
                {
	           angulos_brazo_izq[ki] = ang_left_arm[ki]*PI/180;
                }
		this->jBrazoIzq.trf->setMatrix( ultimaBrazo->getMatrix() );
		this->jAntebrazoIzq.trf->setMatrix( ultimaAntebrazo->getMatrix() );
	      }
	  }
	// Se actualizan los valores de ángulos anteriores
	for (ki=0; ki<4; ki++)
	  ang_left_arm[ki]=angulos_brazo_izq[ki]*180/PI;

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
	    alternatas_izq = this->alternativasBrazoIzq (alter, angmax);
	    // Prueba las dos alternativas más cercanas
	    Vec3d codo = this->jBrazoIzq.dirsegmento;
	    codo = codo * computeLocalToWorld(this->jBrazoIzq.nodepath);
	    int alternata_def;
	    if (alternatas_izq[0].z()<alternatas_izq[1].z()) alternata_def = 0;
	    else alternata_def = 1;
	    
	    // Si la alternativa escogida da una posición con el codo más bajo...
	    if (alternatas_izq[alternata_def].z()<codo.z())
	      {
		// Recoloca el brazo según la posición alternativa del codo
		angulos_brazo_izq = apuntaBrazoIzq(alternatas_izq[alternata_def], manoizqnorm);
		
		// Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
		// Comprobar límites
		fuera_limites_alt = false;
		if ( (enlimitesInt(16, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
		if ( (enlimitesInt(17, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
		if ( (enlimitesInt(18, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
		if ( (enlimitesInt(19, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
		//if ( !enlimites(16, angulos_brazo_izq[0]*180/PI) ) fuera_limites_alt = true;
		//if ( !enlimites(17, angulos_brazo_izq[1]*180/PI) ) fuera_limites_alt = true;
		//if ( !enlimites(18, angulos_brazo_izq[2]*180/PI) ) fuera_limites_alt = true;
		//if ( !enlimites(19, angulos_brazo_izq[3]*180/PI) ) fuera_limites_alt = true;
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
	    else
	      {
		// ... si se ha encontrado una alternativa más natural, se actualizan los ángulos
		for (ki=0; ki<4; ki++)
		  ang_left_arm[ki]=angulos_brazo_izq[ki]*180/PI;
	      }
	  }
      }

    // Antes de volver, se deshace el cambio de escala
    cambiarTamanio(_scale,jCintura);

    return(result);
    
  }
//RETARGETING ANGULOS
 std::vector<double> modeloHOAPpalotes::mueveBrazoIzq(const osg::Vec3d& manoizq,std::vector<double> angulos_brazo_izq,std::vector<double> ang_left_pos_ant, const osg::Vec3d& codoizq)
 {
    int ki;
    bool angulo_valido[4];
    // NUEVO 
    std::vector<double> angulos_brazo_izq1(4);
    std::vector<double> angulos_aux_izq(4);
    int alter = numero_alternativas;
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
    manoizqnorm /= _scale;

    //NUEVO obs
    assert(_obs!=NULL);
      this->changed();
    //FIN NUEVO obs

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
    
    // Calcula nuevos ángulos
    //NUEVO     angulos_brazo_izq = apuntaIKBrazoIzqAn(manoizqnorm);
    jAntebrazoIzq.trf->setMatrix(osg::Matrix::rotate(angulos_brazo_izq[3],1.0,0.0,0.0) * // LARM_JOINT[4]
				 osg::Matrix::rotate(angulos_brazo_izq[2],0.0,0.0,1.0) * // LARM_JOINT[3]				 
				 osg::Matrix::translate(0.0f,0.0f,115.0f));

    jBrazoIzq.trf->setMatrix(osg::Matrix::rotate(angulos_brazo_izq[1],0.0,1.0,0.0) *  // LARM_JOINT[2]
			     osg::Matrix::rotate(angulos_brazo_izq[0],1.0,0.0,0.0) *  // LARM_JOINT[1]
                             osg::Matrix::rotate(osg::inDegrees(90.0),1.0,0.0,0.0) *    
			     osg::Matrix::translate(90.0,0.0,120.0));
    
    // Comprobar límites
    if ( (enlimitesInt(16, angulos_brazo_izq[0]*180/PI))<1000 ) 
      {angulo_valido[0] = false; fuera_limites = true;} //cout<<"Límite alcanzado en LJOINT0 de HOAPpalotes"<<endl;}
    if ( (enlimitesInt(17, angulos_brazo_izq[1]*180/PI))<1000 )  
      {angulo_valido[1] = false; fuera_limites = true;} //cout<<"Límite alcanzado en LJOINT1 de HOAPpalotes"<<endl;}
    if ( (enlimitesInt(18, angulos_brazo_izq[2]*180/PI))<1000 ) 
      {angulo_valido[2] = false; fuera_limites = true;} //cout<<"Límite alcanzado en LJOINT2 de HOAPpalotes"<<endl;}
    if ( (enlimitesInt(19, angulos_brazo_izq[3]*180/PI))<1000 ) 
      {angulo_valido[3] = false; fuera_limites = true;} //cout<<"Límite alcanzado en LJOINT3 de HOAPpalotes"<<endl;}
    
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
        //printf("HAY COLISION\n");
	par_colisionado = _pobs->par_colisionado;
	
	      cout <<"COLISION entre " 
		   << this->_pares_colision[par_colisionado].first->trf->getName() 
		   <<" y " 
		   << this->_pares_colision[par_colisionado].second->trf->getName() << std::endl;
	
	this->cambiaColor(*(this->_pares_colision[par_colisionado].first));
	this->cambiaColor(*(this->_pares_colision[par_colisionado].second));	
      }
    else
      {
	par_colisionado = -1;
        //printf("NO HAY COLISION\n");
      }
    if ( (par_colisionado!=-1)||(fuera_limites) ) // Si la posición no es válida...
      {
        
	// ...busca posiciones alternativas a aquella que está dando problemas	    
	encuentra_alternativa = false;
	alternatas_izq = this->alternativasBrazoIzq (alter, angmax);
	// Prueba las alternativas, empezando desde la más cercana, hasta que encuentra una que no produce problemas
	for (int kk=0;kk<alter;kk++)
	  {
	    // Recoloca el brazo según la posición alternativa del codo
	    angulos_brazo_izq = apuntaBrazoIzq(alternatas_izq[kk], manoizqnorm);
	    
	    // Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
	    // Comprobar límites
	    fuera_limites_alt = false;
	    if ( (enlimitesInt(16, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
	    if ( (enlimitesInt(17, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
	    if ( (enlimitesInt(18, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
	    if ( (enlimitesInt(19, angulos_brazo_izq[0]*180/PI))<1000 )  fuera_limites_alt = true;
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
                //printf("ALGUNOS FUERA DE LIMITE Y OTROS NO\n");
		// A ver si depuramos esto, oño
		for (ki=0; ki<4; ki++)
		  angulos_brazo_izq[ki] = angulos_aux_izq[ki];
		
		giraBrazoIzq(angulos_brazo_izq);
		
	      }
	    else
	      {
                //printf("VOLVER A ULTIMA POSICION\n");
		// ... si no hay por donde pillarlo, se vuelve a la última posición válida
                for (ki=0; ki<4; ki++)
                {
	           angulos_brazo_izq[ki] = ang_left_pos_ant[ki]*PI/180;
                }
		this->jBrazoIzq.trf->setMatrix( ultimaBrazo->getMatrix() );
		this->jAntebrazoIzq.trf->setMatrix( ultimaAntebrazo->getMatrix() );
	      
              }
	  }
      }
    

    // Antes de volver, se deshace el cambio de escala
    cambiarTamanio(_scale,jCintura);
    for (ki=0; ki<4; ki++)
     {
       angulos_brazo_izq1[ki]=angulos_brazo_izq[ki]*180/PI;
      }
    

  return angulos_brazo_izq1;
 
  }
//FIN RETARGETING ANGULOS
  bool modeloHOAPpalotes::mueveBrazoDer(const osg::Vec3d& manoder, const osg::Vec3d& cododer)
  {
    int ki;
    bool angulo_valido[4];
    std::vector<double> angulos_brazo_der(4);
    std::vector<double> angulos_aux_der(4);
    int alter = numero_alternativas;
    int par_colisionado = -1;
    
    bool result = true;
    bool fuera_limites = false;
    bool fuera_limites_alt = false;
    bool encuentra_alternativa;
    MatrixTransform* ultimaBrazo = new MatrixTransform;

    // Pasos previos para considerar los cambios de escala. En primer lugar, se fija la escala del modelo a 1...
    cambiarTamanio(1.0,jCintura);
    // ...en segunda lugar, se aplica el factor de escala inverso al punto 3D que se quiere alcanzar
    Vec3d manodernorm(manoder);
    manodernorm /= _scale;

    // Lo primero, comprobar que hay un observador de colisiones
    assert(_pobs!=NULL);
    MatrixTransform* ultimaAntebrazo = new MatrixTransform;
    
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
    if ( (enlimitesInt(6, angulos_brazo_der[0]*180/PI))<1000 ) 
      {angulo_valido[0] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT1"<<endl;}
    if ( (enlimitesInt(7, angulos_brazo_der[1]*180/PI))<1000 ) 
      {angulo_valido[1] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT2"<<endl;}
    if ( (enlimitesInt(8, angulos_brazo_der[2]*180/PI))<1000 ) 
      {angulo_valido[2] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT3"<<endl;}
    double ang_RJOINT4;
    // Si se ha salido del límite del codo de 90, se indica
    if ( ( (ang_RJOINT4 = enlimitesInt(9, angulos_brazo_der[3]*180/PI))<1000 ) && (ang_RJOINT4*180/PI > 0.0) ) 
      {angulo_valido[3] = false; fuera_limites = true;}// cout<<"Límite alcanzado en RJOINT3 de NOMAD"<<endl;}
    // pero si se ha salido del límite del codo de -90, nos limitamos a colocar el codo en -90 (problemas con singularidades de matrices)
    if ( ( (ang_RJOINT4 = enlimitesInt(9, angulos_brazo_der[3]*180/PI))<1000 ) && (ang_RJOINT4*180/PI < 0.0) )
      {
	angulo_valido[3] = true; 
	angulos_brazo_der[3] = -PI/2;
	if (!fuera_limites) // si es el único que se salió de límites, se actualiza la pose
	  giraBrazoDer(angulos_brazo_der);
      }
    //if ( !enlimites(6, angulos_brazo_der[0]*180/PI) ) 
    //  {angulo_valido[0] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT1"<<endl;}
    //if ( !enlimites(7, angulos_brazo_der[1]*180/PI) ) 
    //  {angulo_valido[1] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT2"<<endl;}
    //if ( !enlimites(8, angulos_brazo_der[2]*180/PI) ) 
    //  {angulo_valido[2] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT3"<<endl;}
    //if ( !enlimites(9, angulos_brazo_der[3]*180/PI) ) 
    //  {angulo_valido[3] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT4"<<endl;}

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
	/*alternatas = this->alternativasBrazoDerHOAPpalotes (alter);   
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
    // Si ha habido colisión o nos hemos salido de algún límite, se buscan posiciones alternativas
    if ( (par_colisionado!=-1)||(fuera_limites) )
      {
	// ...busca posiciones alternativas a aquella que está dando problemas	    
	encuentra_alternativa = false;
	alternatas_der = this->alternativasBrazoDer (alter, angmax);
	// Prueba las alternativas, empezando desde la más cercana, hasta que encuentra una que no produce problemas
	for (int kk=0;kk<alter;kk++)
	  {
	    // Recoloca el brazo según la posición alternativa del codo
	    angulos_brazo_der = apuntaBrazoDer(alternatas_der[kk], manodernorm);
	    
	    // Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
	    // Comprobar límites
	    fuera_limites_alt = false;
	    if ( (enlimitesInt(6, angulos_brazo_der[0]*180/PI))<1000 ) fuera_limites_alt = true;
	    if ( (enlimitesInt(7, angulos_brazo_der[1]*180/PI))<1000 ) fuera_limites_alt = true;
	    if ( (enlimitesInt(8, angulos_brazo_der[2]*180/PI))<1000 ) fuera_limites_alt = true;
	    if ( (enlimitesInt(9, angulos_brazo_der[3]*180/PI))<1000 ) fuera_limites_alt = true;
	    //if ( !enlimites(6, angulos_brazo_der[0]*180/PI) ) fuera_limites_alt = true;
	    //if ( !enlimites(7, angulos_brazo_der[1]*180/PI) ) fuera_limites_alt = true;
	    //if ( !enlimites(8, angulos_brazo_der[2]*180/PI) ) fuera_limites_alt = true;
	    //if ( !enlimites(9, angulos_brazo_der[3]*180/PI) ) fuera_limites_alt = true;

	    // Comprobar si ha habido colisiones
	    _pobs->limpiarLinks();
	    linksMovidos(6, &_pobs->_links_sospechosos);
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
		for (ki=0; ki<4; ki++)
		  angulos_brazo_der[ki] = angulos_aux_der[ki];
		
		giraBrazoDer(angulos_brazo_der);
		
	      }
	    else
	      {
                for (ki=0; ki<4; ki++)
                {
	           angulos_brazo_der[ki] = ang_right_arm[ki]*PI/180;
                }
		// ... si no hay por donde pillarlo, se vuelve a la última posición válida
		this->jBrazoDer.trf->setMatrix( ultimaBrazo->getMatrix() );
		this->jAntebrazoDer.trf->setMatrix( ultimaAntebrazo->getMatrix() );
	      }
	  }

	// Se actualizan los valores de ángulos anteriores
	for (ki=0; ki<4; ki++)
	  ang_right_arm[ki]=angulos_brazo_der[ki]*180/PI;
      }
    // Si estamos dentro de los límites, se miran sólo las dos alternativas más cercanas, buscando una posición
    // más baja (más "relajada y natural") para el codo
    else
      {
	if (fabs(angulos_brazo_der[3])>0.1)
	  {
	    
	    // Salva la posición recién calculada, que a fin de cuentas es válida y probablemente haya que volver a ella
	    ultimaBrazo->setMatrix( this->jBrazoDer.trf->getMatrix() );
	    ultimaAntebrazo->setMatrix( this->jAntebrazoDer.trf->getMatrix() );
	    
	    // ...busca posiciones alternativas a la actual	    
	    encuentra_alternativa = false;
	    alternatas_der = this->alternativasBrazoDer (alter, angmax);
	    // Prueba las dos alternativas más cercanas
	    Vec3d codo = this->jBrazoDer.dirsegmento;
	    codo = codo * computeLocalToWorld(this->jBrazoDer.nodepath);
	    int alternata_def;
	    if (alternatas_der[0].z()<alternatas_der[1].z()) alternata_def = 0;
	    else alternata_def = 1;
	    
	    // Si la alternativa escogida da una posición con el codo más bajo...
	    if (alternatas_der[alternata_def].z()<codo.z())
	      {
		// Recoloca el brazo según la posición alternativa del codo
		angulos_brazo_der = apuntaBrazoDer(alternatas_der[alternata_def], manodernorm);
		
		// Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
		// Comprobar límites
		fuera_limites_alt = false;
		if ( (enlimitesInt(6, angulos_brazo_der[0]*180/PI))<1000 ) fuera_limites_alt = true;
		if ( (enlimitesInt(7, angulos_brazo_der[1]*180/PI))<1000 ) fuera_limites_alt = true;
		if ( (enlimitesInt(8, angulos_brazo_der[2]*180/PI))<1000 ) fuera_limites_alt = true;
		if ( (enlimitesInt(9, angulos_brazo_der[3]*180/PI))<1000 ) fuera_limites_alt = true;
		//if ( !enlimites(6, angulos_brazo_der[0]*180/PI) ) fuera_limites_alt = true;
		//if ( !enlimites(7, angulos_brazo_der[1]*180/PI) ) fuera_limites_alt = true;
		//if ( !enlimites(8, angulos_brazo_der[2]*180/PI) ) fuera_limites_alt = true;
		//if ( !enlimites(9, angulos_brazo_der[3]*180/PI) ) fuera_limites_alt = true;
		// Comprobar si ha habido colisiones
		_pobs->limpiarLinks();
		linksMovidos(6, &_pobs->_links_sospechosos);
		this->changed();
		if ( (_pobs->no_hay_colision)&&(!fuera_limites_alt) )    
		  {
		    encuentra_alternativa = true;
		  }
	      }
	    // Si no hay alternativas, se vuelve a la última válida conocida
	    if (!encuentra_alternativa)
	      {
		this->jBrazoDer.trf->setMatrix( ultimaBrazo->getMatrix() );
		this->jAntebrazoDer.trf->setMatrix( ultimaAntebrazo->getMatrix() );
	      }
	    else
	      {
		// ... si se ha encontrado una alternativa más natural, se actualizan los ángulos
		for (ki=0; ki<4; ki++)
		    ang_right_arm[ki]=angulos_brazo_der[ki]*180/PI;
	      }
	  }
      }

    // Antes de volver, se deshace el cambio de escala
    cambiarTamanio(_scale,jCintura);

    return(result);
    
  }
//INICIO RETARGETING ANGULOS
std::vector<double> modeloHOAPpalotes::mueveBrazoDer(const osg::Vec3d& manoder,std::vector<double> angulos_brazo_der,std::vector<double> ang_right_pos_ant, const osg::Vec3d& cododer)
  {
    int ki;
    bool angulo_valido[4];
    std::vector<double> angulos_brazo_der1(4);
    std::vector<double> angulos_aux_der(4);
    int alter = numero_alternativas;
    int par_colisionado = -1;
    
    bool result = true;
    bool fuera_limites = false;
    bool fuera_limites_alt = false;
    bool encuentra_alternativa;
    MatrixTransform* ultimaBrazo = new MatrixTransform;

    // Pasos previos para considerar los cambios de escala. En primer lugar, se fija la escala del modelo a 1...
    cambiarTamanio(1.0,jCintura);
    // ...en segunda lugar, se aplica el factor de escala inverso al punto 3D que se quiere alcanzar
    Vec3d manodernorm(manoder);
    manodernorm /= _scale;
    //NUEVO obs
    assert(_obs!=NULL);
      this->changed();
    //FIN NUEVO obs
    // Lo primero, comprobar que hay un observador de colisiones
    assert(_pobs!=NULL);
    MatrixTransform* ultimaAntebrazo = new MatrixTransform;
    
    // Salva la posición anterior, por si es necesario volver a ella
    ultimaBrazo->setMatrix( this->jBrazoDer.trf->getMatrix() );
    ultimaAntebrazo->setMatrix( this->jAntebrazoDer.trf->getMatrix() );
    for (ki=0; ki<4; ki++)
      {
	angulo_valido[ki] = true;
	angulos_aux_der[ki] = angulos_brazo_der[ki];
      }
    
    // Calcula nuevos ángulos
    //Nuevo angulos_brazo_der1 = apuntaIKBrazoDerAn(manodernorm);
    jAntebrazoDer.trf->setMatrix(osg::Matrix::rotate(angulos_brazo_der[3],1.0,0.0,0.0) *//RARMJOINT[4]
				 osg::Matrix::rotate(angulos_brazo_der[2],0.0,0.0,1.0) *//RARMJOINT[3] 
				 osg::Matrix::translate(0.0f,0.0f,115.0f));

    jBrazoDer.trf->setMatrix(osg::Matrix::rotate(angulos_brazo_der[1],0.0,1.0,0.0) *  // RARMJOINT[2]
			     osg::Matrix::rotate(angulos_brazo_der[0],1.0,0.0,0.0) *  // RARMJOINT[1]
			     osg::Matrix::rotate(osg::inDegrees(90.0),1.0,0.0,0.0) *              
			     osg::Matrix::translate(-90.0,0.0,120.0));
    
    
    // Comprobar límites
    if ( (enlimitesInt(6, angulos_brazo_der[0]*180/PI))<1000 ) 
      {angulo_valido[0] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT1"<<endl;}
    if ( (enlimitesInt(7, angulos_brazo_der[1]*180/PI))<1000 ) 
      {angulo_valido[1] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT2"<<endl;}
    if ( (enlimitesInt(8, angulos_brazo_der[2]*180/PI))<1000 ) 
      {angulo_valido[2] = false; fuera_limites = true;}//cout<<"Límite alcanzado en RJOINT3"<<endl;}
    if ( (enlimitesInt(9, angulos_brazo_der[3]*180/PI))<1000 ) 
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
        //printf("HAY COLISION\n");
	par_colisionado = _pobs->par_colisionado;
	
	      cout <<"COLISION entre " 
		   << this->_pares_colision[par_colisionado].first->trf->getName() 
		   <<" y " 
		   << this->_pares_colision[par_colisionado].second->trf->getName() << std::endl;
	
	this->cambiaColor(*(this->_pares_colision[par_colisionado].first));
	this->cambiaColor(*(this->_pares_colision[par_colisionado].second));	
      }
    else
      {
	par_colisionado = -1;
         //printf("NO HAY COLISION\n");
	// PRUEBA. TAMBIEN SACA ALTERNATIVAS, PA VER... PERO LAS SACA CON BOLAS 3D Y TO!!!! OOOOOMA MORENOOOOOO!!!
	/*alternatas = this->alternativasBrazoDerHOAPpalotes (alter);   
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
    // Si ha habido colisión o nos hemos salido de algún límite, se buscan posiciones alternativas
    if ( (par_colisionado!=-1)||(fuera_limites) )
      {
        
	// ...busca posiciones alternativas a aquella que está dando problemas	    
	encuentra_alternativa = false;
	alternatas_der = this->alternativasBrazoDer (alter, angmax);
	// Prueba las alternativas, empezando desde la más cercana, hasta que encuentra una que no produce problemas
	for (int kk=0;kk<alter;kk++)
	  {
	    // Recoloca el brazo según la posición alternativa del codo
	    angulos_brazo_der = apuntaBrazoDer(alternatas_der[kk], manodernorm);
	    
	    // Comprueba límites y colisiones. Si no hay problemas, se selecciona esa posición
	    // Comprobar límites
	    fuera_limites_alt = false;
	    if ( (enlimitesInt(6, angulos_brazo_der[0]*180/PI))<1000 ) fuera_limites_alt = true;
	    if ( (enlimitesInt(7, angulos_brazo_der[1]*180/PI))<1000 ) fuera_limites_alt = true;
	    if ( (enlimitesInt(8, angulos_brazo_der[2]*180/PI))<1000 ) fuera_limites_alt = true;
	    if ( (enlimitesInt(9, angulos_brazo_der[3]*180/PI))<1000 ) fuera_limites_alt = true;
	    // Comprobar si ha habido colisiones
	    _pobs->limpiarLinks();
	    linksMovidos(6, &_pobs->_links_sospechosos);
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
                //printf("ALGUNOS FUERA DE LIMITE Y OTROS NO\n");
		for (ki=0; ki<4; ki++)
		  angulos_brazo_der[ki] = angulos_aux_der[ki];
		
		giraBrazoDer(angulos_brazo_der1);
		
	      }
	    else
	      {
		// ... si no hay por donde pillarlo, se vuelve a la última posición válida
                //printf("VOLVER A ULTIMA POSICION\n");
                for (ki=0; ki<4; ki++)
                {
	           angulos_brazo_der[ki] = ang_right_pos_ant[ki]*PI/180;
                }
		this->jBrazoDer.trf->setMatrix( ultimaBrazo->getMatrix() );
		this->jAntebrazoDer.trf->setMatrix( ultimaAntebrazo->getMatrix() );
	      }
	  }
      }
    

    // Antes de volver, se deshace el cambio de escala
    cambiarTamanio(_scale,jCintura);
    
    for (ki=0; ki<4; ki++)
     {
       angulos_brazo_der1[ki]=angulos_brazo_der[ki]*180/PI;
      }

    return angulos_brazo_der1;
    
  }

//FIN RETARGETING ANGULOS
  // Función para adaptar la altura a la que se sitúa la cara del modelo  a un valor determinado (en cm.)
  void modeloHOAPpalotes::setAlturaCara(int altura_cm)
  {
    double altura_modelo; // altura normalizada (sin aplicar el factor de escala actual) del modelo
    double factor;        // factor de escalado

    // En primer lugar, se fija la escala del modelo a 1...
    cambiarTamanio(1.0,jCintura);
    altura_modelo = jPantorrillaIzq.dirsegmento.length() + jMusloIzq.dirsegmento.length() + 
      jCintura.dirsegmento.length() + jPecho.dirsegmento.length() + jCabeza.dirsegmento.length();
    altura_modelo = 0.9025*altura_modelo; // Altura de la nariz, obtenida como aproximación usando tablas antropométricas
    _scale = 1.0; //... y el factor de escala que pudiera haber se sobreescribe con 1

    // En segundo lugar, se fija el nuevo factor de escala del modelo
    factor = ((double)altura_cm*10) / altura_modelo; // la altura en cm se multiplica por 10 para pasarla a mm.
    if (factor >= 1.0) {
      agrandaModelo(factor);
    }
    else {
      factor = 1 / factor;
      achicaModelo(factor);
    }    
  }

  // Devuelve la altura del modelo (en cm)
  int modeloHOAPpalotes::devuelveAltura()
  {
    double altura = jPecho.dirsegmento.length() + 
      jMusloIzq.dirsegmento.length() + jPantorrillaIzq.dirsegmento.length() +
      jCintura.dirsegmento.length() + jCabeza.dirsegmento.length()/2;
    
    return( (int)(altura/10) );   // Se divide entre 10 porque las medidas del HOAPpalotes están orig. en mm.
  }

}
