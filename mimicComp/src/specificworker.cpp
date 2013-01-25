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

SpecificWorker::SpecificWorker(MapPrx& mprx, QThread *parent) : GenericWorker(mprx, parent)	
{
	Period = 3000;
	lastQuery = QTime(0,0,0,0);
	JointPeriod = 100;
	//RoboCompJoint
	positions["r1"] = 0.f;	error["r1"] = 0.f;
	positions["r2"] = 0.f;	error["r2"] = 0.f;
	positions["r4"] = 0.f;	error["r3"] = 0.f;
/*	positions["l1"] = 0.f;	error["l1"] = 0.f;
	positions["l2"] = 0.f;	error["l2"] = 0.f;
	positions["l4"] = 0.f;	error["l3"] = 0.f;*/

	//Read motors params
	try
	{
		params = jointmotor->getAllMotorParams();
	}
	catch(const Ice::Exception& e)
	{
		qDebug()<<"Error reading motors params from JointMotor"<<e.what();
	}

}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}
#define MAX_VEL 3
#define MAX_ACC 0.10
#define MAX_ERROR 0.4

void SpecificWorker::run()
{
	qDebug()<<"First run";
static float caca = 0;

	forever
	{
		caca+=0.04;
		//Read model positions
		try
		{
		//	evaluadoroff->getRTMatrixList(mList);
//			trackeropenni->getRTMatrixList(mList);

		}
		catch(const Ice::Exception& e)
		{
			qDebug()<<"Error reading matrix from EvaluadorOff"<<e.what();
		}
		
		
		qDebug()<<"size"<<mList.size();
		
		
		//Read motors positions
		try
		{
			jointmotor->getAllMotorState(motors);
		}
		catch(const Ice::Exception& e)
		{
			qDebug()<<"Error reading motors from JointMotor"<<e.what();
		}

	//	if(mList.size() > 0 ) {

		static float vel1=0, pos1=motors["l4"].pos;
		float posd1 = (1.+sin(caca));		//mList["XN_SKEL_LEFT_SHOULDER"].at(3)+M_PI;
		float acc_d_1 = posd1 - motors["l4"].pos - vel1;

		float max_acc = fabs(posd1 - motors["l4"].pos) / MAX_ERROR;
		if( max_acc > 1.f)	max_acc = MAX_ACC;
		else if(max_acc < -1.f)	max_acc = -MAX_ACC;
			 else max_acc = max_acc * MAX_ACC; 
		
		if (acc_d_1 > MAX_ACC) acc_d_1 = MAX_ACC;
		else if (acc_d_1 < -MAX_ACC) acc_d_1 = -MAX_ACC;

		vel1 = vel1 + ((acc_d_1 * 1000.) / Period);
		if (vel1 > MAX_VEL) vel1 = MAX_VEL;
		else if (vel1 < -MAX_VEL) vel1 = -MAX_VEL;

		pos1 = pos1 + vel1 * 1000. / Period;

		if (pos1 > posd1 and posd1 > motors["l4"].pos)
			pos1 = posd1;
		if (pos1 < posd1 and posd1 < motors["l4"].pos)
			pos1 = posd1;
		
		
		RoboCompJointMotor::MotorGoalPosition goal;
		goal.name = "l4";
		goal.position = pos1;
		goal.maxSpeed = 1.4;

		qDebug()<<"pos des"<<(float)((1.+sin(caca)))/*mList["XN_SKEL_LEFT_SHOULDER"].at(3)+M_PI*/<<"motor"<<motors["r1"].pos<<"pos"<<pos1<<"vel"<<vel1<<"acc"<<acc_d_1;
		
/*		QTime now = QTime::currentTime();
		if (lastQuery.msecsTo(now) > JointPeriod)
		{
			lastQuery = now;
			goalList.clear();
			
			qDebug()<<"*************";
		foreach (QString key, positions.keys())
			{
//				if(fabs(positions[key] - motors[key.toStdString()].pos) < RANGE)
				{
					goal.name = key.toStdString();
					goal.position = positions[key];
					goalList.push_back(goal);
					goal.maxSpeed = fabs(positions[key] - motors[key.toStdString()].pos) * 5.f;
					qDebug()<<key<<positions[key]<<"actual"<<motors[key.toStdString()].pos<<"speed"<<goal.maxSpeed;
				}
			}*/
			try
			{
				jointmotor->setPosition(goal);
			}
			catch(const Ice::Exception& e)
			{
				qDebug()<<"Error setting position to jointMotor"<<e.what();
			}
//		}
		usleep(Period);
	}
}

void SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	this->start();
};
