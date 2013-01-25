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

/**
* \brief Default constructor
*/
Worker::Worker(RoboCompCamera::CameraPrx cameraprx, RoboCompKinect::KinectPrx kinectprx)  : Ui_guiDlg()
{
	camera = cameraprx;
	kinect = kinectprx;

	Period = BASIC_PERIOD*1000;
	mutex = new QMutex();
	
	setupUi(this);
	
// 	int imgHeight=480;
// 	int imgWidth=640;
	kinectOrCam=false;//Bool selection image source (Kinect=true or Webcam=false)
	score=0;
	distance2mole1=9999;
	didMyWork1=false;
	didMyWork2=false;
	xMoles=400;
	yMoles=-400;
	zMoles=500;
	scoreZoom=1;
	aux = 0.0;
	
	systemTime = QTime::currentTime();
	qsrand((uint)systemTime.msec());
	
	osgArt = new OsgArt(qframeOsgArt );
	startOSGArt();
	
	show();
	connect (&timer, SIGNAL(timeout())		,this, SLOT(compute()) );
	connect(pbMole1, SIGNAL(pressed())			,this, SLOT(fn_pbMole1()) );
	connect(pbMole2, SIGNAL(pressed())			,this, SLOT(fn_pbMole2()) );
	connect(pbMole3, SIGNAL(pressed())			,this, SLOT(fn_pbMole3()) );
	connect(sliderMoleX,SIGNAL(valueChanged(int))	,this, SLOT(fn_sliderMoleX(int)) );
	connect(sliderMoleY,SIGNAL(valueChanged(int))	,this, SLOT(fn_sliderMoleY(int)) );
	connect(sliderMoleZ,SIGNAL(valueChanged(int))	,this, SLOT(fn_sliderMoleZ(int)) );
	connect(pbHammer1, SIGNAL(pressed())			,this, SLOT(fn_pbHammer1()) );
	connect(pbHammer2, SIGNAL(pressed())			,this, SLOT(fn_pbHammer2()) );
	connect(pbHammer3, SIGNAL(pressed())			,this, SLOT(fn_pbHammer3()) );
	connect(sliderHammerX,SIGNAL(valueChanged(int))	,this, SLOT(fn_sliderHammerX(int)) );
	connect(sliderHammerY,SIGNAL(valueChanged(int))	,this, SLOT(fn_sliderHammerY(int)) );
	connect(sliderHammerZ,SIGNAL(valueChanged(int))	,this, SLOT(fn_sliderHammerZ(int)) );
	connect(spinBoxScoreZoom,SIGNAL(valueChanged(double))	,this, SLOT(fn_spinBoxScoreZoom(double)) );
	connect(pbKinectOrCam, SIGNAL(pressed())		,this, SLOT(fn_pbKinectOrCam()) );
	
	timer.start(Period/1000); //is in ms
	
}
/**
* \brief Thread method
*/
void Worker::compute()
{
	try {	// To get image from kinect or camera.
		if(kinectOrCam){kinect->getDataRGBZinRGB(img,imgDepth,tHeadState,tBaseState);} //Capture RGB image from cameraComp.
		else{camera->getRGBPackedImage(0,img,tHeadState,tBaseState);}//Capture RGB image from kinectComp.
	    }
	catch (exception e) { qDebug()<<"Error talking to kinect or camera."<< e.what() ;}
	
	try {	// To update background of the OSG scene with the RGB image from kinect or camera. It uses Intel IPP.
		osgArt->updateART(img);		
	    }
	catch (exception e) { qDebug()<<"Error updating osgArt with video image from kinect or camera."<< e.what() ;}
	
	try {	// To update the score in the text object in OSG scene.
		scoreString=int2string(score);
		osgArt->changeText(scoreText,scoreString);
	    }
	catch (exception e) { qDebug()<<"Error updating text in OSG scene."<< e.what() ;}
	
// 	randomValue = 
	setRandomMolesStatus(randomInt(0,100), systemTime);
	checkWhichMoledWasHit(100);
	
}
void Worker::startOSGArt()
{
	
	scoreString=int2string(score);//score
	mtMole = osgArt->getMatrixTransform2();
	mtHammer = osgArt->getMatrixTransform1();
	
	//Box(position,rotation,dimensions,color,transparent, arMT);
	scoreBox = osgArt->addBox(osg::Vec3(0,0,-3),osg::Vec3(0,0,0),osg::Vec3(160*scoreZoom,80*scoreZoom,2),osg::Vec4(1,1,1,1),false,mtMole);
	//Text(position,color,CharSize,StringText,Font*, arMT);
	scoreTextFont = osgText::readFontFile("data/fonts/arial.ttf");
	scoreText = osgArt->addText(osg::Vec3(-65,15,3), osg::Vec4(1,0,0,0.6), 30*scoreZoom, "Score:", scoreTextFont,mtMole);
	scoreText = osgArt->addText(osg::Vec3(-65,-30*scoreZoom,3), osg::Vec4(0.8,0.1,0.1,0.8), 30*scoreZoom, scoreString, scoreTextFont,mtMole);
	
	//Ball(position, radius, color, transpatent, arMT)
	mtModelLibrary = osgArt->addBall(osg::Vec3(0,0,0), 100, osg::Vec4(1,1,1,1), true, mtMole);
	osgArt->move(mtModelLibrary, osg::Vec3(1000,1000,1000));
	
	//Capsule(position,rotation,radius,height,color,bool transparent,arMT); (NOW THIS OBJECT ARE TRANSPARENT)
	mole1base = osgArt->addCapsule(osg::Vec3(0,0,0), osg::Vec3(3*PI/2,0,0), 20.0, 5.0, osg::Vec4(1,0,0,0.3),true, mtMole); //Red Cap
	osgArt->move(mole1base, osg::Vec3(xMoles-200+12,yMoles,zMoles));
	osgArt->rotate(mole1base, osg::Vec3(0.2*PI/2,0,0));
	mole2base = osgArt->addCapsule(osg::Vec3(0,0,0), osg::Vec3(3*PI/2,0,0), 20.0, 5.0, osg::Vec4(0,1,0,0.3),true, mtMole); //Green Cap 
	osgArt->move(mole2base, osg::Vec3(xMoles,yMoles,zMoles));
	osgArt->rotate(mole2base, osg::Vec3(0.2*PI/2,0,0));
	mole3base = osgArt->addCapsule(osg::Vec3(0,0,0), osg::Vec3(3*PI/2,0,0), 20.0, 5.0, osg::Vec4(0,0,1,0.3),true, mtMole); //Blue Cap 
	osgArt->move(mole3base, osg::Vec3(xMoles+200-12,yMoles,zMoles));
	osgArt->rotate(mole3base, osg::Vec3(0.2*PI/2,0,0));
	
	//Cone(position,rotation,int coneRadius,int coneHeight,color, bool transparent,arMT);
// 	coneX = osgArt->addCone(osg::Vec3(0,0,0), osg::Vec3(0,PI/2,0), 4.0, 60.0, osg::Vec4(0,1,1,0.5),0, mtMole); //Cyan Cone
// 	coneY = osgArt->addCone(osg::Vec3(0,0,0), osg::Vec3(PI/2,0,0), 4.0, 60.0, osg::Vec4(1,0,1,0.5),0, mtMole); //Magenta Cone
// 	coneZ = osgArt->addCone(osg::Vec3(0,0,0), osg::Vec3(0,0,0), 4.0, 60.0, osg::Vec4(1,1,0,0.5),0, mtMole); //Yellow Cone

	mole1head = new osg::MatrixTransform();
	mole2head = new osg::MatrixTransform();
	mole3head = new osg::MatrixTransform();
	osgArt->move(mole1head, osg::Vec3(100,110,-100));
	osgArt->move(mole2head, osg::Vec3(100,110,-100));
	osgArt->move(mole3head, osg::Vec3(100,110,-100));
	
	mole1base->addChild(mole1head);
	mole2base->addChild(mole2head);
	mole3base->addChild(mole3head);
	
	//Cylinder(position,rotation,float radius, float height,color, bool transparent,arMT);
	hammer1head = osgArt->addCylinder(osg::Vec3(0,40,0), osg::Vec3(0,PI/2,0), 10, 30, osg::Vec4(0.6,0.6,0.8,1),0, mtHammer); //Head
	hammer1base = osgArt->addCylinder(osg::Vec3(0,0,0), osg::Vec3(PI/2,0,0), 4, 70, osg::Vec4(0.5,0.2,0.1,1),0, mtHammer); //Handle
// 	osgArt->resizeCylinderRadius(hammer1head,300);
	
	modelNode1 = osgDB::readNodeFile("data/models/mole/Diglett.3ds");
	if (!modelNode1){std::cout<<"Could not find model =("<<std::endl;}else{std::cout<<"Model loaded! =) "<<std::endl;}
	modelNode2 = osgDB::readNodeFile("data/models/mole/Diglett2.3ds");
	if (!modelNode2){std::cout<<"Could not find model =("<<std::endl;}else{std::cout<<"Model loaded! =) "<<std::endl;}
	modelNode3 = osgDB::readNodeFile("data/models/mole/Diglett3.3ds");
	if (!modelNode3){std::cout<<"Could not find model =("<<std::endl;}else{std::cout<<"Model loaded! =) "<<std::endl;}
	
	mtModelLibrary->addChild(modelNode1);
	mtModelLibrary->addChild(modelNode2);
	mtModelLibrary->addChild(modelNode3);
	
	osgArt->load3DModel(mole1head, modelNode3, osg::Vec3(0,0,0), osg::Vec3(1,1,1));
	osgArt->load3DModel(mole2head, modelNode3, osg::Vec3(0,0,0), osg::Vec3(1,1,1));
	osgArt->load3DModel(mole3head, modelNode3, osg::Vec3(0,0,0), osg::Vec3(1,1,1));
}
void Worker::setRandomMolesStatus(int randomInt, QTime systemTime)
{
  
  
  if(randomInt<10){mole1head->setChild(0,modelNode3);}
  if(randomInt>90){mole1head->setChild(0,modelNode1);}
  
  if(10<randomInt && randomInt<20){mole2head->setChild(0,modelNode3);}
  if(80<randomInt && randomInt<90){mole2head->setChild(0,modelNode1);}
  
  if(20<randomInt && randomInt<30){mole3head->setChild(0,modelNode3);}
  if(70<randomInt && randomInt<80){mole3head->setChild(0,modelNode1);}
}
void Worker::checkWhichMoledWasHit(int threshold)
{
  	try {	// To get interaction between hammer and mole1.
		mtHammer2mole = mt2mtCoordinates( mt2mtCoordinates(mtMole,mtHammer),mole1base );
		matrixTransform2matrixDouble(mtHammer2mole,m1);
		distance2mole1 = sqrt(pow(m1[0][3],2)+pow(m1[1][3],2)+pow(m1[2][3],2));
		
		if(distance2mole1<threshold){mole1head->setChild(0,modelNode2);score++;}
		else{/*mole1head->setChild(0,modelNode2);*/}
	    }
	catch (exception e) { qDebug()<<"Error in the interaction Hammer-Mole1."<< e.what() ;}
	
	try {	// To get interaction between hammer and mole2.
		mtHammer2mole = mt2mtCoordinates( mt2mtCoordinates(mtMole,mtHammer),mole2base );
		matrixTransform2matrixDouble(mtHammer2mole,m2);
		distance2mole2 = sqrt(pow(m2[0][3],2)+pow(m2[1][3],2)+pow(m2[2][3],2));
		
		if(distance2mole2<threshold){mole2head->setChild(0,modelNode2);score++;}
		else{/*mole2head->setChild(0,modelNode2);*/}
	    }
	catch (exception e) { qDebug()<<"Error in the interaction Hammer-Mole2."<< e.what() ;}
	
	try {	// To get interaction between hammer and mole3.
		mtHammer2mole = mt2mtCoordinates( mt2mtCoordinates(mtMole,mtHammer),mole3base );
		matrixTransform2matrixDouble(mtHammer2mole,m3);
		distance2mole3 = sqrt(pow(m3[0][3],2)+pow(m3[1][3],2)+pow(m3[2][3],2));
		
		if(distance2mole3<threshold){mole3head->setChild(0,modelNode2);score++;}
		else{/*mole3head->setChild(0,modelNode2);*/}
	    }
	catch (exception e) { qDebug()<<"Error in the interaction Hammer-Mole2."<< e.what() ;}
}
/**
* \brief Default destructor
*/
Worker::~Worker()
{

}
///Common Behavior
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
	Period = p*1000;
}
/**
* \brief
* @param params_ Parameter list received from monitor thread
*/
bool Worker::setParams(RoboCompCommonBehavior::ParameterList params_)
{
//	active = false;
		//CAMBIAR PARAMETROS Y RE-ARRANQUE DEL COMPONENTE SI ES NECESARIO
//	active = true;
	return true;
}
void Worker::fn_pbKinectOrCam()
{
  kinectOrCam=!kinectOrCam;
}
void Worker::fn_pbHammer1()
{

}
void Worker::fn_pbHammer2()
{

}
void Worker::fn_pbHammer3()
{
//   mole1base->removeChild(mole1head);
//   mole1base->addChild(mole1head);
//   mole1head->replaceChild(modelNode3,modelNode1);
  mole1head->setChild(0,modelNode1);
//   mole3head->setChild(1,modelNode3);
}
void Worker::fn_sliderHammerX(int sliderValue)
{
//   osgArt->resizeBox(scoreBox,osg::Vec3((float)sliderValue,(float)sliderValue,4));
std::cout<<"Slider value: "<<sliderValue<<std::endl;
}
void Worker::fn_sliderHammerY(int )
{
  
}
void Worker::fn_sliderHammerZ(int )
{
  
}
void Worker::fn_pbMole1()
{
//   mole2head->setChild(1,modelNode1);
}
void Worker::fn_pbMole2()
{
//   mole2head->setChild(1,modelNode2);
}
void Worker::fn_pbMole3()
{
// osgArt->setColorRGBA(mole3head,osg::Vec4(1,1,1,1));
//   mole2head->setChild(1,modelNode3);
}
void Worker::fn_sliderMoleX(int sliderValue)
{
  xMoles=sliderValue;
  osgArt->move(mole1base, osg::Vec3(xMoles,0,0));
  osgArt->move(mole2base, osg::Vec3(xMoles,0,0));
  osgArt->move(mole3base, osg::Vec3(xMoles,0,0));
  std::cout<<"Move Moles: "<<xMoles<<" milimetres in X axis."<<std::endl;
}
void Worker::fn_sliderMoleY(int sliderValue)
{
  yMoles=sliderValue;
  osgArt->move(mole1base, osg::Vec3(0,yMoles,0));
  osgArt->move(mole2base, osg::Vec3(0,yMoles,0));
  osgArt->move(mole3base, osg::Vec3(0,yMoles,0));
  std::cout<<"Move Moles: "<<yMoles<<" milimetres in Y axis."<<std::endl;
}
void Worker::fn_sliderMoleZ(int sliderValue)
{
  zMoles=sliderValue;
  osgArt->move(mole1base, osg::Vec3(0,0,zMoles));
  osgArt->move(mole2base, osg::Vec3(0,0,zMoles));
  osgArt->move(mole3base, osg::Vec3(0,0,zMoles));
  std::cout<<"Move Moles: "<<zMoles<<" milimetres in Z axis."<<std::endl;
}
void Worker::fn_spinBoxScoreZoom(double spinBoxValue)
{
  osgArt->changeCharacterSizeText(scoreText,30*spinBoxValue);
  osgArt->scale(scoreBox, osg::Vec3(160*spinBoxValue,80*spinBoxValue,1*spinBoxValue));
}
void Worker::getOpenCVversion()
{
	const char* libraries;	const char* modules;
	cvGetModuleInfo( 0, &libraries, &modules );
	std::cout<<"Libraries: "<<libraries<<std::endl;
	std::cout<<"Modules: "<<modules<<std::endl;
}
void Worker::matrixTransform2matrixDouble(osg::MatrixTransform* mtSrc, double matrixDst[3][4])
{
	for(int row=0;row<4;row++)
	{
	  for(int col=0;col<3;col++)
	  {
		matrixDst[col][row]=mtSrc->getMatrix().operator()(row,col);
	  }
	}
}
void Worker::matrixDouble2matrixTransform(double matrixSrc[3][4], osg::MatrixTransform* mtDst)
{
	osg::Matrix matrixOSG = mtDst->getMatrix();
	matrixOSG.set(matrixSrc[0][0], matrixSrc[1][0], matrixSrc[2][0], 0,
		      matrixSrc[0][1], matrixSrc[1][1], matrixSrc[2][1], 0,
		      matrixSrc[0][2], matrixSrc[1][2], matrixSrc[2][2], 0,
		      matrixSrc[0][3], matrixSrc[1][3], matrixSrc[2][3], 1);
	mtDst->setMatrix(matrixOSG);
}
osg::MatrixTransform* Worker::mt2mtCoordinates(osg::MatrixTransform* mtA, osg::MatrixTransform* mtB)
{
	double matrix1[3][4]; double matrix2[3][4];	// Auxiliar double matrix to calculate distances between OSG objects(matrixTransforms).
	double matrixA[3][4]; double matrixB[3][4]; 	// mtA and mtB in double matrix format.
	osg::MatrixTransform* mtAfromB;
	mtAfromB = new osg::MatrixTransform();
  
	matrixTransform2matrixDouble(mtA,matrixA);
	matrixTransform2matrixDouble(mtB,matrixB);
// 	if (osgArt->getMarker1()->valid() || osgArt->getMarker2()->valid())
// 	{
	  arUtilMatInv(matrixA, matrix1);// Now matrix1 is the inverse of matrixA
	  arUtilMatMul(matrix1, matrixB, matrix2);
	  matrixDouble2matrixTransform(matrix2, mtAfromB);
	  return mtAfromB;
// 	}
// 	else
// 	{
// 	  std::cout<<"The ARToolKit marks are out of camera vision. Please show the 2 marks to the camera."<<std::endl;
// 	  distance2mole1=9999;
// 	}
}

