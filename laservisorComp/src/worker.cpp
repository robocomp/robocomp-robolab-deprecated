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
Worker::Worker(RoboCompLaser::LaserPrx laserprx, QObject *parent) : QObject(parent)
{
	laser = laserprx;

	QWidget *widget = new QWidget();
	widget->resize(600,600);
	widget->show();
	world = new RCDraw(600,600,widget);
	mutex = new QMutex();
	
	
	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));
	Period = BASIC_PERIOD;
	timer.start(Period);
	
	try{
		laserConf = laser->getLaserConfData();
	}
	catch (const Ice::Exception& ex)
	{
		rDebug("Error reading laser configuration data");
	}
	
	
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
void Worker::compute( )
{
	
	try{
		laserData = laser->getLaserData();
		drawLaser(laserData);
		world->update();
	}
	catch (const Ice::Exception& ex)
	{
		rDebug("Error reading laser data");
	}
	
	
}
void Worker::drawLaser(const RoboCompLaser::TLaserData & laserData)
{
//	qDebug()<<"Draw laser"<<laserData.size();
	int x,y;
	for (unsigned int i=0; i< laserData.size(); i++)
	{
		x = cos(laserData[i].angle-M_PI_2)*laserData[i].dist*0.1 + world->getWidth()/2;
		y = sin(laserData[i].angle-M_PI_2)*laserData[i].dist*0.1 + world->getHeight()/2;
		
		world->drawSquare(QRect(x-1,y-1,2,2),Qt::red);
	}
}

