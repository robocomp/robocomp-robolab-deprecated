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
#include <QtCore>
#include <stdint.h>
#include <qlog/qlog.h>
#include <DifferentialRobot.h>
#include <Laser.h>
#include <sicklms-1.0/SickLMS.hh>

using namespace SickToolbox;
/**
       \brief
       @author authorname
*/
class Worker : public QThread
{
Q_OBJECT
private:
	RoboCompLaser::TLaserData laserData;
	RoboCompLaser::LaserConfData laserConf;
	RoboCompDifferentialRobot::DifferentialRobotPrx differentialrobot;
	RoboCompDifferentialRobot::TBaseState bState;
	QTimer timer;
	int Period;
	
	SickLMS        *sicklms;
	std::string     sicklms_dev_path;
	sick_lms_baud_t sicklms_desired_baud;
	double          sick_scan_angle;
	double          sick_scan_resolution;
	unsigned int polarvalues[SickLMS::SICK_MAX_NUM_MEASUREMENTS]; // Uses macro defined in SickLMS.hh
	unsigned int num_values;
	double       laserPolarData[361];
	double div_factor;
	double scanres;
	sick_lms_measuring_units_t units;
  
public:
	Worker(RoboCompDifferentialRobot::DifferentialRobotPrx differentialrobotprx, QObject *parent = 0);
	~Worker();
	void killYourSelf();
	void setPeriod(int p);
	RoboCompLaser::TLaserData getNewData();
	RoboCompLaser::LaserConfData getLaserConf();
	RoboCompDifferentialRobot::TBaseState getBaseData();
private:
	void run();
	bool readLaserData();

	
public slots:
	RoboCompDifferentialRobot::TBaseState getBaseState()
	{
		RoboCompDifferentialRobot::TBaseState b;
		try
		{
			differentialrobot->getBaseState(b);
		}
		catch(Ice::Exception e)
		{
			rDebug("Error talking to differentialRobot");
		}
		return b;
	}
signals:
	void kill();
};

#endif
