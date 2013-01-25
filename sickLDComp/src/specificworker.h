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

#include <genericworker.h>
#include <sickld-1.0/SickLD.hh>
using namespace SickToolbox;
/**
       \brief
       @author authorname
*/

class SpecificWorker : public GenericWorker
{
Q_OBJECT
private:
	RoboCompLaser::TLaserData laserData;
	RoboCompLaser::LaserConfData laserConf;
	RoboCompDifferentialRobot::TBaseState bState;

	SickLD        	*sickld;
	double 			polarvalues[SickLD::SICK_MAX_NUM_MEASUREMENTS]; // Uses macro defined in SickLD.hh
	unsigned int 	num_values;
	double       	laserPolarData[SickLD::SICK_MAX_NUM_MEASUREMENTS];	
	bool talkToBase;
	QMutex *lDataMutex;
	
public:
	SpecificWorker(MapPrx& mprx, QThread *parent = 0);	
	~SpecificWorker();
	void setParams(RoboCompCommonBehavior::ParameterList params);
	TLaserData getLaserData();
	TLaserData getLaserAndBStateData(RoboCompDifferentialRobot::TBaseState& state);
	LaserConfData getLaserConfData();

	void uninitialize();
	
private:
	void initialize();
	void run();
	bool readLaserData();
	
};

#endif