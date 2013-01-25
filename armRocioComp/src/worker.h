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
#ifndef WORKER_H
#define WORKER_H

// #include <ipp.h>
#include <QtGui>
#include <stdint.h>
#include <stdlib.h>

#include <JointMotor.h>


#define CHECK_PERIOD 5000
#define BASIC_PERIOD 100


/**
       \brief
       @author authorname
*/
class Worker : public QObject
{
Q_OBJECT
public:
	Worker(RoboCompJointMotor::JointMotorPrx jointmotorprx, QObject *parent = 0);
	~Worker();
	//QMutex *mutex;                //Shared mutex with servant

	RoboCompJointMotor::JointMotorPrx jointmotor;
	RoboCompJointMotor::MotorParamsList params;
	RoboCompJointMotor::MotorStateMap motors;
	RoboCompJointMotor::MotorGoalPositionList goals,left,right,both;
	
	QVector<float> rampa;
	QVector<float> r_vmax;
	QVector<float> vLimites;
	QVector<float> pDistances;
	QVector<float> tcambio1;
	QVector<float> tcambio2;
	QVector<float> poseEstimada;
	QVector<float> poseInicial;
	float tiempo;
	QTime tmove;
	bool move;
	
	void setPosition(const float vec[],std::string arm);
	bool checkPosition(const float vec[]);
	void setMaxSpeed(float speed);
	
	
	void sendJoint(RoboCompJointMotor::MotorGoalPositionList list);
private:
	QTimer timer;
	std::string s1;
	std::string s2;
	std::string s3;
	std::string s4;

	
	void computeTime();
	
public slots:
	void compute();
    
};

#endif
