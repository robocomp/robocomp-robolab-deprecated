/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "juego2.h"

Juego2::Juego2(osg::ref_ptr< osg::Camera > _cam, osg::ref_ptr< osgART::Tracker > _tracker, RoboCompArmrocio::ArmrocioPrx _arm, RoboCompSpeech::SpeechPrx _speech, QFrame* parent)
{
  cam=_cam;
  tracker=_tracker;
  arm=_arm;  
  speech = _speech;
  
  //posición incial de las bolas, roja y verde y umbra.
  arriba = true;
  mThreshold = 200.f;
  rojo.set (1.0f, 0.0f, 0.0f, 1.0f);  
  verde.set (0.0f, 1.0f, 0.0f, 1.0f);
  //bola roja posicion relativa por arriba del marcador HIRO
  pBolaRoja.set(200,250,0);
  //bola verde posicion relativa por arriba del marcador HIRO
  pBolaVerde.set(200,-250,0);

  vLayout = new QVBoxLayout(parent); 
  
  pbBinarize = new QPushButton ("Binarizar"); 
  
  vLayout->addWidget(pbBinarize);
  
  lUmbral = new QLabel ("Umbral: ");
  lUmbral->setGeometry(0,0,200,25);
  hLUmbral = new QHBoxLayout ();
  hLUmbral->addWidget(lUmbral);
  
  lcdUmbral = new QLCDNumber ();
  lcdUmbral->setSegmentStyle(QLCDNumber::Flat);
  lcdUmbral->display(mThreshold);
  hLUmbral->addWidget(lcdUmbral);
  
  vLayout->addLayout(hLUmbral);  
  
  sliderUmbral = new QSlider(Qt::Horizontal);
  sliderUmbral->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
  sliderUmbral->setRange(100,500);
  sliderUmbral->setValue(mThreshold);
  vLayout->addWidget(sliderUmbral);
   
  //botones fijado
  pbFijaVerde = new QPushButton ("Fijar Verde");  
  pbFijaRoja = new QPushButton ("Fijar Roja");  
  pbConseguido = new QPushButton ("Conseguido");
  
  
  vLayout->addWidget(pbFijaVerde);
  vLayout->addWidget(pbFijaRoja);
  vLayout->addWidget(pbConseguido);
  lArm = new QLabel("Brazo: ");
  vLayout->addWidget(lArm);
  
  hLArm = new QHBoxLayout ();  
  rbArriba = new QRadioButton("Arriba");
  hLArm->addWidget(rbArriba);  
  rbReposo = new QRadioButton("Reposo");
  hLArm->addWidget(rbReposo);  
  rbAbajo = new QRadioButton("Abajo");
  hLArm->addWidget(rbAbajo);  
  rbBucle = new QRadioButton("Bucle");
  hLArm->addWidget(rbBucle);
  
  vLayout->addLayout(hLArm);
  vLayout->addStretch(5);
  
  connect (pbConseguido, SIGNAL (clicked(bool)) ,this,SLOT (conseguido(bool)) );	
  connect (sliderUmbral,SIGNAL (valueChanged(int)),this,SLOT(vSlider(int)) );   
  connect(pbFijaVerde,SIGNAL (clicked(bool)),this,SLOT (fijaVerde(bool)));
  connect(pbFijaRoja,SIGNAL (clicked(bool)),this,SLOT (fijaRoja(bool)));
  connect(rbArriba,SIGNAL (clicked(bool)),this, SLOT (armArriba(bool)) );
  connect(rbReposo,SIGNAL (clicked(bool)),this, SLOT (armReposo(bool)) );
  connect(rbAbajo,SIGNAL (clicked(bool)),this, SLOT (armAbajo(bool)) );
  connect(rbBucle,SIGNAL (clicked(bool)),this, SLOT (armLoop(bool)) );
    
  //marcadores utilizados
  marker = tracker->addMarker("single;data/patt.hiro;160;0;0");
  marker2 = tracker->addMarker("single;data/patt.play;85;0;0");

	if (!marker.valid() or !marker2.valid())  
		qFatal("Could not add marker!");	
    
	marker->setActive(true);
	marker2->setActive(true);
// 	osgART::addEventCallback(root, new osgART::MarkerDebugCallback(marker));
	
	arTransform = new osg::MatrixTransform();
	osgART::attachDefaultEventCallbacks(arTransform.get(),marker.get());		
	//bola verde
	arTransform->addChild(ball(60.f));
	//bola azul sobre el marcador
	arTransform->addChild(ball(60.f, osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)));		  
	
	arTransform2 = new osg::MatrixTransform();
	osgART::attachDefaultEventCallbacks(arTransform2.get(),marker2.get());
	arTransform2->addChild(testCube(50.0));
	
	arTransform->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");	
	arTransform2->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");	
	
	//camera
	cam->addChild(arTransform.get());	
	cam->addChild(arTransform2.get());
	
	changePosition();    
}

void Juego2::armArriba(bool b)
{
	try {
		arm->setPose(RoboCompArmrocio::game2Up);
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}
}
void Juego2::armReposo(bool b)
{
	try {
		arm->setPose(RoboCompArmrocio::reposo);
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}

}
void Juego2::armAbajo(bool b)
{
	try {
		arm->setPose(RoboCompArmrocio::game2Down);
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}

}

void Juego2::armLoop(bool b)
{
  QTime t;
  
  while(rbBucle->isChecked())
  {
	try {
		arm->setPose(RoboCompArmrocio::game2Up);
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}
	t.start();
	while(t.elapsed()<4000)
	{
	  if(!arm->isMoving())
	  {
	    break;
	  }
	  QApplication::processEvents();
	  usleep(1000);
	}
	try
	{
		arm->setPose(RoboCompArmrocio::game2Down);
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}
	t.restart();
	while(t.elapsed()<4000)
	{
	  if(!arm->isMoving())
	  {
	    break;
	  }
	  QApplication::processEvents();
	  usleep(1000);
	}
  }
  try
  {
	  arm->setPose(RoboCompArmrocio::reposo);
  }
  catch (Ice::Exception e ) {
	  qDebug()<<"Error talking to Arm"<<e.what();
  }
}


void Juego2::changePosition()
{
  //centro de la bola
  osg::Vec3 centro = dynamic_cast<osg::Sphere*> (dynamic_cast<osg::ShapeDrawable*> (dynamic_cast<osg::Geode*>(arTransform->getChild(0))->getDrawable(0))->getShape() )->getCenter();
  osg::Vec4 color;
  if (arriba )
  {
	//la pongo abajo en verde
// 	centro.set (Xb, Yb,Zb );	
	try {
		arm->setPose(RoboCompArmrocio::game2Down);		
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}
	
	try {
		QString s (QString::fromUtf8("<prosody range=\"x-high\"> ¡Abajo!.</prosody>" ));	
		speech->say( s.toStdString());
	}
	catch ( Ice::Exception e) {
		qDebug()<<"Error talking to Speech"<<e.what();
	}
	centro.set (pBolaVerde);
	color.set (0.0f, 1.0f, 0.0f, 1.0f);    
  }
  else
  {
	//la pongo arriba en rojo
	try {
		arm->setPose(RoboCompArmrocio::game2Up);		
	}
	catch (Ice::Exception e ) {
		qDebug()<<"Error talking to Arm"<<e.what();
	}
	
	try {
		QString s (QString::fromUtf8("<prosody range=\"x-high\"> ¡Arriba!.</prosody>" ));	
		speech->say( s.toStdString());
	}
	catch ( Ice::Exception e ){
		qDebug()<<"Error talking to Speech"<<e.what();
	}
	centro.set (pBolaRoja);		
	color.set (1.0f, 0.0f, 0.0f, 1.0f);	
  }
  arriba = not arriba;
  updateBola(centro,color);

}

/**
Los cálculos se hacen relativos a la matriz del marcador hiro, es de tipo osg::Matrixd, de 4x4, donde las matriz de rotación R propiamente dicha, está representa, en forma traspuesta R^t en la submatriz de 3x3,
quedando la última fila para representar las coordenadas del centro del marcador desde la camara. Esta posición se puede solicitar directamente con la función osg::Matrixd::getTrans()
Matrixd representación:
		| [R^t] 0 |
		| [T  ] 1 |

*/
void Juego2::computeGame()
{
	 if (!marker->valid() or !marker2->valid() ) return;
	 
	//posicion de los marcadores desde la cámara. 
	osg::Vec3 pHiro= arTransform->getMatrix().getTrans(); 
	osg::Vec3 pCubo= arTransform2->getMatrix().getTrans();	

	QMat R(3,3); //matriz rotacion del hiro
	QVec qvecHiro(3);
	QVec qvecBola(3);
	QVec qvecCubo(3);
	QVec bolaDesdeCam(3);
	
	qvecHiro(0)=pHiro.x(); qvecHiro(1)=pHiro.y(); qvecHiro(2)=pHiro.z();
	qvecCubo(0)=pCubo.x(); qvecCubo(1)=pCubo.y(); qvecCubo(2)=pCubo.z();
	
	for ( int i=0; i<3; i++)
	{
		for ( int j=0; j<3; j++)
		{	
 			R(i,j)=arTransform->getMatrix()(i,j);
		}
	}	
	 
	if (arriba) 
	{					
// 		posBola.set (posHiro + pBolaRoja);
		qvecBola(0)=pBolaRoja.x(); qvecBola(1)=pBolaRoja.y(); qvecBola(2)=pBolaRoja.z(); 
	} 
	else 
	{
// 		posBola.set(posHiro + pBolaVerde);
		qvecBola(0)=pBolaVerde.x(); qvecBola(1)=pBolaVerde.y(); qvecBola(2)=pBolaVerde.z(); 	 
	}
	
	bolaDesdeCam=R.transpose()*qvecBola + qvecHiro; 
	
  qvecHiro.print( "posición del HIRO: ");  
  qvecCubo.print(" posición del CUBO: ");
  qDebug()<<"bola verde";pose(pBolaVerde);
  qDebug()<<"bola roja";pose(pBolaRoja);
  bolaDesdeCam.print (" posición de la bola desde la camara OBJETIVO: "  );
  qvecBola.print("posición de la bola desde HIRO");
  
    
  QVec offset = bolaDesdeCam - qvecCubo;
  float distance = offset.norm2();

   std::cout<<"distance: "<<distance<<std::endl;
   std::cout<<"-------------"<<std::endl;
	
  if (distance <=mThreshold ) 
	{				  
	  changePosition(); 				 
	}

}


osg::Geode* Juego2::ball (float size, osg::Vec4 color)
{
	osg::Sphere *ball = new osg::Sphere( osg::Vec3(0,0,size/2.0), size);
	osg::ShapeDrawable* sphereDrawable = new osg::ShapeDrawable( ball );
	osg::Geode* shapeGeode = new osg::Geode();
	sphereDrawable->setColor(color);	
	shapeGeode->addDrawable( sphereDrawable );
	return shapeGeode;
}

/** 
 * Creates and returns an osg::Geode containing a simple box of the specified size.
 * \param	size	The edge length of the cube. Default value is 80mm.
 * \param	color	The color of the cube. Default value is white.
 * \return	The osg::Geode containing the cube shape.
 */
osg::Geode* Juego2::testCube(float size, osg::Vec4 color) {
	osg::Geode* geode = new osg::Geode();
	osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0, 0, size / 2.0f), size));
	sd->setColor(color);
	geode->addDrawable(sd);
	return geode;
}

void Juego2::updateBola (osg::Vec3 centro, osg::Vec4 color)
{
	//ñapa transición al amarillo
	osg::Vec4 amarillo (1.0f, 1.0f, 0.0f, 1.0f);    
	dynamic_cast<osg::ShapeDrawable*> (dynamic_cast<osg::Geode*>(arTransform->getChild(0))->getDrawable(0))->setColor(amarillo); 	
	///
	dynamic_cast<osg::ShapeDrawable*> (dynamic_cast<osg::Geode*>(arTransform->getChild(0))->getDrawable(0))->setColor(color); 
	dynamic_cast<osg::Sphere*> (dynamic_cast<osg::ShapeDrawable*> (dynamic_cast<osg::Geode*>(arTransform->getChild(0))->getDrawable(0))->getShape() )->setCenter(centro);	
}

void Juego2::vSlider(int v)
{
	mThreshold = v;
	lcdUmbral->display(mThreshold);
}

//fija la posicion de la bola donde se encuentra el marcador del cubo.
QVec Juego2::fijador()
{
	osg::Vec3 pHiro= arTransform->getMatrix().getTrans(); //T
	osg::Vec3 pCubo= arTransform2->getMatrix().getTrans(); //y
	std::cout<<"phiro: "; pose(pHiro);
	std::cout<<"pCubo: ";pose(pCubo);	
	//matriz del hiro
// 	osg::Matrix3 R;
	QMat R(3,3);
	QVec qvecHiro(3);
	QVec qvecCubo(3);
	QVec qvecBola(3);
	
	qvecHiro(0)=pHiro.x(); qvecHiro(1)=pHiro.y(); qvecHiro(2)=pHiro.z();
	qvecCubo(0)=pCubo.x(); qvecCubo(1)=pCubo.y(); qvecCubo(2)=pCubo.z(); 
	
	for ( int i=0; i<3; i++)
	{
		for ( int j=0; j<3; j++)
		{	
 			R(i,j)=arTransform->getMatrix()(i,j);
		}
	}
	qvecBola=R* (qvecCubo-qvecHiro);
	return qvecBola;
}

void Juego2::fijaRoja(bool b )
{
	if (!marker->valid() or !marker2->valid() ) 
	{
		QMessageBox b;
		b.warning(vLayout->parentWidget(),QString::fromUtf8("Atención"),QString::fromUtf8("Uno de los dos marcadores no se vé."));
		return;
	}	
	QVec Bola(3);
	Bola=fijador();
	pBolaRoja.set(Bola(0),Bola(1),Bola(2));
	qDebug()<<"fijar en rojo";
	pose(pBolaRoja);
	updateBola(pBolaRoja,rojo);
}

void Juego2::fijaVerde(bool b)
{
	if (!marker->valid() or !marker2->valid() ) 
	{
		QMessageBox b;
		b.warning(vLayout->parentWidget(),QString::fromUtf8("Atención"),QString::fromUtf8("Uno de los dos marcadores no se vé."));
		return;
	}
		
	QVec Bola(3);
	Bola=fijador();
	pBolaVerde.set(Bola(0),Bola(1),Bola(2));
	qDebug()<<"fijar en verde";
	pose(pBolaVerde);
	updateBola(pBolaVerde,verde);
}

void Juego2::conseguido(bool b)
{	
	changePosition();
}

Juego2::~Juego2()
{	
	delete pbBinarize;
	delete pbFijaRoja; delete pbFijaVerde; delete pbConseguido;
	delete lcdUmbral;delete lUmbral;
	delete sliderUmbral; 
	delete hLUmbral; 
	delete rbArriba; delete rbReposo; delete rbAbajo;delete rbBucle;
	delete lArm; delete hLArm;
	delete vLayout;

	tracker->removeMarker(marker);
	tracker->removeMarker(marker2);
	
	cam->removeChild(arTransform);
	cam->removeChild(arTransform2); 	
}
