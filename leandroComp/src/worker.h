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

#include <Camera.h>	//Proxy at CameraComp.
#include <Kinect.h>	//Proxy at KinectComp.

#include <qlog/qlog.h>		//RoboComp Class to write a log file.
#include <rcdraw/rcdraw.h>	//RoboComp Class to draw video & images in QT qFrames.

#include <QtGui> 			//QT GUI libraries.
#include <QProcess>
#include "ui_leandroDlg.h" 	//My component GUI .h with QT Designer.

#include <stdio.h>	//Standard in/out C++ libraries.
#include <iomanip> 	//C++ library to manipulate standard in/out flow.
#include <iostream>	//C++ library to standard in/out flow.
#include <ctype.h>	//Library C.
#include <stdlib.h> //system("/bin/sh -c myfile.sh"); To run Bash Scripts from C++
#include <string>	//C++ standard library to manage strings.

//Open CV
#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>
#include <ipp.h>


using namespace cv;
using namespace std;

#define CHECK_PERIOD 5000
#define BASIC_PERIOD 50
#define FOV 62.7	//kinect FOV (Field of Vision)

/**
       \brief
       @author leandro
*/
	struct CircleCV
	{
	  int x, y, radius;
	  Scalar color;
	  Point center;
	};
	struct FingersCVcolor
	{
	  Scalar orange, green, pink, blue, aux;
	  Scalar toleranceO, toleranceG, toleranceP, toletanceB, toleranceAU;
	};
	struct Rectangle
	{
	  int x;
	  int y;
	  int width;
	  int height;
	};
  
	
	struct Target 
	{
	  RotatedRect trackBox;
	  Rect trackWindow;
	  Mat hist;
	  Mat patron;
	  Mat hue;
	  Mat backproj;	
	  bool visible;	
  // 	Mat auxErode;	
	  QVec coor3D,despla,wH;
	  int contPerdido;
	  Mat mascara;
	  
	  //color elipse
	  Scalar color;
	  QColor colorQT;
	  string ventana;
	  QString ventanaQString;
	  string ventanaHist;
  // 	Mat imageGRAY;
	};
// 	struct Anyone
// 	{
// 	  
// 	};
class Worker : public QWidget , private Ui_leandroDlg
{
Q_OBJECT
public:
	Worker(RoboCompCamera::CameraPrx cameraprx, RoboCompKinect::KinectPrx kinectprx, QWidget *parent = 0);
	~Worker();
	void killYourSelf();
	void setPeriod(int p);
	QMutex *mutex;                //Shared mutex with servant
	
private:
	QTimer timer;
	QTimer timerOneFrameLater;
	int Period;
	
	//PROXIES-------------------------
	RoboCompCamera::CameraPrx camera;
	RoboCompKinect::KinectPrx kinect;
	//IMAGES--------------------------
	RoboCompKinect::rgbType imageRGB;
	RoboCompKinect::irType imageIR;
	RoboCompKinect::depthType depth;
	//STATES--------------------------------------
	RoboCompCommonHead::THeadState hState;
	RoboCompDifferentialRobot::TBaseState bState;
	//QT4 & USER INTERFACES-----------------------
	RCDraw *rcd2;
	QImage *qImageRGB, *qImageIR, *qImageTest;
	QColor qColorRGB;
    QRgb valueRGB;
	int r_QTvalue, g_QTvalue, b_QTvalue, a_QTvalue;
	int luxValue, luxRangeValue;
	//ROBOCOMP PROCESSING--------------------------
	QProcess kinectProcessStart, kinectProcessStop;
	string robocompPath;
	string kinectPathBin;
	bool runKinect;
	//IMAGE SIZES-----------------------------------------
	int imgHeight, imgWidth, imgHeightIR, imgChannelsRGB;
	//DEPTH-----------------------------------------------
	float minDepth, maxDepth, depthValue, depthRangeValue;
	//BOOL TO WORKING MODES-------------------------------
	bool calibration, irORrgb;
	//OpenCV-------------------------------------------------
	Point pointCVtarget, pointCVul, pointCVdr, pointCVcenter, pointCVaux;
	Rect rectangleCV;
	Size ksizeCV;
	Mat imageCVrgb, imageCVgray, imageCVrgbINV, imageCVrgbAUX;
	Mat maskCV3x3;
	vector<Mat> threeCVplanes;
	vector<Mat> threeCVplanesINV;
	MatIterator_<uchar> iteratorCVinit, iteratorCVfinal;
	float optionsCV[3];
	int counterCV;
	CircleCV circle;
  //OPENCV CAMSHIFT TRACKING-----------
	Mat backProjImageCV;
	Rect windowCV;
	TermCriteria criteriaCV;

	int narrays;
	CvHistogram histogramCV;
  //OPENCV LUIKY TRACKING---------------
	float hranges[2];
	const float* phranges;
	int hsize;
	Mat histogram;
	RotatedRect trackBox;
	Rect trackWindow;

		QVector<Target> v;
	
public slots:
  
  //GUI SLIDERS---------------------//
	void depth_Slider(int);			//
	void lux_Slider(int);			//
	void depthRange_Slider(int);	//
	void luxRange_Slider(int);		//
  //GUI BUTTONS-----------------//
	void pbPaintZFromIRCam();	//
	void pbPaintZFromRGBCam();	//
	void pbPaintIR();			//
	void pbPaintRGB();			//
	void pbStartKinect();		//
	void pbStopKinect();		//
  //OPENCV BUTTONS------------------//
	void pbOpenCV1();			//
	void pbOpenCV2();			//
	void pbOpenCV3();			//
	void pbOpenCV4();			//
  //PAINT IN FRAME----------//
	void paintAxies();		//
	void paintVideo();		//
	void paintRB_Depth();	//
	void paintWelcomeMSG();	//
  //OPENCV------------------------------------------------------------------//
	void openCVwork();														//
	void paintCV3images(Mat,Mat,Mat);										//
	void invertCVrgbColor(Mat,Mat);											//
	void getPreviousImageCV(Mat, Mat);										//
	CircleCV initCircleCV(int x, int y, int radius, int r, int g, int b);	//
// 	void initFingersCV();													//
	void myTracking();
  //OPENCV LUIKY TRACKING-----------------------------//
	Mat calcHistogramCV (Mat imageCV1);
	Mat getHistogramCV(const cv::Mat histogram);
	void trackCamShift();
  //COMPUTE: ROBOCOMP MAJOR LOOP----//
	void compute();					//

signals:
	void kill();
};

#endif
