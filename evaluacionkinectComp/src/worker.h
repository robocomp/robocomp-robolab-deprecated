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

#include <Kinect.h>
#include "ui_evaluacionDlg.h"
#include <QMat/QMatAll>

#include <rcdraw/rcdraw.h>
#include <fstream>


#define CHECK_PERIOD 5000
#define BASIC_PERIOD 67
#define FOCAL 525

using namespace std;


/**
       \brief
       @author authorname
*/
class Worker : public QWidget, private Ui_evaluacionForm
{
Q_OBJECT
public:
	Worker(RoboCompKinect::KinectPrx kinectprx, QObject *parent = 0);
	~Worker();
	void killYourSelf();
	void setPeriod(int p);
	QMutex *mutex;                //Shared mutex with servant

	RoboCompKinect::KinectPrx kinect;
	RoboCompKinect::depthType z_vector, z_vectorIR;
	RoboCompKinect::rgbType imgRGB;
	RoboCompDifferentialRobot::TBaseState bState;
	RoboCompCommonHead::THeadState hState;
	
	RCDraw *rcdRGB,*rcdZ,*rcdRGBZ;
	QImage *qImgRGB,*qImgZ,*qImgRGBZ;
	Ipp8u *imgZ;
	int imgAlto, imgAncho;
	float maxDepth,minDepth;
	bool grab,capture;
	QRect roiGrab;
	QFileDialog dialog;
		
	IppiSize ippSizeImage;
	
	
private:	
	QTimer timer;	
	std::ofstream file;
	QString nameFile;
	int Period;
	void writeFile(QRect r);
	
public slots:
	void mousePress(QPointF p );
	void mouseRelase(QPointF p  );
	void compute();
	void on_pbCapture_clicked();
signals:
	void kill();
};

#endif
