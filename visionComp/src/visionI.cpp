#include "visionI.h"

VisionI::VisionI( Worker *worker, QObject *parent) : QObject(parent)
{
	this->worker = worker;
	this->mutex = worker->mutex;
}

VisionI::~VisionI()
{

}

//Servant methods

void VisionI::getLevelPyr(Ice::Byte cam, Ice::Int level, imgType & roi, RoboCompVision::TState & state, const Ice::Current &)
{
	if (cam > worker->paramsVision.numCams-1 or cam <0)
	{
		RoboCompVision::HardwareFailedException hw;
		QString str = QString("No Camera with number %1.").arg(cam);
		hw.what = str.toStdString();
		throw hw;
	}
	if (worker->isChecking() )
	{
		RoboCompVision::ConnectionFailedException con;
		con.what = "VisionComp disconnected from CameraComp.";
		throw con;
	}
// 	if (worker->isMoving()==true)
// 	{
// 		RoboCompVision::MovingImageException ex;
// 		throw ex;
// 	};

	worker->getLevelPyr(cam,level,roi, state);
}

void VisionI::getWholePyr(Ice::Byte cam, imgType & roi, RoboCompVision::TState & state, const Ice::Current &)
{
	if( worker->isChecking() ==true)
	{
		RoboCompVision::ConnectionFailedException con;
		con.what = "VisionComp disconnected from CameraComp.";
		throw con;
	}
	if( cam > worker->paramsVision.numCams-1 )
	{
		RoboCompVision::HardwareFailedException hw;
		QString str = QString("No Camera with number %1.").arg(cam);
		hw.what = str.toStdString();
		throw hw;
	}
// 	if (worker->isMoving()==true)
// 	{
// 		RoboCompVision::MovingImageException ex;
// 		throw ex;
// 	};
	worker->getWholePyr(cam, roi, state);
}

void VisionI::getLevelROIList(Ice::Byte cam, Ice::Int level, RoboCompVision::ROIVector & roiList, RoboCompVision::TState & state, const Ice::Current &)
{
	if( worker->isChecking() ==true)
	{
		RoboCompVision::ConnectionFailedException con;
		con.what = "VisionComp disconnected from CameraComp.";
		throw   con;
	}
	if( cam > worker->paramsVision.numCams-1 )
	{
		RoboCompVision::HardwareFailedException hw;
		QString str = QString("No Camera with number %1.").arg(cam);
		hw.what = str.toStdString();
		throw hw;
	}
// 	if (worker->isMoving()==true)
// 	{
// 		RoboCompVision::MovingImageException ex;
// 		throw ex ;
// 	};
	worker->getLevelROIList(cam, level,roiList, state);
}

void VisionI::getWholePyrAndROIList( const std::string & cam, imgType& img , RoboCompVision::ROIPyrVectors & roiList, RoboCompVision::TState & state, const Ice::Current &)
{
	if( worker->isChecking() == true)
	{
		RoboCompVision::ConnectionFailedException con;
		con.what = "VisionComp disconnected from CameraComp.";
		throw   con;
	}
	if ( worker->getWholePyrAndROIList( cam, img , roiList, state) == true );
	else
	{
		RoboCompVision::HardwareFailedException con;
		con.what = "You are probably asking for an non-existing camera: " + cam;
		throw   con;
	}
}

void VisionI::getWholeROIList(Ice::Byte cam, RoboCompVision::ROIPyrVectors & roiList, RoboCompVision::TState & state, const Ice::Current &)
{
	if( worker->isChecking() ==true)
	{
		RoboCompVision::ConnectionFailedException con;
		con.what = "VisionComp disconnected from CameraComp.";
		throw   con;
	}
	if( cam > worker->paramsVision.numCams-1 )
	{
		RoboCompVision::HardwareFailedException hw;
		QString str = QString("No Camera with number %1.").arg(cam);
		hw.what = str.toStdString();
		throw hw;
	}
// 	if (worker->isMoving()==true)
// 	{
// 		RoboCompVision::MovingImageException ex;
// 		throw ex ;
// 	};
	roiList.resize(PIRNIVELES-1);
	worker->getWholeROIList(cam, roiList, state);
}

void VisionI::getWholePyrAndROIListBothCam( imgType& imgLeft , RoboCompVision::ROIPyrVectors & roiListLeft,  imgType& imgRight , RoboCompVision::ROIPyrVectors & roiListRight,  RoboCompVision::TState & state, const Ice::Current &)
{
	if( worker->isChecking() == true)
	{
		RoboCompVision::ConnectionFailedException con;
		con.what = "VisionComp disconnected from CameraComp.";
		throw   con;
	}
	worker->getWholePyrAndROIListBothCam( imgLeft , roiListLeft, imgRight , roiListRight, state);
}

void VisionI::getWholePyrBothCam( imgType& imgLeft, imgType& imgRight, RoboCompVision::TState & state, const Ice::Current &)
{
	if( worker->isChecking() == true)
	{
		RoboCompVision::ConnectionFailedException con;
		con.what = "VisionComp disconnected from CameraComp.";
		throw   con;
	}
	worker->getWholePyrBothCam( imgLeft, imgRight, state);
}


void VisionI::getROILevelPyr(Ice::Byte cam, Ice::Int level, Ice::Int xc, Ice::Int yc, Ice::Int w, Ice::Int h, imgType & img, RoboCompVision::TState & state, const Ice::Current &)
{
	if( worker->isChecking() ==true)
	{
		RoboCompVision::ConnectionFailedException con;
		con.what = "VisionComp disconnected from CameraComp.";
		throw   con;
	}
	if( cam > worker->paramsVision.numCams-1 )
	{
		RoboCompVision::HardwareFailedException hw;
		QString str = QString("No Camera with number %1.").arg(cam);
		hw.what = str.toStdString();
		throw hw;
	}
// 	if (worker->isMoving()==true)
// 	{
// 		RoboCompVision::MovingImageException ex;
// 		throw ex ;
// 	};
	worker->getROILevelPyr(cam, level, xc, yc, w, h, img , state);
}

void VisionI::getROIWholePyr(Ice::Byte cam, Ice::Int xc, Ice::Int yc, Ice::Int w, Ice::Int h, imgType & img, RoboCompVision::TState & state, const Ice::Current &)
{
	if (worker->isChecking() == true)
	{
		RoboCompVision::ConnectionFailedException con;
		con.what = "VisionComp disconnected from CameraComp.";
		throw   con;
	}
	if (cam > worker->paramsVision.numCams-1)
	{
		RoboCompVision::HardwareFailedException hw;
		QString str = QString("No Camera with number %1.").arg(cam);
		hw.what = str.toStdString();
		throw hw;
	}
// 	if (worker->isMoving() == true)
// 	{
// 		RoboCompVision::MovingImageException ex;
// 		throw ex ;
// 	}
	worker->getROIWholePyr(cam, xc, yc, w, h, img ,state);
}

RoboCompVision::TCamParams VisionI::getCamParams(const Ice::Current &)
{
	return worker->paramsVision;
}


