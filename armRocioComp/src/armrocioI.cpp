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
#include "armrocioI.h"

const float pose0[] =  {0.1,	0.1,	0.f,	0.f,	0.f };	//reposo

const float pose11[] = {0.1,	0.1,	0.f,	0.f,	0.f };	//juego1_abajo
const float pose12[] = {M_PI-0.2,	0.1,	0.f,	0.f,	0.f };	//juego 1_arriba

const float pose21[] = {0.1,	0.1,	0.f,	0.f,	0.f };	//juego 2_abajo
const float pose22[] = {0.1,	0.1,	0.f,	2.4,	0.f };	//juego 2_arriba


const float hola_1[] = {0.f,/*0.15*/0.f,0.50};
const float hola_2[] = {0.f,/*0.15*/0.f,-0.30};

const float luiky[] = {M_PI/2,	M_PI/2-0.25,	0.f,	M_PI/2, 0.f }; //openni

ArmrocioI::ArmrocioI(Worker *_worker, QObject *parent) : QObject(parent)
{
	worker = _worker;
	//mutex = worker->mutex;       // Shared worker mutex
	// Component initialization...
	left = right = unknown;
}


ArmrocioI::~ArmrocioI()
{
	// Free component resources here
}


void ArmrocioI::setPose(Pose pose,const std::string &arm, const Ice::Current&)
{
  const float *pactual;
  switch(pose)
  {
 	case game1Up:	pactual = &pose12[0];
 					break;
 	case game1Down:	pactual = &pose11[0];
 					break;
	case game2Up:	pactual = &pose22[0];
  					break;
	case game2Down:	pactual = &pose21[0];
  					break;
	case reposo:	pactual = &pose0[0];
					break;
	case hola1:
					break;
	case hola2:
					break;
	case openni:	pactual = &luiky[0];
					break;
					
	default:  RoboCompArmrocio::PoseNotImplemented ex;
			  ex.what = std::string("Exception: ArmRocioComp::game::") ;
			  throw ex;
			  break;
	}
	
	if (arm == "both")
	{
		left =  pose;
		right = pose;
	}
	else
	{	
		if (arm == "left" )
			left = pose;
		else 
			right = pose;
	}
	worker->setPosition(pactual,arm);
}

bool ArmrocioI::isMoving(const Ice::Current&)
{
	return worker->move;
}

void ArmrocioI::getNextPose(const std::string &arm,Pose &p,const Ice::Current&)
{
	if(arm == "left")
		p = left;
	else
		p = right;
}
void ArmrocioI::getJointState(RoboCompJointMotor::MotorStateMap &jstateMap,const Ice::Current& )
{
	jstateMap = worker->motors;
}
void ArmrocioI::setMaxSpeed(float speed, const Ice::Current& )
{
	worker->setMaxSpeed(speed);
}
RoboCompJointMotor::MotorParamsList ArmrocioI::getJointParams(const Ice::Current& )
{
	return worker->params;
}


