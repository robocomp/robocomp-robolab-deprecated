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
#include <osgviewer/osgview.h>
#include <osg/io_utils>

#include <innermodel/innermodelviewer.h>

#include <QFile>
#include "openFileDlg.h"
#include "graphics.h"


/**
       \brief
       @author leandro
*/

class SpecificWorker : public GenericWorker
{
Q_OBJECT
protected:
     void closeEvent(QCloseEvent *event);

public:
	OsgView *osg;
	InnerModel *innerModel;
	InnerModelViewer *imv;
	int alto, ancho;
	SpecificWorker(MapPrx& mprx);	
	~SpecificWorker();
	void  setParams(RoboCompCommonBehavior::ParameterList params);
	void  getRTMatrixList(RTMatrixList& RTMatList);
	void  updateTimeBar();
	void  getHeaderInfo();
	void  updateLCDangles(QString jointName, float rotationXrad, float rotationYrad, float rotationZrad);
	//---------------------------------- Variables for read files.
	QFile sessionFile;
	QTextStream sessionStream;
	QString sessionTextLine;
	QString sessionFilePath;
	int numberOfSessionJoints;
	int indexBlock;
	bool toPlay;
	bool toLoop;
	bool fileLoaded;
	bool toShowGraph;
	int timeBarValue;
	QTime sessionInitialTime;
	QTime sessionUpdatedTime;
	float sessionMSecs;
	float sessionPeriod;
	int lineTextNumber;
	RTMatrixList RTMatList;
	QMutex* mutexRTMatList;
	
	OpenFileDlg openFileDlg;
	graphics *sessionGraph;
	
	
	

public slots:
	void compute();
	QString getFileName();
	void loadSessionFile(QString pathSessionFile);
	void updateRTMatrix(QStringList);
	void play();
	void pause();
	void stop();
	void setLoop();
	void setSpeedMore();
	void setSpeedLess();
	void resetSpeed();
	void showGraph();
};

#endif