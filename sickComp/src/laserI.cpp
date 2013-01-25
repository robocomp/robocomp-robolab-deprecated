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
#include "laserI.h"

LaserI::LaserI(Worker *_worker, QObject *parent) : QObject(parent)
{
	worker = _worker;
	// Component initialization...
}


LaserI::~LaserI()
{
	// Free component resources here
}

// Component functions, implementation

RoboCompLaser::TLaserData LaserI::getLaserAndBStateData(RoboCompDifferentialRobot::TBaseState &bState,const Ice::Current& )
{
	bState = worker->getBaseState();
	return worker->getNewData();
}


RoboCompLaser::TLaserData LaserI::getLaserData(const Ice::Current &)
{
/*	RoboCompLaser::TLaserData data = lh->getNewData();
	std::sort(data.begin(), data.end(), TLaserDataSortPredicate);
	return data;*/
	return worker->getNewData();
}
LaserConfData LaserI::getLaserConfData( const Ice::Current&) 
{ 
	return worker->getLaserConf();
}


