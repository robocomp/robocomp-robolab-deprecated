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
#include<iostream>

#include <ipp.h>
#include <QtGui>
#include <stdint.h>
#include <qlog/qlog.h>
#include <Aprendiz.h>

#include <osg/Geode>
#include <osg/TexGen>
#include <osg/Texture1D>
#include <osg/Texture2D>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>

#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Drawable>

#include <osgUtil/IntersectVisitor>

#include <cassert>
#include <unistd.h>
#include <stdio.h>
#include <modelo-osg/utils.h>
#include <modelo-osg/modelo.h>
#include <modelo-osg/modeloVISOR.h>
#include <modelo-osg/modeloVISORpalotes.h>
#include <modelo-osg/modeloHOAP.h>
#include <modelo-osg/modeloHOAPpalotes.h>
#include <modelo-osg/modeloNOMAD.h>
#include <modelo-osg/modeloNOMADpalotes.h>

#include <obs/observadormodelo.h>
#include <obs/observadormodelo.h>
#include <obs/observadormodelocoli.h>
#include <retargeting/retargeting.h>

#include <osg/io_utils>

#include <set>
#include <QString>



#define CHECK_PERIOD 5000
#define BASIC_PERIOD 50

using namespace std;

/**
       \brief
       @author authorname
*/
class Worker : public QObject
{
Q_OBJECT

private:
	QTimer timer;
	QTime t;
	int Period;
	
public:
	Worker(QObject *parent = 0);
	~Worker();
	void setWrist(Vec3 pose );
	void setElbowWrist(Vec3 wrist, Vec3 elbow, std::vector <float> m);
	void getShoulders(RoboCompAprendiz::coor3D& leftShoulder, RoboCompAprendiz::coor3D& rightShoulder);
	void getElbows(RoboCompAprendiz::coor3D& leftElbow, RoboCompAprendiz::coor3D& rightElbow);
	std::vector<double> getAnglesRight();
	void setAngleElbowWrist(float wrist, float elbow);
	void killYourSelf();
	void setPeriod(int p);
	QMutex *mutex;                //Shared mutex with servant
		// construct the viewer.
  osgViewer::Viewer viewer;
 
	std::ofstream myfile;


public slots:
	void compute();
    
  
    
signals:
	void kill();
};

#endif
