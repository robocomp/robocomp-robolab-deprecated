/*
 *    Copyright (C) 2006-2010 by RoboLab - University of Extremadura
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "worker.h"



using namespace osgDTE;
using namespace std;

#define OSGMODELOVIEWERDEBUG 1

#ifdef OSGMODELOVIEWERDEBUG 
  #define SIDEBUG(x) x
#else
  #define SIDEBUG(x)
#endif

osg::Group* escena;
osg::ref_ptr<modelo> mT = 0;
enum tipo_modelo{persona,HOAP,NOMAD,personapalotes,HOAPpalotes,NOMADpalotes};
tipo_modelo modelo_es;
MatrixTransform* pos_cabeza = new MatrixTransform;
MatrixTransform* pos_cintura = new MatrixTransform;
MatrixTransform* pos_mano_der = new MatrixTransform;
MatrixTransform* pos_mano_izq = new MatrixTransform;
MatrixTransform* pos_codo_der = new MatrixTransform;
MatrixTransform* pos_codo_izq = new MatrixTransform;
MatrixTransform* pos_homb_der = new MatrixTransform;
MatrixTransform* pos_homb_izq = new MatrixTransform;
MatrixTransform* pos_suelo = new MatrixTransform;
std::vector<MatrixTransform*> pos_alternatas_der;
std::vector<MatrixTransform*> pos_alternatas_izq;
osg::Group* grupoalternatas = new osg::Group();

std::vector<double> array_grados(20); // ya lo inicializa a ceros
int articulacion = 0;
osgDTE::observadormodelocoli* pobs = 0;
int par_colisionado = -1;
bool bolas_visibles = true;



// PRUEBA...
FILE* f1 = NULL;
int cuenta_frames;

int alturacm = 170;

// //: Clase para manejar eventos con pick
class PickHandler : public osgGA::GUIEventHandler {
  
public:
  PickHandler(osgViewer::Viewer* viewer) :
    arrastrando(false),
    _viewer(viewer),
    _cual(ninguno)
  {}
  
  bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

  virtual bool pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);

  virtual bool move(const osgGA::GUIEventAdapter& ea);

  virtual void guardarXYVentanaInicial(const osgGA::GUIEventAdapter& ea);

  virtual void guardarTransformacion(MatrixTransform* trf);
  
protected:

  bool arrastrando;
  osgViewer::Viewer* _viewer;
  ref_ptr<const osgGA::GUIEventAdapter> mouse_inicial;
  MatrixTransform* trf_inicial;
  MatrixTransform* trf_actual;
  enum e_cual { ninguno, codo, munieca, CCD, CCDInv, AnI, AnD, HoI, HoD, Cabeza, CoI,CoD } _cual;

};

// ---------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------
// Función que gestiona el movimiento, con el ratón, de las bolas
bool PickHandler::move(const osgGA::GUIEventAdapter& ea)
{
  if (arrastrando) {	
    // Computar el rayo desde el punto donde esta el raton
    Vec3 near, far;

    osg::Matrix MVPW = _viewer->getCamera()->getViewMatrix() 
      * _viewer->getCamera()->getProjectionMatrix() 
      * _viewer->getCamera()->getViewport()->computeWindowMatrix();

    osg::Matrix inverse_MVPW;
    inverse_MVPW.invert(MVPW);

    near = osg::Vec3(ea.getX(), ea.getY(), 0.0) * inverse_MVPW;
    far = osg::Vec3(ea.getX(), ea.getY(), 1.0)* inverse_MVPW;

    //_viewer->computeNearFarPoints(ea.getX(), ea.getY(), 0, near, far);
    
    // Proyectar la posicion de la bola en el espacio a ese rayo
    Vec3 r_inicial = trf_inicial->getMatrix().getTrans();    
    Vec3 r_final(Project(r_inicial, near, far));
	
    
    // Mover la bola a ese punto
    trf_actual->setMatrix(Matrix::translate(r_final));
    
    // "limpia" los colores de las articulaciones
    std::vector <int>::iterator kk;
    for (kk=pobs->_pares_colisionados.begin(); kk<pobs->_pares_colisionados.end(); kk++)
      {
	mT->devuelveColor(*(mT->_pares_colision[*kk].first));
	mT->devuelveColor(*(mT->_pares_colision[*kk].second)); 
      }   
    pobs->_pares_colisionados.clear();
    
    // Apuntar el segmento que convenga a ese punto
		switch (_cual)
		{
			case AnI:
			// apunta el brazo izquierdo del modelo... incluye búsqueda de alternativas debida a colisiones y límites
			mT->mueveBrazoIzq(r_final,pos_codo_izq->getMatrix().getTrans());
			break;
			case AnD:
			// apunta el brazo derecho del modelo... incluye búsqueda de alternativas debida a colisiones y límites
			mT->mueveBrazoDer(r_final,pos_codo_der->getMatrix().getTrans());	
		// 	std::cout<<"posicion enviada al brazo brazo"<<r_final<<std::endl;
			break;
			case CoI:
			// apunta el codo izquierdo del modelo... incluye búsqueda de alternativas debida a colisiones y límites
			mT->mueveBrazoIzq(pos_mano_izq->getMatrix().getTrans(),r_final);
			break;
			case CoD:
			// apunta el codo izquierdo del modelo... incluye búsqueda de alternativas debida a colisiones y límites
			mT->mueveBrazoDer(pos_mano_der->getMatrix().getTrans(),r_final);
			break;
		
			case HoI:
			// AÑADIR FUNCIÓN PARA GIRAR EL TORSO DE LA MANERA ADECUADA
			break;
			case HoD:
			// AÑADIR FUNCIÓN PARA GIRAR EL TORSO DE LA MANERA ADECUADA
			break;	
			case Cabeza:
			mT->mueveCabeza(r_final);
			break;
			default:
			break;
		}
	return true;
	}
  else return false;
}



// -----------------------------------------------------------



bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
{
  int xcab, ycab, zcab;
  int xmanoder, ymanoder, zmanoder;
  int xmanoizq, ymanoizq, zmanoizq;
  osg::Vec3d cab, manoder, manoizq;

  std::vector <int>::iterator kk;

  switch(ea.getEventType())
    {
    case(osgGA::GUIEventAdapter::PUSH):
        {
	  return pick(_viewer,ea);
        }

    case(osgGA::GUIEventAdapter::DRAG):
      {
	return move(ea);
      }
       
    case(osgGA::GUIEventAdapter::RELEASE):
      {
	arrastrando = false;
	return false;
      }
 
    case(osgGA::GUIEventAdapter::KEYDOWN):
      {
	switch(ea.getKey())
	  {
	  case '+':
	    if ( (modelo_es == persona)||(modelo_es == personapalotes) ) {
	      alturacm += 5;
	      mT->setAlturaCara(alturacm);
	    }
	    else {
	      mT->achicaModelo();
	    }
	    break;
	  case '-':
	    if ( (modelo_es == persona)||(modelo_es == personapalotes) ) {
	      alturacm -= 5;
	      mT->setAlturaCara(alturacm);
	    }
	    else {
	      mT->achicaModelo();
	    }
	    break;

	  case 'q':
	    if (bolas_visibles) {
	      pos_cabeza->setNodeMask(0);
	      pos_mano_izq->setNodeMask(0);
	      pos_mano_der->setNodeMask(0);
		  pos_codo_der->setNodeMask(0);
		  pos_codo_izq->setNodeMask(0);
	      pos_homb_izq->setNodeMask(0);
	      pos_homb_der->setNodeMask(0);
	      grupoalternatas->setNodeMask(0);
	      bolas_visibles = false;
	    }
	    else {
	      pos_cabeza->setNodeMask(1);
	      pos_mano_izq->setNodeMask(1);
	      pos_mano_der->setNodeMask(1);
		  pos_codo_der->setNodeMask(1);
		  pos_codo_izq->setNodeMask(1);
	      pos_homb_izq->setNodeMask(1);
	      pos_homb_der->setNodeMask(1);
	      grupoalternatas->setNodeMask(1);
	      bolas_visibles = true;	      
	    }
	    break;
	    
	  }
	return false;
      }

    default:
      return false;
    }
}

bool PickHandler::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
{
    osgUtil::LineSegmentIntersector::Intersections intersections;

    float x = ea.getX();
    float y = ea.getY();

    if (view->computeIntersections(x,y,intersections))
    {
      // just take the first hit - nearest the eye point.
      osgUtil::LineSegmentIntersector::Intersections::iterator hit = intersections.begin();
      //osgUtil::Hit& hit = hlist.front();      
      NodePath nodePath = hit->nodePath;
      Node* node = (nodePath.size()>=1)?nodePath[nodePath.size()-1]:0;
      Group* parent = (nodePath.size()>=2)?dynamic_cast<Group*>(nodePath[nodePath.size()-2]):0;
      
      if (parent && node)
	{
	  Geode* geode = dynamic_cast<Geode*>(node);
	  MatrixTransform* trf = dynamic_cast<MatrixTransform*>(parent);
	  if (geode && trf)
	    {
	      arrastrando = true;
	      guardarXYVentanaInicial(ea);
	      guardarTransformacion(trf);
	      if (geode->getName() == "codo") {
		_cual = codo;
	      }
	      else if (geode->getName() == "muñeca") {
		_cual = munieca;
	      }
	      else if (geode->getName() == "CCD") {
		_cual = CCD;
	      }
	      else if (geode->getName() == "CCDInv") {
		_cual = CCDInv;
	      }
	      else if (geode->getName() == "AnI") {
		_cual = AnI;
	      }
	      else if (geode->getName() == "AnD") {
		_cual = AnD;
	      }
	      else if (geode->getName() == "HoI") {
		_cual = HoI;
	      }
	      else if (geode->getName() == "HoD") {
		_cual = HoD;
	      }
	      else if (geode->getName() == "Cabeza") {
		_cual = Cabeza;
	      }
	      else if (geode->getName() == "CoI") {
		_cual = CoI;
	      }
	      else if (geode->getName() == "CoD") {
		_cual = CoD;
	      }
	      else arrastrando = false;
	    }
	}
      
      return true;
    }
}



void PickHandler::guardarXYVentanaInicial(const osgGA::GUIEventAdapter& ea)
{
  mouse_inicial = &ea;
}

void PickHandler::guardarTransformacion(MatrixTransform* trf)
{
  trf_inicial = new MatrixTransform(*trf, CopyOp::DEEP_COPY_ALL);
  trf_actual = trf;
}


/** Crear un modelillo con una esfera */
MatrixTransform* crearBolaDinamica(const char* nombre, const osg::Vec4& color, 
						 const osg::Vec3& posicion, float radio)
{
  // Primer segmento (solo una esfera)
  MatrixTransform* trf_0 = new MatrixTransform;
  //trf_0->setMatrix(Matrix::translate(-18.0f,95.0f,52.0f));
  trf_0->setMatrix(Matrix::translate(posicion));
  Geode* geo_0 = new Geode();
  geo_0->setName(nombre);
  geo_0->addDrawable(esfera(Vec3(0.0f,0.0f,0.0f),2.0*radio,color));
  trf_0->addChild(geo_0);

  return trf_0;
}

// Crear un modelillo con una caja
MatrixTransform* crearCajaDinamica(const char* nombre, const osg::Vec4& color, 
				   const osg::Vec3& posicion)
{
  // Para los tamanios
  float lado = 3;
  // Primer segmento (solo una esfera)
  MatrixTransform* trf_0 = new MatrixTransform;
  //trf_0->setMatrix(Matrix::translate(-18.0f,95.0f,52.0f));
  trf_0->setMatrix(Matrix::translate(posicion));
  Geode* geo_0 = new Geode();
  geo_0->setName(nombre);
  geo_0->addDrawable(caja(Vec3(0.0f,0.0f,0.0f),lado,lado,lado,color));
  trf_0->addChild(geo_0);
  return trf_0;
}

osg::Group* createModeloOSG()
{
  osg::Group* group = new osg::Group();
  if ( (modelo_es == persona)||(modelo_es == personapalotes)||(modelo_es == NOMAD)||(modelo_es == NOMADpalotes) )
    {
      group->addChild(pos_cabeza = crearBolaDinamica("Cabeza", AZUL, Vec3(0.0f,0.0f,100.0f), 2.0f));
      group->addChild(pos_mano_izq = crearBolaDinamica("AnI", AMARILLO, Vec3(30.0f,-30.0f,40.0f), 2.0f));
      group->addChild(pos_mano_der = crearBolaDinamica("AnD", ROJO2, Vec3(-30.0f,-30.0f,40.0f), 2.0f));
	  group->addChild(pos_codo_izq = crearBolaDinamica("CoI", AZUL, Vec3(50.0f,0.0f,40.0f), 2.0f));
	  group->addChild(pos_codo_der = crearBolaDinamica("CoD", AZUL, Vec3(-50.0f,0.0f,40.0f), 2.0f));	  
      group->addChild(pos_homb_izq = crearBolaDinamica("HoI", VERDE, Vec3(30.0f,0.0f,90.0f), 1.0f));
      group->addChild(pos_homb_der = crearBolaDinamica("HoD", BLANCO, Vec3(-30.0f,0.0f,90.0f), 1.0f));
    }
  else
    {
      group->addChild(pos_cabeza = crearBolaDinamica("Cabeza", AZUL, Vec3(0.0f,0.0f,700.0f), 2.0f));
      group->addChild(pos_mano_izq = crearBolaDinamica("AnI", AMARILLO, Vec3(100.0f,-300.0f,400.0f), 2.0f));
      group->addChild(pos_mano_der = crearBolaDinamica("AnD", ROJO2, Vec3(-100.0f,-300.0f,400.0f), 2.0f));
      group->addChild(pos_homb_izq = crearBolaDinamica("HoI", VERDE, Vec3(100.0f,0.0f,650.0f), 1.0f));
      group->addChild(pos_homb_der = crearBolaDinamica("HoD", BLANCO, Vec3(-100.0f,0.0f,650.0f), 1.0f));
    } 
  return group;
}

void updateModelo()
{
  // -----------------------------------------------------------------------------------------------------------
  // Si NO HAY RETARGETING, el updateModelo se limita a actualizar las posiciones del modelo visto
  
   // Si no se está haciendo nada, se comprueba de todos modos la posición de la cabeza para ver si se puede poner el cuerpo 
    // mejor alienado respecto al eje Y (mirando p'alante)
    //  mT->mueveCabeza(pos_cabeza->getMatrix().getTrans());
    mT->alineaHombros(pos_homb_izq->getMatrix().getTrans(), pos_homb_der->getMatrix().getTrans());
    mT->mueveBrazoIzq(pos_mano_izq->getMatrix().getTrans(),pos_codo_izq->getMatrix().getTrans());
    mT->mueveBrazoDer(pos_mano_der->getMatrix().getTrans(),pos_codo_der->getMatrix().getTrans());
	
   
  // Haya o no retargeting, las alternativas se muestran
  for (int kk=0; kk<mT->numero_alternativas; kk++)
    {
      pos_alternatas_izq.at(kk)->setMatrix(osg::Matrix::translate(mT->alternatas_izq.at(kk))); 
      pos_alternatas_der.at(kk)->setMatrix(osg::Matrix::translate(mT->alternatas_der.at(kk))); 
    }

}




/**
* \brief Default constructor
*/
Worker::Worker(QObject *parent) : QObject(parent)
{

  t.start();
  myfile.open ("AdiosModelo01.txt");
  // add model to viewer.
  escena = new osg::Group();
  // registra el observador de colisiones
  pobs = new osgDTE::observadormodelocoli;

  modelo_es = persona;
  mT = new osgDTE::modeloVISOR(pobs);

  escena = createModeloOSG();
  mT->reset();
  escena->addChild(mT.get());
  /// add the handler for doing the picking
  viewer.addEventHandler(new PickHandler(&viewer));

  // Crea una cámara para ver la escena
  osgGA::TrackballManipulator* manipulator = new osgGA::TrackballManipulator();
  viewer.setCameraManipulator(manipulator);

  viewer.addEventHandler(new osgViewer::StatsHandler);
  viewer.addEventHandler(new osgViewer::WindowSizeHandler());
  
  
  // Variables relacionadas con el número de alternativas, para establecerlo y pintarlas en el entorno 3D
  mT->numero_alternativas = 20;
  mT->angmax = PI/8;
  pos_alternatas_izq.resize(mT->numero_alternativas);
  pos_alternatas_der.resize(mT->numero_alternativas);
  for (int i=0; i<mT->numero_alternativas; i++)
    {  
      pos_alternatas_izq.at(i) = new MatrixTransform;
      pos_alternatas_der.at(i) = new MatrixTransform;
      grupoalternatas->addChild(pos_alternatas_izq.at(i) = 
				crearBolaDinamica("AltIzq", AMARILLO, Vec3(0.0f,0.0f,0.0f), 0.5f));
      grupoalternatas->addChild(pos_alternatas_der.at(i) = 
				crearBolaDinamica("AltDer", ROJO2, Vec3(0.0f,0.0f,0.0f), 0.5f));
    }
  
  escena->addChild(grupoalternatas);

  viewer.setSceneData(escena);

  mT->registrar(pobs);

  // Establece una serie de colisiones y limites por defecto
  mT->colisionesDefecto();
   

  mT->limitesDefecto();  
  
	///*********************************************************
	mutex = new QMutex();
	//setupUi(this);
	//show();
	
	

	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));
	Period = BASIC_PERIOD;
	timer.start(Period);
	
}

/**
* \brief Default destructor
*/
Worker::~Worker()
{
	myfile.close();
}
void Worker::killYourSelf()
{
	rDebug("Killing myself");
	emit kill();
	exit(1);
}
/**
* \brief Change compute period
* @param per Period in ms
*/
void Worker::setPeriod(int p)
{
	rDebug("Period changed"+QString::number(p));
	Period = p;
	timer.start(Period);
}
void Worker::compute( )
{    
	mutex->lock();
  if( !viewer.done() )
    { 
		updateModelo();
		viewer.frame();
	}
	std::vector<double> v=getAnglesRight();
	myfile.open("AdiosModelo01.txt",std::ios::app);
	if (v.size()!=0){
		qDebug()<<v[3]<<(-1)*(v[3]+M_PI/2)<<t.currentTime().toString("hh:mm:ss.zzz");
		myfile<<t.currentTime().toString("hh:mm:ss.zzz").toStdString()<<" "<<(-1)*v[0]<<" "<<v[1]-(M_PI/2)<<" "<<v[2]<<" "<<(-1)*(v[3]+M_PI/2)<<"\n";
	}
	myfile.close();
	
    mutex->unlock();
}

void Worker::setWrist(Vec3 pose)
{
	std::cout<<"set Wrist pose "<<pose<<"\n";
	mutex->lock();
	
	Matrixd d =pos_mano_der->getMatrix();
	d.setTrans(pose);
	pos_mano_der->setMatrix(d);
	
	
	// "limpia" los colores de las articulaciones
    std::vector <int>::iterator kk;
    for (kk=pobs->_pares_colisionados.begin(); kk<pobs->_pares_colisionados.end(); kk++)
      {
	mT->devuelveColor(*(mT->_pares_colision[*kk].first));
	mT->devuelveColor(*(mT->_pares_colision[*kk].second)); 
      }   
    pobs->_pares_colisionados.clear();
	
// 	std::cout<<"pos_mano_der->getMatrix().getTrans()"<<pos_mano_der->getMatrix().getTrans()<<"\n";
    mT->mueveBrazoDer(pose);
	
// 	std::cout<<"pos_mano_der->getMatrix().getTrans()"<<pos_mano_der->getMatrix().getTrans()<<"\n";
	mutex->unlock();

}
void Worker::setElbowWrist(Vec3 wrist, Vec3 elbow, std::vector< float > m)
{
// 	std::cout<<"set elbow and Wrist "<<elbow<<" "<<wrist<<" "<<m.size()<<"\n";
	mutex->lock();
	
	int k=0;
	Matrixd mOsg;
	//Hacemos una copia de la matriz de transformacion de osgART, que solo tiene rotacion, a la matriz del pecho del aprendiz
	for (int i=0; i < 3 ; i++) {
		for (int j=0; j < 3 ; j++) {		
		 mOsg(i,j)=m[k];		 
// 		 std::cout<<m[k]<<" ";
		 k++;
		}
// 		 std::cout<<"\n";
	}
	//sin olvidar que la matriz de osg tiene la translacion incluida en la ultima fila y antes de inicializar debemos añadirla.
	mOsg.setTrans( mT->accedeJoint("jPecho")->trf->getMatrix().getTrans() );
	
	mT->accedeJoint("jPecho")->trf->setMatrix(mOsg);
	
	qDebug()<<"ang X="<<atan2(mOsg(2,1),mOsg(2,2) );
	
	
// 	mT->mueveModelo(desplaZ);
	
	Matrixd mWrist =pos_mano_der->getMatrix();
	mWrist.setTrans(wrist);
	pos_mano_der->setMatrix(mWrist);
	
	Matrixd mElbow =pos_codo_der->getMatrix();
	mElbow.setTrans(elbow);
	pos_codo_der->setMatrix(mElbow);
	
	// "limpia" los colores de las articulaciones
    std::vector <int>::iterator kk;
    for (kk=pobs->_pares_colisionados.begin(); kk<pobs->_pares_colisionados.end(); kk++)
      {
	mT->devuelveColor(*(mT->_pares_colision[*kk].first));
	mT->devuelveColor(*(mT->_pares_colision[*kk].second)); 
      }   
    pobs->_pares_colisionados.clear();
    
	mT->mueveBrazoDer(wrist,elbow);

	mutex->unlock();
	
	
}

std::vector< double > Worker::getAnglesRight()
{
	return mT->getAngulosBrazoDer();
}

void Worker::getShoulders(RoboCompAprendiz::coor3D& leftShoulder, RoboCompAprendiz::coor3D& rightShoulder)
{
	Vec3d left, right;
	//coordenadas del hombro en superficie
	right= mT->obtieneDatosVirtualesCODAModelo3D()[11];
	left= mT->obtieneDatosVirtualesCODAModelo3D()[0];;
	leftShoulder.x =left.x();
	leftShoulder.y =left.y();
	leftShoulder.z =left.z();
	
	rightShoulder.x =right.x();
	rightShoulder.y =right.y();
	rightShoulder.z =right.z();
	
}
void Worker::getElbows(RoboCompAprendiz::coor3D& leftElbow, RoboCompAprendiz::coor3D& rightElbow)
{
	Vec3d left, right;// 	
	right= computeLocalToWorld(mT->accedeJoint("jAntebrazoDer")->nodepath).getTrans();
	left= computeLocalToWorld(mT->accedeJoint("jAntebrazoIzq")->nodepath).getTrans();
	leftElbow.x=left.x();
	leftElbow.y=left.y();
	leftElbow.z=left.z();
	
	rightElbow.x=right.x();
	rightElbow.y=right.y();
	rightElbow.z=right.z();
	
}
























