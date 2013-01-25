/*
 *    Copyright (C) 2006-2010 by RoboLab - University of Extremadurais file is part of RoboComp
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

Worker::Worker(RoboCompJointMotor::JointMotorPrx jointmotorprx, QObject *parent) : QObject(parent)
{
	jointmotor = jointmotorprx;


	//mutex = new QMutex();
	//setupUi(this);
	//show();
// 	try
// 	{
// 		params = jointmotor->getAllMotorParams();
// 		if(params.size() < 4)
// 		  qFatal("Error num motor: required 4 obtained: "+params.size());
// 	}
// 	catch(Ice::Exception e)
// 	{
// 	  qFatal("Error talking to jointMotor::getting params");
// 	}
// 	
// 	r_vmax.resize(4);
// 	//Creamos el vector de rampas
// 	for(int i=0;i<4;i++)
// 		rampa.append(1);
// 	//Calcula r_vmax para cada motor
// 	for(int i=0;i<4;i++)
// 	{
// 		params[i].maxVelocity = 2.5;
// 		qDebug()<<"velocidad maxima"<<params[i].maxVelocity;
// 		r_vmax[i] = pow(params[i].maxVelocity,2)/(rampa.at(i)*2);
// 	}
// 	move = false;
// 	vLimites.resize(4);
// 	pDistances.resize(4);
// 	tiempo = 0.f;
// 	tcambio1.resize(4);
// 	tcambio2.resize(4);
// 	goals.resize(4);
// 	poseEstimada.resize(4);
// 	poseInicial.resize(4);
// 
// 	for(uint i=0;i<params.size();i++)
// 	{
// 		RoboCompJointMotor::MotorGoalPosition goal;
// //		goal.position = -2.f;
// 		goal.name = params[i].name;
// // 		goal.maxSpeed = 6.f;	
// // 		
//  		goals[i] = goal;
//  	}
// 	
// 	qDebug()<<"constructor";
 	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));
	timer.start(BASIC_PERIOD);
	
	/**NUEVO*/
	try
	{
		params = jointmotor->getAllMotorParams();
		if(params.size() < 10)
		  qFatal("Error num motor: required 10 obtained: "+params.size());
	}
	catch(Ice::Exception e)
	{
	  qFatal("Error talking to jointMotor::getting params");
	}
	right.resize(5);
	left.resize(5);
	both.resize(10);
	
	RoboCompJointMotor::MotorGoalPosition aux;
	aux.maxSpeed = 0.75;
	for(int i=0;i<5;i++)
	{
		right[i] = aux;
		left[i] = aux;
		both[i] = aux;
		both[i+5] = aux;
	}
	both[0].name = right[0].name = "r1";
	both[1].name = right[1].name = "r2";
	both[2].name = right[2].name = "r3";
	both[3].name = right[3].name = "r4";
	both[4].name = right[4].name = "r5";
	both[5].name = left[0].name = "l1";
	both[6].name = left[1].name = "l2";
	both[7].name = left[2].name = "l3";
	both[8].name = left[3].name = "l4";
	both[9].name = left[4].name = "l5";
}

Worker::~Worker()
{

}
void Worker::setMaxSpeed(float speed)
{
	qDebug()<<"set max speed"<<speed;
	for(uint i=0;i<params.size();i++)
	   params[i].maxVelocity = speed;
}

void Worker::compute( )
{
	move = false;
	try
	{
		jointmotor->getAllMotorState(motors);
 		for(int i=0;i<motors.size();i++)
 		{
			if(motors[params[i].name].isMoving == true)
			{
				move = true;
				qDebug()<<"moving"<<params[i].name.c_str();
			}
// 			if(motors[i].temperature >= 65)
// 				qFatal("Overheatin motor"+QString::number(i));
// 			  
 		}
	}
	catch(Ice::Exception e)
	{
	  qDebug()<<"Exception"<<e.what();
	}
/*	if(move)
	{
		 float time = tmove.elapsed()/1000.f;
		 qDebug()<<"--------------------------------";
		 qDebug()<<"time elapsed"<<time;
		 if(time < tiempo +1)
		 {
		   //introducir factor en funcion de error en la posicion
			  float pos,speed;
			  for(uint i=0;i<params.size();i++)
			  {
					if(time <= tcambio1.at(i))
					{
						speed = rampa.at(i) * time;
						pos = speed * time/2;
						poseEstimada[i] = pos;
					}
					else
					{
					  if(time > tcambio2.at(i))
					  {
						   speed = (rampa.at(i) * (tiempo - time));
						   pos = pDistances.at(i) - (speed * (tiempo -time)/2);
						   poseEstimada[i] = pos;
					  }
					  else
					  {
						  speed = vLimites.at(i);
						  pos = poseEstimada[i] + (speed * 0.1 ); 
						  poseEstimada[i] = pos;
					  }
					}
					if(fabs(motors[params[i].name].pos - (poseEstimada[i]-poseInicial[i])) > 0.1)
					{
					if( motors[params[i].name].pos >  (poseEstimada[i]-poseInicial[i]) )
 							speed *= 0.9;
 						else  
 							speed *=1.1;
					}
					if(speed < 0.6)
						speed = 0.6;
					
					goals[i].maxSpeed = speed;
					
					qDebug()<<"motor "<<i<<"speed"<<goals[i].maxSpeed<<(time <= tcambio1.at(i))<<(time > tcambio2.at(i));
					qDebug()<<"posiciones"<<"real"<<motors[params[i].name].pos<<"estimada"<<poseEstimada[i]<<"distancia"<<fabs(motors[params[i].name].pos - (poseEstimada[i]));
			  }
			  try{
				 jointmotor->setSyncPosition(goals);
			  }catch(Ice::Exception e)
			  {
				qDebug()<<"Exception"<<e.what();
			  }
		 }
		 else
		 {
			move = false;
			RoboCompJointMotor::MotorGoalVelocity vel;
			vel.velocity = 0.f;
			for(uint i=0;i<params.size();i++)
			{
					vel.name = params[i].name;				
					jointmotor->setVelocity(vel);
			}
		 }
	}*/
}

void Worker::setPosition(const float vec[],std::string arm)
{
// 	for(uint i=0;i<params.size();i++)
// 	{
// 	  goals[i].position = vec[i];
// 	  poseInicial[i] = motors[params[i].name].pos;
// 	}
// 	computeTime();
// 	move = true;
// 	tmove.start();
qDebug()<<"move "<<arm.c_str();
	bool send = false;
	if(arm == "left" or arm == "both")
	{
		send = true;
		for(int i=0;i<5;i++)
		{
			left[i].position = vec[i];
			both[i+5].position = vec[i];
		}
	}
	if(arm == "right" or arm == "both")
	{
		send = true;
		for(int i=0;i<5;i++)
		{
			right[i].position = vec[i];
			both[i].position = vec[i];
		}
	}
	else
	{
		qDebug()<<"Error setting incorrect arm"<<arm.c_str();
	}
	if(send)
	{
		try{
			if(arm == "left")
				jointmotor->setSyncPosition(left);
			else if(arm == "right")
				 jointmotor->setSyncPosition(right);
			else if(arm == "both")
				 jointmotor->setSyncPosition(both);
				
			
		}
		catch(Ice::Exception e)
		{
			qDebug()<<"Exception"<<e.what();
		}
	}
	
}

void Worker::computeTime()
{
	float aux=0.f;
	try
	{
		  jointmotor->getAllMotorState(motors);
	}
	catch(Ice::Exception e)
	{
	  qDebug()<<"Exception"<<e.what();
	}

	//calcula el tiempo de cada motor para el movimiento completo y se queda el mayor.
	for(uint i=0;i<params.size();i++)
	{
		pDistances[i] = fabs(goals[i].position - motors[params[i].name].pos);
		qDebug()<<"distancias"<<goals[i].position<< motors[params[i].name].pos;
		r_vmax[i] = pow(params[i].maxVelocity,2)/(rampa.at(i)*2);
		if(r_vmax.at(i) >= pDistances.at(i)/2)
		{
			vLimites[i] = sqrt(pDistances.at(i) * rampa.at(i));
			aux = 2 * vLimites.at(i)/rampa.at(i);
		}
		else
		{
			float t1 = params[i].maxVelocity / rampa.at(i);
			float r1 = params[i].maxVelocity * t1 / 2;
			float r2 = pDistances.at(i) - 2 *r1;
			float t2 = r2 / params[i].maxVelocity;
			aux = 2*t1+t2;
			vLimites[i] = params[i].maxVelocity;
		}
		if(aux > tiempo)
		  tiempo = aux;
	}

	for(uint i=0;i<params.size();i++)
	{
		aux = pow(tiempo,2) - (4*pDistances.at(i)/rampa.at(i));
		if(aux > -0.001 and aux < 0.f)
			aux = 0.f;
		if(aux < 0.f)
			qDebug()<<"error cálculo de tiempos";
		else
		{
			tcambio1[i] = (tiempo - sqrt(aux))/2;
			tcambio2[i] = (tiempo + sqrt(aux))/2;
			vLimites[i] = tcambio1.at(i) * rampa.at(i);
		}
	}
// 	qDebug()<<"distancias"<<pDistances;
// 	qDebug()<<"tiempo maximo"<<tiempo;
// 	qDebug()<<"tiempo cambio"<<tcambio1;
// 	qDebug()<<"tiempo cambio"<<tcambio2;
// 	qDebug()<<"v limites despues"<<vLimites;
}

bool Worker::checkPosition(const float vec[])
{
	float distancia=0.f;
	distancia += fabs(motors[params[0].name].pos - vec[0]);
	distancia += fabs(motors[params[1].name].pos - vec[1]);
	distancia += fabs(motors[params[2].name].pos - vec[2]);
	distancia += fabs(motors[params[3].name].pos - vec[3]);
	if(distancia < 0.7)
	  return true;
	else 
	  return false;
}


void Worker::sendJoint(RoboCompJointMotor::MotorGoalPositionList list)
{
	try
	{
		  jointmotor->setSyncPosition(list);
	}
	catch(Ice::Exception e)
	{
	  qDebug()<<"Exception"<<e.what();
	}
}
