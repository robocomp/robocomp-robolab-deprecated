
/****************************************************************************
#                                                                           #
#                     Copyright (C) 2010                                    #
#                                                                           #
# This program is free software; you can redistribute it and/or modify      #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation; either version 2 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with this program; if not, write to the Free Software               #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA #
#                                                                           #
****************************************************************************/
#include "worker.h"

Worker::Worker(RoboCompCamera::CameraPrx cameraprx, RoboCompDifferentialRobot::DifferentialRobotPrx differentialrobotprx,RoboCompJointMotor::JointMotorPrx jointmotorprx, QObject *parent) : QObject(parent)
{
	camera = cameraprx;
	differentialrobot = differentialrobotprx;
	jointmotor = jointmotorprx;
	//mutex = new QMutex();
	//setupUi(this);
	//show();
	threshold = 200;
	tipo =true;
	try
  	{
		RoboCompJointMotor::MotorGoalPositionList list;
		RoboCompJointMotor::MotorGoalPosition g;
		g.name = "leftPan";
		g.maxSpeed = 0;
		g.position = 0.25;
		list.push_back(g);
		g.name = "tilt";
		g.position = -0.7;
		list.push_back(g);
		g.name = "rightPan";
		g.position = 0.f;
		list.push_back(g);
		jointmotor->setSyncPosition(list);

	}
	catch(RoboCompJointMotor::OutOfRangeException &ex)
	{
		throw ex;
	}
	RoboCompJointMotor::MotorState aux;
	aux = jointmotor->getMotorState("tilt");
	tilt = aux.pos;
	aux = jointmotor->getMotorState("leftPan");
	leftpan = aux.pos;

	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));
	timer.start(BASIC_PERIOD);
	//Initialize innermodel
	innerModel = new InnerModel("/home/coche/robocomp/Classes/innermodel/innermodel/robex.xml");
	try{
		innerModel->setUpdateTranslationPointers("base", &(bState.x), NULL, &(bState.z));
		innerModel->setUpdateRotationPointers("base", NULL, &(bState.alfa), NULL);
		innerModel->setUpdateRotationPointers(QString::fromStdString("tilt"), &tilt, NULL, NULL);
		innerModel->setUpdateRotationPointers(QString::fromStdString("leftPan"), NULL, &leftpan, NULL);
	}
	catch(const Ice::Exception& ex){
		std::cout << "Innermodel fail "<<ex;
	}
}

Worker::~Worker()
{

}

void Worker::compute( )
{
	try
	{
		camera->getYImage(0, imgReal, hState, bState);
	}
	catch(const Ice::Exception& ex){
		std::cout<< "Camera proxy failure "<<ex;
	}

	innerModel->update();

	printf("x:%f z:%f a:%f\n", bState.x, bState.z, bState.alfa);
	printf("tilt%f  leftpan:%f\n", tilt, leftpan);
}

void Worker::imagenReal(RoboCompCamera::imgType &real)
{
  real=imgReal;
}

void Worker::imagenBinaria(RoboCompCamera::imgType &binaria)
{
  
  QList <RoboCompCxtrelan::punto> listBlancos, tempListBlancos;
  QList <QList <RoboCompCxtrelan::punto> > lista;
  
  listBlancos.clear(); tempListBlancos.clear(); lista.clear();
  
  binaria.resize(320*240);  
  
 for (int i=0; i < 320*240; i++) {
	if(tipo){
		if ( imgReal[i] > threshold) 
			binaria[i]=255;
		else
			binaria[i]=0;
	}
	else{
		if ( imgReal[i] < threshold ) 
			binaria[i]=255;
		else
			binaria[i]=0;
	}
  }
  //qDebug()<<"*****";
  for (uint j=120; j<240; j=j+50) {
	listBlancos.clear(); 
	RoboCompCxtrelan::punto p;
	for (uint i=0; i< 320; i++) {	  
	  if (binaria[(320*j)+i] == 255 )
		{
//  		  qDebug()<<i<<" "<<j<<" "<<binaria[(320*j)+1];
		  p.x=i;
		  p.z=j;
		  tempListBlancos.append(p);		  
		}
	  else
	  {
		if (tempListBlancos.size() > listBlancos.size() ) 
		  {
			 listBlancos=tempListBlancos;			 
		  }
		tempListBlancos.clear();  
	  }
		
	}
	if (tempListBlancos.size() > listBlancos.size() ) 
		  {
			 listBlancos=tempListBlancos;			 
	 		tempListBlancos.clear();  

		  }

	if ( listBlancos.isEmpty() )
	  {RoboCompCxtrelan::punto p; p.x=-1; p.z=-1; listBlancos.append(p); }
	lista.append(listBlancos);	
  }
    //qDebug()<<"***** lista size() ****"<<lista.size();
	
  ///generamos el vector de puntos medios a devolver	
  vectorP.resize(lista.size() );
  vectorPI.resize(lista.size() );
   QVec v(3),r(3),rr(2);
   v(2) = 1.f;
   RoboCompCxtrelan::punto p, pi;
   QMat m = innerModel->getAffineHomographyMatrix("virtualCamera","floor","leftCamera");
   m.print("H");
//    vector
   for (int i=0; i<lista.size();i++){
	/*qDebug()<<"***** tam segmento puntos blancos ****"<<lista.at(i).size();
	qDebug()<<"cordena inicio segmento "<<i<<" : " <<lista.at(i).first().x<<" "<<lista.at(i).first().z;
	qDebug()<<"cordena fin segmento "<<i<<" : " <<lista.at(i).last().x<<" "<<lista.at(i).last().z;
  	qDebug()<<"punto Medio: "<< (lista.at(i).first().x + lista.at(i).last().x ) / 2 <<" "<<lista.at(i).last().z;*/

	pi.x = (lista.at(i).first().x+lista.at(i).last().x)/2;
	pi.z = lista.at(i).last().z;
	vectorPI[i] = pi;

	if ( (lista.at(i).first().x == -1) || ( lista.at(i).last().z == -1 )) {	
	  p.x=0; p.z=0; 
	}
	else {
		v(0) = (lista.at(i).first().x + lista.at(i).last().x ) / 2;
		v(1) = lista.at(i).last().z;
		v(2) = 1;
		v.print("v");
		r = m * v;
		r.print("ret");
		if (r(2)<0.001) 
		{
			p.x=p.z=0;
		}
		else
		{
			rr = r.fromHomogeneousCoordinates();
			p.x =  rr(0);
			p.z = rr(1);
		}
	}
	vectorP[i]=p;
	
	
  }
  
}

void Worker::mover(float avance, float giro)
{
  differentialrobot->setSpeedBase(avance,giro);
}

void Worker::vectorPuntos(RoboCompCxtrelan::ListaPuntos &lp)
{
  lp=vectorP;
}
void Worker::vectorPuntosImagen(RoboCompCxtrelan::ListaPuntos &lp)
{
  lp=vectorPI;
}
void Worker::setThreshold(int _threshold,bool _tipo)
{
	threshold = _threshold;
	tipo = _tipo;
}
