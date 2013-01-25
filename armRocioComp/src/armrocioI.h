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
#ifndef ARMROCIOI_H
#define ARMROCIOI_H

// QT includes
#include <QtCore/QObject>

// Ice includes
#include <Ice/Ice.h>
#include <Armrocio.h>

#include <config.h>
#include "worker.h"



using namespace RoboCompArmrocio;

class ArmrocioI : public QObject , public virtual RoboCompArmrocio::Armrocio
{
Q_OBJECT
public:
	ArmrocioI( Worker *_worker, QObject *parent = 0 );
	~ArmrocioI();
	
	void setPose(Pose pose,const std::string &arm, const Ice::Current& = ::Ice::Current());
	bool isMoving(const Ice::Current& = ::Ice::Current()); 
	void getNextPose(const std::string &arm,Pose &p,const Ice::Current& = ::Ice::Current());
	void getJointState(RoboCompJointMotor::MotorStateMap& jstateMap, const Ice::Current& = ::Ice::Current());
	void setMaxSpeed(float speed,const Ice::Current& = ::Ice::Current());
	RoboCompJointMotor::MotorParamsList getJointParams(const Ice::Current& = ::Ice::Current());
	
	
// 	QMutex *mutex;
private:
	Pose left,right;
	Worker *worker;
public slots:


};

#endif
