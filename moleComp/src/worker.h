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

/// Intel IPP includes
#include <ipp.h>
/// QT4 includes
#include <QtGui>
#include <QTime>
#include <qglobal.h>
/// RoboComp includes
#include <qlog/qlog.h>
#include <CommonBehavior.h>
/// OpenCV includes
#include <cv.h>
#include <cvaux.h>
#include <cvwimage.h>
//#include <cvconfig.h>
/// Standar C++ includes
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
/// My component proxies includes
#include <Camera.h>
#include <Kinect.h>
/// My component GUI includes
#include <ui_guiDlg.h>
/// My component classes includes
#include "osgArt.h"

/// BASIC_PERIOD is a common variable in RoboComp components, this is the period of the QTimer.
#define BASIC_PERIOD 100
/// PI is the number pi to work with angles and radians more easy.
#define PI 3.14159265358979323846

using namespace std;

/**
       \brief
       @author authorname
*/
class Worker  : public QWidget, public Ui_guiDlg
{
Q_OBJECT
public:
	Worker(RoboCompCamera::CameraPrx cameraprx, RoboCompKinect::KinectPrx kinectprx);
	~Worker();
	//CommonBehavior
	void killYourSelf();
	void setPeriod(int p);
	bool setParams(RoboCompCommonBehavior::ParameterList params_);
	
	QMutex *mutex;                		// Shared mutex with servant
	bool kinectOrCam;			// Select image source (Kinect or Webcam).
	int distance2mole1;			// Distance between Hammer ans Mole1.
	int distance2mole2;			// Distance between Hammer and Mole2.
	int distance2mole3;			// Distance between Hammer and Mole3.
	int score;				// Score for the game.
	int scoreZoom;				// Change the size of the score: scoreText and scoreBox.
	string scoreString;			// Score in string format to show in OSG scene.
	double m1[3][4], m2[3][4], m3[3][4];	// Auxiliar double matrix to calculate distances between Hammer and Moles.
	
	bool didMyWork1;			// Auxiliar bool to know when the program did a task.
	bool didMyWork2;			// Auxiliar bool to know when the program did a task.
	
	int xMoles;				// Moles movement in X axis.
	int yMoles;				// Moles movement in Y axis.
	int zMoles;				// Moles movement in Z axis.
	
	int randomValue;
	QTime systemTime;
	
	float aux;
	
	osg::MatrixTransform* mtMole;		// MatrixTransform controled by the ARToolKit mark. Moles are attached here.
	osg::MatrixTransform* mtHammer;		// MatrixTransform controled by the ARToolKit mark. Hammer is attached here.
	
	osg::MatrixTransform* scoreBox;		// The background white plane for the score.
	osg::MatrixTransform* scoreText;	// The position of the score text.
	osgText::Font* scoreTextFont;		// The font for the score text, usually Arial.
	
// 	osg::MatrixTransform* coneX;		// Auxiliar MatrixTransform to get control of the cone that represent the X axis.
// 	osg::MatrixTransform* coneY;		// Auxiliar MatrixTransform to get control of the cone that represent the Y axis.
// 	osg::MatrixTransform* coneZ;		// Auxiliar MatrixTransform to get control of the cone that represent the Z axis.

	osg::MatrixTransform* mole1base;	// This MatrixTransform controles a invisible capsule for hammer-mole1 interaction. mole1head is attached here.
	osg::MatrixTransform* mole2base;	// This MatrixTransform controles a invisible capsule for hammer-mole2 interaction. mole2head is attached here.
	osg::MatrixTransform* mole3base;	// This MatrixTransform controles a invisible capsule for hammer-mole3 interaction. mole2head is attached here.
	osg::MatrixTransform* mole1head;	// This MatrixTransform controles mole1 models amination. Models are attached here.
	osg::MatrixTransform* mole2head;	// This MatrixTransform controles mole2 models amination. Models are attached here.
	osg::MatrixTransform* mole3head;	// This MatrixTransform controles mole3 models amination. Models are attached here.
	
	osg::MatrixTransform* hammer1head;	// This MatrixTransform controles head cylinder of the hammer.
	osg::MatrixTransform* hammer1base;	// This MatrixTransform controles handle cylinder of the hammer.
	
	osg::MatrixTransform* mtHammer2mole;	// Auxiliar MatrixTransform to calculate the distances between Hammer and Moles.
	
	osg::MatrixTransform* mtModelLibrary;	// Auxiliar MatrixTransform to keep alive the mole models when set and unset as child to the MatrixTransform.

	osg::Node* modelNode1;
	osg::Node* modelNode2;
	osg::Node* modelNode3;


public slots:
	void compute(); //main method
	/// Choose between Kinect or WebCam video.
	void fn_pbKinectOrCam(); //Select image source (Kinect or Webcam)
	/// Create all osg primitives (ball, cylinder, cone...)
	void startOSGArt();
	
	/// Convert int values to string to show int values in OSG scene.
	std:: string int2string(int n)
	{
	  std:: stringstream flujo;
	  flujo << n;
	  return(flujo.str());
	}
	
	/// Return random number between low and high
	int randomInt(int low, int high){return qrand() % ((high + 1) - low) + low;}
	
	void fn_pbMole1();
	void fn_pbMole2();
	void fn_pbMole3();
	void fn_sliderMoleX(int);
	void fn_sliderMoleY(int);
	void fn_sliderMoleZ(int);
	void fn_pbHammer1();
	void fn_pbHammer2();
	void fn_pbHammer3();
	void fn_sliderHammerX(int);
	void fn_sliderHammerY(int);
	void fn_sliderHammerZ(int);
	void fn_spinBoxScoreZoom(double);
	
	/// Get the OpenCV version you are using and the modules installed.
	void getOpenCVversion();
	
	/// Get double[3][4] matrix from a Matrix Transform.
	void matrixTransform2matrixDouble(osg::MatrixTransform* mtScr, double matrixDst[3][4]);
	
	/// Get the Matrix Transform from a double[3][4] matrix.
	void matrixDouble2matrixTransform(double matrixSrc[3][4], osg::MatrixTransform* mtDst);
	
	/// Get the MatrixTransform of one MatrixTransform from other MatrixTransform coordinates. mtA_from_mtB mt2mtCoordinates(mtA, mtB);
	osg::MatrixTransform* mt2mtCoordinates(osg::MatrixTransform* mtA, osg::MatrixTransform* mtB);
	
	/// Run the random status of the moles, hidden or avaible to get hits.
	void setRandomMolesStatus(int randomInt, QTime systemTime);
	
	/// Calcule the distances between the hammer and each mole, and load the modelNode2 if the distance is lower than threshold.
	void checkWhichMoledWasHit(int threshold);
	

	 
private:
	int Period;
	QTimer timer;
	RoboCompCamera::CameraPrx camera;
	RoboCompKinect::KinectPrx kinect;
	RoboCompCamera::imgType img;
	RoboCompKinect::depthType imgDepth;
	RoboCompDifferentialRobot::TBaseState tBaseState;
	RoboCompCommonHead::THeadState tHeadState;
	 
	OsgArt *osgArt;
	
	
signals:
	void kill();
};

#endif
