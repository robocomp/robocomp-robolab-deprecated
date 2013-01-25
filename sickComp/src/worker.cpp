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
Worker::Worker(RoboCompDifferentialRobot::DifferentialRobotPrx base_prx, QObject *_parent) 
{
	differentialrobot = base_prx;

	
}

/**
* \brief Default destructor
*/
Worker::~Worker()
{

}
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
	Period = p;
	timer.start(Period);
}


RoboCompLaser::TLaserData Worker::getNewData()
{
	return laserData;
}

RoboCompLaser::LaserConfData Worker::getLaserConf()
{
	return laserConf;  
}

RoboCompDifferentialRobot::TBaseState Worker::getBaseData()
{
	return bState;
}


void Worker::run()
{
	forever
	{
		try{
			differentialrobot->getBaseState(bState);
		}
		catch(Ice::Exception e)
		{
			rError("Error talking to differentialRobot");
		}
		if (readLaserData()==false)
		{
			std::cout << "Error reading laser " << std::endl;
		}
		usleep(Period*100);
	}
}
bool Worker::readLaserData()
{
	// Read laser data (polar)
// 	laserData.clear();
	try 
	{
		sicklms->GetSickScan(polarvalues, num_values);
			
		if(num_values > 361) num_values = 361;
		for(unsigned int i = 0; i < num_values; i++)
		{
			laserPolarData[i] = (double)polarvalues[i]/div_factor;
			laserData[i].angle = (180- i*0.5)*M_PI/180.f;
			laserData[i].dist = (double)polarvalues[i]/div_factor*1000;
		}
	}
    // Catch possible exceptions
  	catch(...) 
	{
		std::cerr << "Error while receiving laser data\n" << std::endl;
		return false;
  	}  
  	return true;
}