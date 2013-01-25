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

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)	
{
	alto=480;ancho=640;
     innerModel = new InnerModel("/home/robocomp/robocomp/Files/InnerModel/humanColor.xml");
//	innerModel = new InnerModel("/home/ramon/robocomp/Files/InnerModel/juegoJoint.xml");
// ----
	QGLFormat fmt;
	fmt.setDoubleBuffer(true);	
	QGLFormat::setDefaultFormat(fmt);
	
	osg = new OsgView (frameOSG,true);		
	imv = new InnerModelViewer(innerModel, "world", osg->getRootGroup());
	innerModel->updateTransformValues("XN_SKEL_TORSO", 0, 0, 2000, -2.0, 0.0, 0.0);
	imv->update();
	
	toPlay=0;
	toLoop=0;
	toShowGraph=0;
	numberOfSessionJoints=0;
	indexBlock=0;
	fileLoaded=0;
	lineTextNumber=0;
	timeBarValue=0;
	sessionInitialTime.setHMS(0,0,0,0);
	sessionMSecs=0;
	sessionPeriod=33.3;
	mutexRTMatList = new QMutex();
	RTMatList.clear();
	setLoop();
	timeBar->reset();
	
	connect( getFile_pb, 	SIGNAL(pressed()) 		,this, SLOT(getFileName()));
	connect( setPlay_pb, 	SIGNAL(pressed()) 		,this, SLOT(play()));
	connect( setPause_pb, 	SIGNAL(pressed()) 		,this, SLOT(pause()));
	connect( reset_pb, 		SIGNAL(pressed()) 		,this, SLOT(stop()));
	connect( setLoop_pb, 	SIGNAL(pressed()) 		,this, SLOT(setLoop()));
	connect( increaseSpeed_pb, 	SIGNAL(pressed()) 	,this, SLOT(setSpeedMore()));
	connect( decreaseSpeed_pb, 	SIGNAL(pressed()) 	,this, SLOT(setSpeedLess()));
	connect( resetSpeed_pb, 	SIGNAL(pressed()) 	,this, SLOT(resetSpeed()));
	connect( showGraph_pb, 		SIGNAL(pressed()) 	,this, SLOT(showGraph()));
	
	this->Period = sessionPeriod;
	timer.start(Period);
}

void SpecificWorker::compute( )
{
  if (toPlay && fileLoaded)
  {
	if (lineTextNumber < 10)
	{
	  cout<<"Voy a por la info de cabecera"<<endl;
	  getHeaderInfo(); 
	}
    for (int i=0; i<numberOfSessionJoints; i++)
    {
	  sessionTextLine=sessionFile.readLine();
	  lineTextNumber++;
	  QStringList sessionTextLineStrings=sessionTextLine.split(" ");
	  if (sessionTextLineStrings.size()==4)
	  {
		QString joint_name = sessionTextLineStrings.at(0);
		float rx = sessionTextLineStrings.at(1).toFloat();
		float ry = sessionTextLineStrings.at(2).toFloat();
		float rz = sessionTextLineStrings.at(3).toFloat();
		innerModel->updateRotationValues(joint_name, rx, ry, rz);
		updateLCDangles(joint_name, rx, ry, rz);
		updateRTMatrix(sessionTextLineStrings);
	  }
	  timeBarValue=timeBarValue+sessionTextLine.size();
	  timeBar->setValue(timeBarValue);
    }
	if (toShowGraph)
	{
	  sessionGraph->drawMarkers((float)(lineTextNumber*sessionPeriod/12));
	}
    sessionMSecs = sessionMSecs + (sessionPeriod/Period)*sessionPeriod;
	sessionUpdatedTime = sessionInitialTime.addMSecs(sessionMSecs);
// 	cout<<"Time:	"<<sessionUpdatedTime.toString("hh:mm:ss").toStdString()<<endl;
	timeEdit->setTime(sessionUpdatedTime);

    if (sessionFile.atEnd())
    {
      if (toLoop==1)
      {
		stop();
		play();
      }
      else
	  {
		stop();
	  }
    }
  }
  else
  {
// 	cout<<"Para comenzar a reproducir la sesión, primero cargue el archivo de sesión y después presiones el botón Play."<<endl;
  }
  imv->update();
}
QString SpecificWorker::getFileName()
{
  sessionFilePath=openFileDlg.openFile();
  QStringList sessionPathName=sessionFilePath.split("/");
  cout<<"File Name: "<<sessionPathName.last().toStdString()<<endl;
  nameOfFile_label->setText(sessionPathName.last());
  stop();
  return sessionFilePath;
}
void SpecificWorker::loadSessionFile(QString pathSessionFile)
{
	sessionFile.setFileName(pathSessionFile);
	sessionFile.open(QIODevice::ReadOnly | QIODevice::Text);
	fileLoaded=1;
	timeBar->setMaximum(sessionFile.size());
}
void SpecificWorker::updateRTMatrix(QStringList sessionTextLineStrings)
{
  mutexRTMatList->lock();
  RTMatList[sessionTextLineStrings.at(0).toStdString()].clear();
  RTMatList[sessionTextLineStrings.at(0).toStdString()].push_back(0.0);
  RTMatList[sessionTextLineStrings.at(0).toStdString()].push_back(0.0);
  RTMatList[sessionTextLineStrings.at(0).toStdString()].push_back(0.0);
  RTMatList[sessionTextLineStrings.at(0).toStdString()].push_back(sessionTextLineStrings.at(1).toFloat());
  RTMatList[sessionTextLineStrings.at(0).toStdString()].push_back(sessionTextLineStrings.at(2).toFloat());
  RTMatList[sessionTextLineStrings.at(0).toStdString()].push_back(sessionTextLineStrings.at(3).toFloat());
  mutexRTMatList->unlock();
}
void SpecificWorker::play()
{
  toPlay=1;
}
void SpecificWorker::pause()
{
  toPlay=0;
}
void SpecificWorker::stop()
{
  toPlay=0;
  loadSessionFile(sessionFilePath);
  timeBarValue=0;
  lineTextNumber=0;
  numberOfSessionJoints=0;
  timeBar->setValue(timeBarValue);
  innerModel->updateTransformValues("XN_SKEL_TORSO", 0, 0, 2000, -2.0, 0.0, 0.0);
  imv->update();
  sessionMSecs = 0;
  sessionUpdatedTime = sessionInitialTime.addMSecs(sessionMSecs);
  cout<<"Time:	"<<sessionUpdatedTime.toString("hh:mm:ss").toStdString()<<endl;
  timeEdit->setTime(sessionUpdatedTime);
}
void SpecificWorker::setLoop()
{
  toLoop=!toLoop;
  if (toLoop){setLoop_pb->setChecked(0);}
  else {setLoop_pb->setChecked(1);}
}
void SpecificWorker::showGraph()
{
  sessionGraph = new graphics(sessionFilePath);
  toShowGraph=1;
}
void SpecificWorker::updateLCDangles(QString jointName, float rotationXrad, float rotationYrad, float rotationZrad)
{
  rotationXrad = floor(360*rotationXrad/(2*3.1416));
  rotationYrad = floor(360*rotationYrad/(2*3.1416));
  rotationZrad = floor(360*rotationZrad/(2*3.1416));
  
  if(!jointName.compare(QString("XN_SKEL_WAIST")))
  {
// 	waist_x_lcd->display(rotationXrad);
// 	waist_y_lcd->display(rotationYrad);
// 	waist_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_TORSO")))
  {
// 	torso_x_lcd->display(rotationXrad);
// 	torso_y_lcd->display(rotationYrad);
// 	torso_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_NECK")))
  {
// 	neck_x_lcd->display(rotationXrad);
// 	neck_y_lcd->display(rotationYrad);
// 	neck_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_RIGHT_SHOULDER")))
  {
	shoulder_r_x_lcd->display(rotationXrad);
	shoulder_r_y_lcd->display(rotationYrad);
	shoulder_r_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_LEFT_SHOULDER")))
  {
	shoulder_l_x_lcd->display(rotationXrad);
	shoulder_l_y_lcd->display(rotationYrad);
	shoulder_l_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_RIGHT_KNEE")))
  {
// 	knee_r_x_lcd->display(rotationXrad);
// 	knee_r_y_lcd->display(rotationYrad);
// 	knee_r_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_LEFT_KNEE")))
  {
// 	knee_l_x_lcd->display(rotationXrad);
// 	knee_l_y_lcd->display(rotationYrad);
// 	knee_l_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_RIGHT_HIP")))
  {
// 	hip_r_x_lcd->display(rotationXrad);
// 	hip_r_y_lcd->display(rotationYrad);
// 	hip_r_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_LEFT_HIP")))
  {
// 	hip_l_x_lcd->display(rotationXrad);
// 	hip_l_y_lcd->display(rotationYrad);
// 	hip_l_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_RIGHT_ELBOW")))
  {
	elbow_r_x_lcd->display(rotationXrad);
	elbow_r_y_lcd->display(rotationYrad);
	elbow_r_z_lcd->display(rotationZrad);
  }
  if(!jointName.compare(QString("XN_SKEL_LEFT_ELBOW")))
  {
	elbow_l_x_lcd->display(rotationXrad);
	elbow_l_y_lcd->display(rotationYrad);
	elbow_l_z_lcd->display(rotationZrad);
  }
}
void SpecificWorker::resetSpeed()
{
  this->Period = sessionPeriod;
  cout<<"Period: "<<Period<<endl;
  timer.start(Period);
}
void SpecificWorker::setSpeedMore()
{
  if (this->Period > 1)  {this->Period =this->Period*0.5; }
  else  
  {
	QMessageBox msgBox;
	msgBox.setText("Estas loco!! El Innermodel se estresa si va mas rapido.");
	msgBox.exec();
  }
  cout<<"Period: "<<Period<<endl;
  timer.start(Period);
}
void SpecificWorker::setSpeedLess()
{
  this->Period =this->Period*2;
  cout<<"Period: "<<Period<<endl;
  timer.start(Period);
}
void SpecificWorker::getHeaderInfo()
{
  for (int i=0; i<10; i++)
  {
	sessionTextLine=sessionFile.readLine();
	lineTextNumber++;
	QStringList headerTextLineStrings=sessionTextLine.split(" ");
	switch(lineTextNumber)
	{
	  case 1:
		if (headerTextLineStrings.at(1).toInt() != 1 
		  || headerTextLineStrings.at(2).toInt() != 0 
		  || headerTextLineStrings.at(3).toInt() != 0)
		{
		  i=9;
		  cout<<"Version: "<<headerTextLineStrings[1].toStdString();
		  cout<<"."<<headerTextLineStrings.at(2).toStdString();
		  cout<<"."<<headerTextLineStrings.at(3).toStdString()<<endl;
		  QMessageBox msgBox;
		  msgBox.setText("The document doesn't have a correct header.");
		  msgBox.exec();
		  break;
		}
		else
		{
		  qDebug()<<"Version: "<<headerTextLineStrings[1]<<"."<<headerTextLineStrings[2]<<"."<<headerTextLineStrings[3];
		  break;
		}
	  case 2:
		nameOfCenter_label->setText(headerTextLineStrings.at(1));
		cout<<"Center: "<<headerTextLineStrings.at(1).toStdString()<<endl;
		break;
	  case 3:
		doctorName_label->setText(headerTextLineStrings.at(1));
		cout<<"Doctor: "<<headerTextLineStrings.at(1).toStdString()<<endl;
		break;
	  case 4:
		break;
	  case 5:
		nameOfPatient_label->setText(headerTextLineStrings.at(1));
		cout<<"Patient: "<<headerTextLineStrings.at(1).toStdString()<<endl;
		break;
	  case 6:
		yearOfSession_label->setText(headerTextLineStrings.at(1));
		monthOfSession_label->setText(headerTextLineStrings.at(2));
		dayOfSession_label->setText(headerTextLineStrings.at(3));
		break;
	  case 7:
		initialTime_label->setText(
		  headerTextLineStrings.at(1)+QString(":")
		+ headerTextLineStrings.at(2)+QString(":")
		+ headerTextLineStrings.at(3));
		sessionInitialTime.setHMS(headerTextLineStrings.at(1).toInt(),headerTextLineStrings.at(2).toInt(),headerTextLineStrings.at(3).toInt(),0);
		break;
	  case 8:
		break;
	  case 9:
		break;
	  default:
		break;
	}
  }
  qDebug()<<"Numero de linea"<<lineTextNumber;
  numberOfSessionJoints=1;
  for (int i=0; i<12; i++)
  {
	sessionTextLine=sessionFile.readLine();
	lineTextNumber++;
	if(sessionTextLine.compare(QString("\n")))
	{
	  numberOfSessionJoints++;
	}
	else break;
	
  }
  qDebug()<<"Numero de Articulaciones: "<<numberOfSessionJoints;
}
void SpecificWorker::getRTMatrixList(RTMatrixList& RTMatL)
{
  mutexRTMatList->lock();
  RTMatL=RTMatList;
  mutexRTMatList->unlock();
}
void SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
// 	timer.start(Period);
};

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}
void SpecificWorker::closeEvent(QCloseEvent *event)
 {
	 killYourSelf();
 }
