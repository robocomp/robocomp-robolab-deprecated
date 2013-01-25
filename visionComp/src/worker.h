#ifndef WORKER_H
#define WORKER_H

#include <QtCore>
#include <qimage.h>

//#include "../SiftGPU/src/SiftGPU.h"
//#include <GL/gl.h>

#include <Camera.h>
#include <Vision.h>
#include <ipp.h>
#include <config.h>
#include <roidetector.h>

//#define MAX_SIFT 512
//#define SIFT_DIM 128


typedef struct
{
		int pyramidLevels;
		int radius;

} TConfigParams;

class Worker : public QObject
{
	Q_OBJECT
	public:
		Worker( const RoboCompCamera::CameraPrx & camera, TConfigParams _configParams, QObject *parent = 0);
		~Worker();
	
		QMutex *mutex;
		RoboCompCamera::TCamParams paramsCamera;
		RoboCompVision::TCamParams paramsVision;
		inline bool isChecking() 	{ return CHECKING; };
		inline bool isMoving() 		{ return ISMOVING; };
	
		void getLevelPyr(uchar cam , int level, imgType &img, RoboCompVision::TState & state);
		void getWholePyr(uchar cam , imgType &pyr, RoboCompVision::TState & state );
		void getROILevelPyr(uchar cam , int level, int xc, int yc, int w, int h, imgType &img, RoboCompVision::TState & state);
		void getROIWholePyr(uchar cam , int xc, int yc, int w, int h, imgType &img , RoboCompVision::TState & state);
		void getLevelROIList(uchar cam , int level, RoboCompVision::ROIVector &roiList , RoboCompVision::TState & state);
		void getWholeROIList(uchar cam , RoboCompVision::ROIPyrVectors &roiList, RoboCompVision::TState & state);
		void getWholePyrAndROIListBothCam( imgType& imgLeft , RoboCompVision::ROIPyrVectors & roiListLeft,  imgType& imgRight , RoboCompVision::ROIPyrVectors & roiListRight,  RoboCompVision::TState & state);
		void getWholePyrBothCam( imgType& imgLeft, imgType& imgRight, RoboCompVision::TState & state);
		bool getWholePyrAndROIList( std::string cam, imgType &img , RoboCompVision::ROIPyrVectors &roiList, RoboCompVision::TState &state);
  
    public slots:
		void compute();

	private:
		void printFPS( );
		unsigned int indexOfCamera( std::string cam );
		RoboCompCamera::CameraPrx camera;
		RoboCompCamMotion::THeadState hState;
		RoboCompDifferentialRobot::TBaseState bState;
		QTimer * timer;
		int BASIC_PERIOD;
		bool CHECKING;   									//true if there are connection problems
		bool ISMOVING;  									// false if CameraComp could not supply an image.
		QImage *qimgL, *qimgR;
		RoboCompCamera::imgType imagenV;
	
		//Harris-Laplace
		roiDetector *ROIDetect[MAX_CAMERAS];	
	
		//State
		RoboCompVision::TState state, stateNew;
};

#endif
