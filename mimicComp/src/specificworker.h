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
#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#define RANGE 0.5
#define ERROR 0.2

#include <genericworker.h>

/**
       \brief
       @author authorname
*/

class SpecificWorker : public GenericWorker
{
Q_OBJECT
public:
	SpecificWorker(MapPrx& mprx, QThread *parent = 0);	
	~SpecificWorker();
	void setParams(RoboCompCommonBehavior::ParameterList params);
	
	void run();
private:
	RoboCompJointMotor::MotorParamsList params;
	RoboCompJointMotor::MotorGoalPositionList goalList;
	RoboCompJointMotor::MotorStateMap motors;
//	RoboCompEvaluadorOff::RTMatrixList mList;
	RoboCompTrackeropenni::RTMatrixList mList;
	QTime lastQuery;
	int JointPeriod;
	QHash<QString,float> positions,error;
	
};

#endif