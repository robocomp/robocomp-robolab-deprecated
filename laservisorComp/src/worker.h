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
#include <qlog/qlog.h>
#include <DifferentialRobot.h>
#include <Laser.h>
#include <rcdraw/rcdraw.h>

#define CHECK_PERIOD 5000
#define BASIC_PERIOD 100


using namespace std;

/**
       \brief
       @author authorname
*/
class Worker : public QObject
{
Q_OBJECT
public:
	Worker(RoboCompLaser::LaserPrx laserprx, QObject *parent = 0);
	~Worker();
	void killYourSelf();
	void setPeriod(int p);
	void drawLaser(const RoboCompLaser::TLaserData & laserData);
	QMutex *mutex;                //Shared mutex with servant
private:
	
	RoboCompLaser::LaserConfData laserConf;
	RoboCompLaser::LaserPrx laser;
	RoboCompLaser::TLaserData laserData;
	RCDraw *world;
	QTimer timer;
	int Period;
public slots:
	void compute();
	
signals:
	void kill();
};

#endif
