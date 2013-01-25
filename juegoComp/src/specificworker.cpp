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
 
#include "specificworker.h"

#define VIDEO 1
#define AR 0

/**
* \brief Default constructor
*/
void SpecificWorker::getJointsPosition(jointListType& myJointList)
{
	if (stateTrack==RoboCompTrackeropenni::Tracking)
	{		
		myJointList=jointList;	
	}
}
void SpecificWorker::getRTMatrixList(RTMatrixList& myRTMatList)
{
	if (stateTrack==RoboCompTrackeropenni::Tracking)
	{		
		myRTMatList=RTMatrixMap;		
	}
}

void SpecificWorker::poseLimpia()
{
	float z = -1000;
// 	arbol[0]=0;	arbol[1]=0;arbol[2]=z;
// 	cesta[0]=0;	cesta[1]=0;cesta[2]=z;
// 	manzana[0]=0;manzana[1]=0;manzana[2]=z;
	innerModel->updateTransformValues ("XN_SKEL_TORSO",0,300,z,-1.570000052,0,0);
	
	innerModel->updateTransformValues ("XN_ESCENARIO_ARBOL",arbol[0],arbol[1],z,1.570000052,0,0);
	innerModel->updateTransformValues ("XN_CESTA",cesta[0],cesta[1],z,1.570000052,0,0);
	innerModel->updateTransformValues ("XN_MANZANA_MOVIL",10000,10000,z,1.570000052,0,0);
	innerModel->updateTransformValues ("XN_MANZANA_ANIMADA",manzana[0],manzana[1],z,1.570000052,0,0);
}


void SpecificWorker::poseOpenNi()
{
	poseLimpia();
	innerModel->updateTransformValues("XN_SKEL_LEFT_SHOULDER",-250,0,200,0,-1.570000052,1.570000052);
	innerModel->updateTransformValues("XN_SKEL_RIGHT_SHOULDER",250,0,200,0,1.570000052,-1.570000052);
	innerModel->updateTransformValues("XN_SKEL_RIGHT_ELBOW",0,0,400,1.570000052,0,0);	
	innerModel->updateTransformValues("XN_SKEL_LEFT_ELBOW",0,0,400,1.570000052,0,0);	
	innerModel->updateTransformValues ("XN_SKEL_TORSO",0,300,2500,-1.570000052,0,0);	
	
	try {
		ursusarm->setMovement(RoboCompUrsusArm::openNiD);
	}
	catch (Ice::Exception e ){
			qDebug()<<"error talking to ursusarm->setMovement, in specificworker::posenopeni "<< e.what();
	}
	try {
		ursusarm->setMovement(RoboCompUrsusArm::openNiI);
	}
	catch (Ice::Exception e ){
			qDebug()<<"error talking to ursusarm->setMovement, in specificworker::posenopeni "<< e.what();
	}
	
	osg->setTextHUD("PONTE COMO YO");
	
}


SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)	
{
	alto=480;ancho=640;
	numEtapaActual = -1;	
	nombreObjetivo = "";
		
	cesta = QVec::vec3(-400,-800,2500);
	arbol = QVec::vec3(-350,-890,2500);
	manzana = QVec::vec3(-320,1000,2300);
	
	listaEtapas.clear();
	juegoCargado = false;
	dump=false;
	evaluador=false;
//	saveFile("dump.txt");
	///añadir ruta fichero al config y checkeo de parametros
	innerModel = new InnerModel("/home/robocomp/robocomp/Files/InnerModel/ejercicioHombroPoseNi.xml");
	
	// ----
	QGLFormat fmt;
	fmt.setDoubleBuffer(true);	
	QGLFormat::setDefaultFormat(fmt);

 	
	osg = new OsgView (frameOSG,true);
	osgHeight = frameOSG->height();
	osgWidth = frameOSG->width();
	imv = new InnerModelViewer(innerModel, "world", osg->getRootGroup());	
		
	//imgTypeRGB ice
	imgTypeRGB.resize(ancho*alto*3);
	imgTypeAR.resize(frameOSG->width()*frameOSG->height()*4);
	mutexImgAR = new QMutex ( );
	mutexRTMatrixMap = new QMutex ();
	mutexJointList = new QMutex ();
	
	osgImage = new osg::Image(); 
	osgImage.get()->allocateImage(ancho,alto, 1, GL_RGB, GL_UNSIGNED_BYTE, 1);
	connect(osg, SIGNAL(keyPress(QString)), this, SLOT(keyAction(QString)));
	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));	
	
// 	widgetFrame->setVisible(false);
	lastVisible = visible = VIDEO;
	setJuego("Reposo.");
	this->Period=33.3;
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker(){}
void SpecificWorker::closeEvent(QCloseEvent *event)
{
	 
	if (dump)
	{
		 qDebug()<<"close file";
		 dumpFile.close();
	}
	killYourSelf();
}


void SpecificWorker::compute( )
{
	switchFrame();
	try
	{
		trackeropenni->getRGB(imgTypeRGB,stateTrack);		
		osgImage.get()->setImage(640,480,1,GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, &imgTypeRGB[0], osg::Image::NO_DELETE, 1);		
		osg->setImageHUD(osgImage);			
		
		mutexImgAR->lock();		
		memcpy(&imgTypeAR[0],osg->grabFrameBuffer().bits(),osg->grabFrameBuffer().width()*osg->grabFrameBuffer().height()*4);
		mutexImgAR->unlock();
				
	}
	catch (Ice::Exception e )
	{
		qDebug()<<"Error talking to trackeropenni"<<e.what();
	}

	//RTMAP
	mutexRTMatrixMap->lock();
	try {
		if(evaluador)
		  evaluadoroff->getRTMatrixList(RTMatrixMap);
		else
		{
		  trackeropenni->getRTMatrixList(RTMatrixMap);
		}
	}	
	catch (Ice::Exception e ){
		qDebug()<<"Error talking to trackeropenni, getRTMatrixList"<<e.what();
	}
	mutexRTMatrixMap->unlock();		  
		
	///CHECK STATE TRACK
	osg->setTextHUD(stateTrackToQstring(stateTrack).toStdString(),osg::Vec3(320,10,0));
	if (stateTrack == RoboCompTrackeropenni::Tracking ){
			updateJoints();			
	}
	
	
	
	//jointlist
	mutexJointList->lock();
	try {		
		trackeropenni->getJointsPosition(jointList);		
	}
	catch (Ice::Exception e ){
		qDebug()<<"Error talking to trackeropenni, getJointsPosition"<<e.what();
	}
	mutexJointList->unlock();
	

	// ------ Pablo M. ------------
	// En este momento OSG e Innermodel están actualizados con la pose real.
	// Ahora empezaría la lógica del juego que, según creo, debe ser:
	// - Si no estamos en ninguna etapa coger la primera -> ESTO DEBE HACERSE AL CARGAR EL JUEGO
	// - Comprobar si el modelo está en la posición de fin de etapa.
	// - Si FIN DE ETAPA:
	//	* Guardar el tiempo que se ha tardado en completar la etapa.
	//	* Lanzar frase de fin de etapa (de haberla).
	//	* Registrar los datos que sean necesarios en ficheros externos.
	//  	* Sacar siguiente etapa de la lista.
	// - Si NO FIN DE ETAPA:
	//	* Comprobar la correcta ejecución del ejercicio
	//	* Si INCORRECTO:
	//		~ Lanzar frase de ejercicio incorrecto.
	//		~ Anotar ejercicio cancelado/mal hecho
	//		~ Resetear ejercicio.
	//	* Si CORRECTO:
	//		~ Comprobar el tiempo que lleva la etapa en relación con las anteriores.
	//		~ De superar el límite establecido (de haberlo) lanzar frase disparada por tiempo (de haberla).
	// ------ \Pablo M. ------------


	if(juegoCargado)
	{

// 	  InnerModelTransform* imTransform;
// 	  imTransform=innerModel->getTransform("XN_SKEL_ELBOW_LEFT");// <- Meter aquí el ID de la mano o de lo que sea
	  QVec posManoEnElMundo = innerModel->transform("world",QVec::vec3(0.,0.,0.),"XN_SKEL_LEFT_HAND");	  	  
	  if(etapaActual->finEtapa(posManoEnElMundo))
	  {
	    //Gardar tiempo -> ¿Cómo se hace esto?
		qDebug()<<"------";
	    qDebug()<<etapaActual->getFraseFin();// -> Mandar al speech por si no le gusta a marcos su gram.	
		qDebug()<<"------";
		
		///speechgenerator dice sus frases
		try {
			speechgenerator->setState(etapaActual->getFraseFin().toStdString());
		}
		catch (Ice::Exception e ){
			qDebug()<<"Error to talking speechgenerator:"<<e.what();
		}
		
		///el oso solo sube y baja y se puede interrumpir el moviemiento.
		RoboCompUrsusArm::Movement m;		
		qDebug()<<etapaActual->getId();
		if (etapaActual->getId() == "Flexión hombro Izquierdo.") {
			qDebug()<<"---"<<etapaActual->getId();
			qDebug()<<"---"<<etapaActual->getFraseFin();
			qDebug()<<"---";
			if (etapaActual->getFraseFin() == "subir")
				m =  RoboCompUrsusArm::hombroDS;
			else if (etapaActual->getFraseFin() == "bajar")
				m =  RoboCompUrsusArm::hombroDB;
		}
		if (etapaActual->getId() == "Flexión y extensión de codo Izquierdo.") {
			
			if (etapaActual->getFraseFin() == "subir")
				m =  RoboCompUrsusArm::codoDS;					
			else if (etapaActual->getFraseFin() == "bajar")
				m =  RoboCompUrsusArm::codoDB;					
		}
		try {
			ursusarm->setMovement(m);
		}
		catch (Ice::Exception e ){
			qDebug()<<"Error to talking Ursus Arm:"<<e.what();
		}
		
		
	    //Registrar algo si hace falta
	   
	    numEtapaActual++;
					
	    if(numEtapaActual < listaEtapas.size()){
	      etapaActual = listaEtapas.at(numEtapaActual);
	      if(nombreObjetivo != "")
			innerModel->updateTranslationValues(nombreObjetivo,0,0,-1000000);
	      nombreObjetivo = etapaActual->getNombreObjetivo();
	      QVec f = etapaActual->getPosFin();
	      if(etapaActual->getSeguirObjetivo())
	      {
			innerModel->updateTranslationValues(nombreObjetivo,posManoEnElMundo[0],posManoEnElMundo[1],posManoEnElMundo[2]);
	      }
	      else
	      {
			innerModel->updateTranslationValues(nombreObjetivo,f[0],f[1],f[2]);
	      }		
	    }
	    else
	    {
		  ///en continuo
		  reloadJuego();
//	      qDebug()<<"-----------------------------------------------";
	    }
	  }
 	  else
	  {
		
	    if(etapaActual->getSeguirObjetivo())
	    {
			innerModel->updateTranslationValues(nombreObjetivo,posManoEnElMundo[0],posManoEnElMundo[1],posManoEnElMundo[2]);		
	    }    	    
	  }

	}
	else
	{
//	  qDebug()<<"No hay juego cargado";
	}

	imv->update();			
}
/**
* \brief actualiza la posición de los joints. Si está activo el volcado de datos los guarda en el fichero.
* La lista de valores por nodo, representa, primero la transalacion(x,y,z), luego la rotacion rx,ry,rz.
*/

void SpecificWorker::updateJoints()
{
	///copia y actualizacion del innerModel
	map<string,std::vector<float> >::reverse_iterator it;
	for ( it=RTMatrixMap.rbegin() ; it != RTMatrixMap.rend(); it++ )
	{
        	QString name = QString::fromStdString( (*it).first);						
		///por si no queremos poner el tracker en funcionamiento. y no pintar el muñeco.
		if(evaluador)
		  innerModel->updateRotationValues(name,(*it).second[3],(*it).second[4],(*it).second[5]);
		else		
		///escondemos el muñeco.
		  innerModel->updateTransformValues (name,(*it).second[0],(*it).second[1],(*it).second[2],(*it).second[3],(*it).second[4],(*it).second[5]);					

		if (name =="XN_SKEL_TORSO" && juegoCargado){
			osg->setTextHUD(stateTrackToQstring(stateTrack).toStdString(),osg::Vec3(320,10,0));
		
// 			QVec f = etapaActual->getPosFin();
// 			f[2]=(*it).second[2];
// 			etapaActual->setPosFin(f);
			
			///movimiento del objeto en relacion al paciente. Cuando el objeto (manzana) está fijo, cuando es parte de la escenografía.
			/*innerModel->updateTranslationValues(nombreObjetivo,innerModel->getTransform(nombreObjetivo)->getTr().x(),innerModel->getTransform(nombreObjetivo)->getTr().y(),	(*it).second[2]	); */									
			
		}
		///si estamos grabando volcamos los datos
		if (dump)
		{		
			dumpFile<<(*it).first<<" "
				<<(*it).second[3]<<" "
				<<(*it).second[4]<<" "
				<<(*it).second[5]<<"\n";
		}
	}
	if (dump)
		dumpFile<<"\n";
}
QString SpecificWorker::stateTrackToQstring( const RoboCompTrackeropenni::trackingState s)
{
	
	if (s == RoboCompTrackeropenni::NoTracking )
			return QString("Sin Usuario" );
	if (s == RoboCompTrackeropenni::Calibrating ){
			static bool primera = false;
			if (primera == false){
			try {
				speech->say(QString::fromUtf8("Aguanta, un poco más.").toStdString(),false);
				primera = true;
			}
			catch (Ice::Exception e ){
				qDebug()<<"Errot talking to speech"<<e.what();
			}
			
			}
			return QString("Calibrando" );
	}
	if (s == RoboCompTrackeropenni::LookingForPose ){		
			return QString("Buscando pose" );
	}
	if (s == RoboCompTrackeropenni::Tracking ){
		static bool primeraTracking = false;
		if (primeraTracking == false){
			try {
				ursusarm->goRestPosition("left");
				ursusarm->goRestPosition("right");
				primeraTracking = true;
			}
			catch (Ice::Exception e ){
				qDebug()<<"Errot talking to ursusarm->goRestPosition"<<e.what();
			}
		}
		
		return QString("Siguiendo" );	
	}
	
	return QString("trackingState ERROR");
}

void SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	timer.start(Period);
}

void SpecificWorker::getARimg(imgType& img,int &width,int &height)
{
	mutexImgAR->lock();
		img=imgTypeAR;
		width = frameOSG->width();
		height = frameOSG->height();
	mutexImgAR->unlock();
}
///std::string text must be in UTF-8, since the codification in this file is utf-8
bool SpecificWorker::setJuego(const std::string& text)
{
	Etapa* e;
	listaEtapas.clear();
	
	///arrancamos gramar
	try {
			speechgenerator->setState("stop");
		}
	catch (Ice::Exception e ){
			qDebug()<<"Error talking to speechgenerator"<<e.what();		
	}
	
	if(QString::fromStdString(text) == "Reposo.")
	{
		try {
			ursusarm->goRestPosition("left");
			ursusarm->goRestPosition("right");
		}
		catch (Ice::Exception e) {			
			qDebug()<<"error talking to ursusarm setJuego"<<e.what();
		}		
		juegoCargado = false;	
		poseLimpia();
		return false;
	}
	if(QString::fromStdString(text) == "Bienvenida.")
	{
		qDebug()<<"Entrenamiento.";
		try {
		  speech->say("Hola, soy Ursus. Estoy muy contento de verte.", false);
		}
		catch ( Ice::Exception e ){
		  qDebug()<<"Error talking to speech"<<e.what();		  
		}		
		try {
		  ursusarm->loadMovement("holaIzq.txt","left");
		  ursusarm->reloadMovement("left");
		}
		catch ( Ice::Exception e ){
		  qDebug()<<"Error talking to ursusarm"<<e.what();		  
		}
		
		juegoCargado = false;	
		poseLimpia();
		return false;
	}
	
	else if(QString::fromStdString(text) == "Entrenamiento.")
	{		
	  qDebug()<<"Entrenamiento.";
	  try {
		  speech->say("Ponte como yo.", false);
	  }
	  catch ( Ice::Exception e ){
		  qDebug()<<"Error talking to speech"<<e.what();
		  
	  }
	  poseOpenNi( );
	  juegoCargado = false;
	  
	return false;
	}
	
	if (stateTrack ==RoboCompTrackeropenni::NoTracking){
		poseLimpia();
		juegoCargado=false;		
		try {
		  speech->say("No podemos jugar. No te veo.", false);
		}
		catch ( Ice::Exception e ){
			qDebug()<<"Error talking to speech"<<e.what();		  
		}
		///reset pose
		try {
			ursusarm->goRestPosition("left");
			ursusarm->goRestPosition("right");
		}
		catch (Ice::Exception e) {			
			qDebug()<<"error talking to ursusarm setJuego"<<e.what();
		}
		return false;
	}
		
	else if(QString::fromStdString(text) == "Flexión hombro Derecho.")
	{	
// 		poseLimpia();
// 		
// 		
// 		e = new Etapa(QString::fromStdString(text),QString("Empieza etapa 1 Hombro"),QString("bajar"),QString("Tiempo etapa 1 Hombro"),QVec::vec3(manzana[0],manzana[1],manzana[2]),500,10,"XN_MANZANA_ANIMADA",false);
// 		listaEtapas.append(e);
// 		// 	  InnerModelTransform * z = innerModel->getTransform("XN_MANZANA_MOVIL");
// 		e = new Etapa(QString::fromStdString(text),QString("Empieza etapa 2 Hombro"),QString("subir"),QString("Tiempo etapa 2 Hombro"),QVec::vec3(cesta[0],cesta[1],cesta[2]),300,10,"XN_MANZANA_MOVIL",true);
// 		listaEtapas.append(e);
// 		
// 		///Escenografía movimiento en Z
// 		innerModel->updateTranslationValues("XN_ESCENARIO_ARBOL",arbol[0],arbol[1],arbol[2]);
// 		innerModel->updateTranslationValues("XN_CESTA",cesta[0],cesta[1],cesta[2]);
// 		innerModel->updateTranslationValues("XN_MANZANA_ANIMADA",manzana[0],manzana[1],manzana[2]);
		
	}
	if(QString::fromStdString(text) == "Flexión hombro Izquierdo.")
	{	
		poseLimpia();
		cesta[0]=-400;cesta[1]=-700;cesta[2]=2500;
		arbol[0]=-350;arbol[1]=-890;arbol[2]=2500;
		manzana[0] =-320;manzana[1] =1000;manzana=2300;
		
		e = new Etapa(QString::fromStdString(text),QString("Empieza etapa 1 Hombro"),QString("bajar"),QString("Tiempo etapa 1 Hombro"),QVec::vec3(manzana[0],manzana[1],manzana[2]),500,10,"XN_MANZANA_ANIMADA",false);
		listaEtapas.append(e);
		
		e = new Etapa(QString::fromStdString(text),QString("Empieza etapa 2 Hombro"),QString("subir"),QString("Tiempo etapa 2 Hombro"),QVec::vec3(cesta[0],cesta[1],cesta[2]),500,10,"XN_MANZANA_MOVIL",true);
		listaEtapas.append(e);
				
		
		///Escenografía movimiento en Z		
		innerModel->updateTranslationValues("XN_ESCENARIO_ARBOL",arbol[0],arbol[1],arbol[2]);
		innerModel->updateTranslationValues("XN_CESTA",cesta[0],cesta[1],cesta[2]);
		innerModel->updateTranslationValues("XN_MANZANA_ANIMADA",manzana[0],manzana[1],manzana[2]);
	}
	else if(QString::fromStdString(text) == "Flexión y extensión de codo Derecho.")
	{
// 		qDebug()<<"Cargando juego Codo";
// 		e = new Etapa(QString::fromStdString(text),QString("Empieza etapa 1 Codo"),QString("bajar"),QString("Tiempo etapa 1 Codo"),QVec::vec3(-250,500,0),500,10,"XN_MANZANA_ANIMADA",false);
// 		listaEtapas.append(e);
// 		e = new Etapa(QString::fromStdString(text),QString("Empieza etapa 2 Codo"),QString("subir"),QString("Tiempo etapa 2 Codo"),QVec::vec3(-250,-400,0),400,10,"XN_MANZANA_MOVIL",true);
// 		listaEtapas.append(e);
	}
	else if(QString::fromStdString(text) == "Flexión y extensión de codo Izquierdo.")
	{
		poseLimpia();
		cesta[0]=-400;cesta[1]=-800;cesta[2]=2500;
		arbol[0]=-350;arbol[1]=-890;arbol[2]=2500;
		manzana[0] =-320;manzana[1] =500;manzana=2300;
		
		e = new Etapa(QString::fromStdString(text),QString("Empieza etapa 1 Codo"),QString("bajar"),QString("Tiempo etapa 1 Codo"),QVec::vec3(manzana[0],manzana[1],manzana[2]),500,10,"XN_MANZANA_ANIMADA",false);
		listaEtapas.append(e);
		e = new Etapa(QString::fromStdString(text),QString("Empieza etapa 2 Codo"),QString("subir"),QString("Tiempo etapa 2 Codo"),QVec::vec3(cesta[0],cesta[1],cesta[2]),500,10,"XN_MANZANA_MOVIL",true);
		listaEtapas.append(e);
		
		
		innerModel->updateTranslationValues("XN_ESCENARIO_ARBOL",arbol[0],arbol[1],arbol[2]);
		innerModel->updateTranslationValues("XN_CESTA",cesta[0],cesta[1],cesta[2]);
		innerModel->updateTranslationValues("XN_MANZANA_ANIMADA",manzana[0],manzana[1],manzana[2]);
	}	  
    else {
		return false;
	}
	if (juegoCargado){
		///reset pose
		try {
		ursusarm->goRestPosition("left");
		ursusarm->goRestPosition("right");
		}
		catch (Ice::Exception e) {			
			qDebug()<<"error talking to ursusarm setJuego"<<e.what();
		}
	}
	///comun a todos los juegos
	reloadJuego();		
	return true;
}
void SpecificWorker::reloadJuego()
{
	numEtapaActual=0;
	etapaActual = listaEtapas.at(numEtapaActual);
	///si no es la primera etapa	
	if(nombreObjetivo != "")
	  innerModel->updateTranslationValues(nombreObjetivo,0,0,-1000000);
	nombreObjetivo = etapaActual->getNombreObjetivo();
	qDebug()<<"nombreObjetivo, pos fin:"<<nombreObjetivo<<etapaActual->getPosFin();
	innerModel->updateTranslationValues(nombreObjetivo,etapaActual->getPosFin().x(),etapaActual->getPosFin().y(),etapaActual->getPosFin().z());
	juegoCargado = true;
	
	///arrancamos gramar
	try {
		speechgenerator->setState("start");
	}
	catch (Ice::Exception e ){
		qDebug()<<"Error talking to speechgenerator"<<e.what();		
	}
	
	
}


bool SpecificWorker::saveFile(const std::string& name, const std::string& header)
{
	if (dump == false){
		QString nameAux =QString::fromStdString(name);
		if (!nameAux.endsWith(".txt") )
			nameAux=nameAux+".txt";
		dumpFile.open(nameAux.toStdString().c_str(),std::ios::out | std::ios::trunc );
		dump = true;		
		
		dumpFile<<header;
		return true;
	}
	return false;
}

void SpecificWorker::stopRecording()
{
	if (dump ){
		dumpFile.close();
		dump=false;
	}
}

void SpecificWorker::playVideo(const std::string& path)
{
	QString myPath;
	Phonon::MediaSource comon(QString::fromStdString(path));
  
	myPath = QString::fromStdString(path);
	qDebug()<<"***** "<<myPath;
//   videoPlayer->load(myPath);
	videoPlayer->load(comon);
	videoPlayer->play();
  
}

void SpecificWorker::makeARVisible()
{
	visible = AR;
	if(videoPlayer->isPlaying())
		videoPlayer->stop();
}

void SpecificWorker::switchFrame()
{
	if(visible != lastVisible)
	{
		lastVisible = visible;
		stackedWidget->setCurrentIndex((int)visible);
	}
}

void SpecificWorker::makeVideoVisible()
{
	visible = VIDEO;
}

void SpecificWorker::pauseVideo()
{
	if(videoPlayer->isPlaying())
		videoPlayer->pause();
}

void SpecificWorker::stopVideo()
{
	if(videoPlayer->isPlaying())
		videoPlayer->stop();
}  

void SpecificWorker::playAr()
{

}

void SpecificWorker::keyAction(QString key)
{
  qDebug()<<"#####################################################################"<<key;
}

void SpecificWorker::invisible(){

//   /*  qDebug()<<"------------------------------";
//     if(visible == VIDEO){
//       visible = AR;
//       widgetVideo->setVisible(false);
//       widgetFrame->setVisible(true);
//     }
//     else{
//       visible = VIDEO;
//       widgetFrame->setVisible(false);
//       widgetVideo->setVisible(true);
//     }*/
}
void SpecificWorker::getGameRange(string game, float & min, float & max)
{
	if (listaEtapas.size() > 0 ) {
		min=listaEtapas[0]->getRangoError();
		max=listaEtapas[1]->getRangoError();
	}
}
void SpecificWorker::setGameRange(string game, float min, float max)
{
	if (listaEtapas.size() > 0 ) {
		listaEtapas[0]->setRangoError(min);
		listaEtapas[1]->setRangoError(max);
		qDebug()<<listaEtapas[0]->getRangoError();	
	}
}

void SpecificWorker::getSceneObjectPosition(const std::string& object, int& x, int& y, int& z)
{	
	if(object == "XN_ESCENARIO_ARBOL") {
	x = innerModel->getTransform (QString::fromStdString(object))->backtX - arbol[0];
	y = innerModel->getTransform (QString::fromStdString(object))->backtY - arbol[1];
	z = innerModel->getTransform (QString::fromStdString(object))->backtZ - arbol[2];	
	}
	
	if(object == "XN_CESTA") {
	x = innerModel->getTransform (QString::fromStdString(object))->backtX - cesta[0];
	y = innerModel->getTransform (QString::fromStdString(object))->backtY - cesta[1];
	z = innerModel->getTransform (QString::fromStdString(object))->backtZ - cesta[2];	
	}
	
	if(object == "XN_MANZANA_ANIMADA") {
	x = innerModel->getTransform (QString::fromStdString(object))->backtX - manzana[0];
	y = innerModel->getTransform (QString::fromStdString(object))->backtY - manzana[1];
	z = innerModel->getTransform (QString::fromStdString(object))->backtZ - manzana[2];	
	}
}
void SpecificWorker::setSceneObjectPosition(const std::string& object, int x, int y, int z)
{
	
	if(object == "XN_ESCENARIO_ARBOL")
		innerModel->updateTransformValues ("XN_ESCENARIO_ARBOL",arbol[0]+x,arbol[1]+y,arbol[2]+z,1.570000052,0,0);
	if(object == "XN_CESTA"){
		if (listaEtapas.size() > 0 ){
			listaEtapas[1]->setPosFin(QVec::vec3(cesta[0]+x,cesta[1]+y,cesta[2]+z));
		}
		innerModel->updateTransformValues ("XN_CESTA",cesta[0]+x,cesta[1]+y,cesta[2]+z,1.570000052,0,0);
	}
	if(object == "XN_SKEL_TORSO")
		innerModel->updateTransformValues ("XN_SKEL_TORSO",x,y,z,-1.570000052,0,0);
	if(object == "XN_MANZANA_ANIMADA"){		
		if (listaEtapas.size() > 0 ){
			listaEtapas[0]->setPosFin(QVec::vec3(manzana[0]+x,manzana[1]+y,manzana[2]+z));
		}
		innerModel->updateTransformValues ("XN_MANZANA_ANIMADA",manzana[0]+x,manzana[1]+y,manzana[2]+z,1.570000052,0,0);	
	}
}

void SpecificWorker::resizeEvent(QResizeEvent* event)
{
//     QWidget::resizeEvent(event);
	double  f,a,zn,zf;
	
	osg->getCamera()->getProjectionMatrixAsPerspective(f,a,zn,zf);		
	osg->getCamera()->setProjectionMatrixAsPerspective(f,1.0,zn,zf);
	imgTypeAR.resize(frameOSG->width()*frameOSG->height()*4);
	osg->resize(frameOSG->width(),frameOSG->height());					
	qDebug()<<frameOSG->width()<<frameOSG->height();
}

