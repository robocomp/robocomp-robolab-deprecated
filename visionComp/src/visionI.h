#ifndef VISIONI_H
#define VISIONI_H

#include <string>
#include <iostream>

#include <QtCore>
#include <Ice/Ice.h>

#include <config.h>
#include <worker.h>

using namespace RoboCompCamera;

class VisionI : public QObject , public virtual RoboCompVision::Vision
{
Q_OBJECT
public:
	VisionI ( Worker *worker, QObject *parent = 0 );
	~VisionI();

	void getLevelPyr ( Ice::Byte cam, Ice::Int level, imgType& roi, RoboCompVision::TState & state, const Ice::Current &);
	void getWholePyr ( Ice::Byte cam, imgType& roi, RoboCompVision::TState & state, const Ice::Current & );
	void getLevelROIList ( Ice::Byte cam, Ice::Int level, RoboCompVision::ROIVector & roiList, RoboCompVision::TState & state, const Ice::Current & );
	void getWholeROIList ( Ice::Byte cam, RoboCompVision::ROIPyrVectors & roiList, RoboCompVision::TState & state, const Ice::Current & );
	void getWholePyrAndROIListBothCam( imgType& imgLeft , RoboCompVision::ROIPyrVectors & roiListLeft,  imgType& imgRight , RoboCompVision::ROIPyrVectors & roiListRight,  RoboCompVision::TState & state, const Ice::Current &);
	void getROILevelPyr ( Ice::Byte cam, Ice::Int level, Ice::Int xc, Ice::Int yc, Ice::Int w, Ice::Int h, imgType& img, RoboCompVision::TState & state , const Ice::Current & );
	void getROIWholePyr ( Ice::Byte cam, Ice::Int xc, Ice::Int yc, Ice::Int w, Ice::Int h, imgType& img, RoboCompVision::TState & state , const Ice::Current & );
	RoboCompVision::TCamParams getCamParams ( const Ice::Current& );
	void getWholePyrBothCam( imgType& imgLeft, imgType& imgRight, RoboCompVision::TState & state, const Ice::Current &);
	void getWholePyrAndROIList( const std::string & cam, imgType& img , RoboCompVision::ROIPyrVectors & roiList, RoboCompVision::TState & state, const Ice::Current &);
	
private:
	Worker *worker;
	QMutex *mutex;
};

#endif
