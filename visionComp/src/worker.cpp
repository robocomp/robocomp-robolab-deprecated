#include "worker.h"

Worker::Worker(const RoboCompCamera::CameraPrx& _camera,  TConfigParams _configParams, QObject *parent) : QObject( parent ) , camera(_camera)
{
	//Read this->params from camera and add local this->params to RoboCompVision::TCamParams
	try
	{
		this->paramsCamera = camera->getCamParams();
		qDebug() << "Reading cameraComp parameters...";
		qDebug() << "Focal: " << this->paramsCamera.focal;
		qDebug() << "Width " <<  this->paramsCamera.width;
		qDebug() << "Height " <<  this->paramsCamera.height;
		qDebug() << "Size " <<  this->paramsCamera.size;
		qDebug() << "NumCams " << this->paramsCamera.numCams;
		qDebug() << "LeftCam " <<  this->paramsCamera.leftCamera;
		qDebug() << "RightCam " <<  this->paramsCamera.rightCamera;
		qDebug() << "BothCams " <<  this->paramsCamera.bothCameras;
		qDebug() << "FPS " <<  this->paramsCamera.FPS;

		this->paramsVision.focal = this->paramsCamera.focal;
		this->paramsVision.width = this->paramsCamera.width;
		this->paramsVision.height = this->paramsCamera.height;
		this->paramsVision.numCams = this->paramsCamera.numCams;
		this->paramsVision.pyrLevels = _configParams.pyramidLevels;
		this->paramsVision.radius = _configParams.radius;
		this->paramsVision.size = this->paramsCamera.size;
		this->paramsVision.leftCamera = this->paramsCamera.leftCamera;
		this->paramsVision.rightCamera = this->paramsCamera.rightCamera;
		this->paramsVision.bothCameras = this->paramsCamera.bothCameras;
		this->paramsVision.FPS = this->paramsCamera.FPS;
		BASIC_PERIOD=(int) rint(1000./((float)this->paramsCamera.FPS));
	}
	catch(const RoboCompCamera::HardwareFailedException& ex)
	{
		qFatal("VisionComp: Hardware exception talking to CameraComp. Aborting...");
	}
	catch(const Ice::Exception& ex)
	{
		qFatal( "VisionComp: Generic exception. Aborting... Check for cameraComp alive!" );
	}

	//Images
	qimgL = new QImage(this->paramsVision.width,this->paramsVision.height,QImage::Format_Indexed8);
	qimgR = new QImage(this->paramsVision.width,this->paramsVision.height,QImage::Format_Indexed8);

	//Harris-Laplace
	Q_ASSERT( this->paramsVision.numCams >0 and this->paramsVision.numCams < 3);
	for( int i = 0; i < this->paramsVision.numCams; ++i )
		ROIDetect[i] = new roiDetector(camera,this->paramsVision,i);

	if(this->paramsVision.numCams>0)
		ROIDetect[0]->getMaxPrismSize(paramsVision.prismWidth, paramsVision.prismHeight);

	//Sift
	//iniGPUSift();

	// Timer
	timer = new QTimer;
	timer->start(BASIC_PERIOD);

	// Mutex
	mutex = new QMutex;
	 
	//Inital state
	ISMOVING = CHECKING = false;
	
	connect(timer, SIGNAL(timeout()), this, SLOT(compute()));

}

Worker::~Worker()
{
	for( int i = 0; i < this->paramsVision.numCams; ++i )
		{
			if(ROIDetect[i] != NULL)
			delete ROIDetect[i];
		}
}

void Worker::compute()
{
	try
	{
		if (this->paramsVision.numCams == 1)
		{
			camera->getYImage(this->paramsVision.leftCamera, imagenV, hState, bState);
			stateNew.hState = hState;
			stateNew.bState = bState;
			stateNew.timeStamp = QTime::currentTime().toString("hh:mm:ss.zzz").toStdString();
			stateNew.timeStampFormat = std::string("hh:mm:ss.zzz");
			ROIDetect[this->paramsVision.leftCamera]->procesa( &imagenV[0], stateNew);
			mutex->lock();
			state=stateNew;
			ROIDetect[this->paramsVision.leftCamera]->swapBuffers();
			mutex->unlock();
		}
 		else if (this->paramsVision.numCams == 2)
 		{
			camera->getYImage(this->paramsVision.bothCameras, imagenV, hState, bState);
			stateNew.hState = hState;
			stateNew.bState = bState;
			stateNew.timeStamp = QTime::currentTime().toString("hh:mm:ss.zzz").toStdString();
			stateNew.timeStampFormat = std::string("hh:mm:ss.zzz");
			ROIDetect[this->paramsVision.leftCamera]->procesa( &imagenV[0], stateNew);
			ROIDetect[this->paramsVision.rightCamera]->procesa( &imagenV[0]+this->paramsVision.size, stateNew);
			mutex->lock();
			state=stateNew;
			ROIDetect[this->paramsVision.leftCamera]->swapBuffers();
			ROIDetect[this->paramsVision.rightCamera]->swapBuffers();	
			mutex->unlock();
		}
		else qDebug() << "VisionComp: wrong number of cameras " << this->paramsVision.numCams;
		printFPS();
		CHECKING = false;
		this->timer->setInterval(BASIC_PERIOD);
	}
	catch (const Ice::Exception& ex)
	{
		qDebug() << "VisionComp: Generic exception talking to CameraComp. ";
		//this->timer->setInterval(CHECK_PERIOD);
		CHECKING = true;
	}

}

unsigned int Worker::indexOfCamera( std::string cam )
{
  if(cam == "LeftCamera") return this->paramsVision.leftCamera;
  else if(cam == "RightCamera") return this->paramsVision.rightCamera;
  else if(cam == "LeftCamera,RightCamera") return 9999;
  else return -1;
}

inline void Worker::printFPS( )
{
	static int fps=0;
	static QTime ti;
	if ((++fps % 50) == 0) {
		qDebug()  << " VisionComp fps: " << (50000 / ti.restart());
	}
}

/////////////////
///Servant
/////////////////

void Worker::getLevelPyr(uchar cam, int level, imgType & img, RoboCompVision::TState & state)
{
	mutex->lock();
		state = this->state;
		ROIDetect[cam]->getLevelPyr(level, img);
	mutex->unlock();
}

void Worker::getWholePyr(uchar cam, imgType & pyr, RoboCompVision::TState & state)
{
	mutex->lock();
		state = this->state;
		ROIDetect[cam]->getWholePyr(pyr);
	mutex->unlock();

}

void Worker::getLevelROIList(uchar cam, int level, RoboCompVision::ROIVector & roiList, RoboCompVision::TState & state)
{
	mutex->lock();
		state = this->state;
		ROIDetect[cam]->getLevelROIList(level, roiList);
	mutex->unlock();	
}


void Worker::getROILevelPyr(uchar cam, int level, int xc, int yc, int w, int h, imgType & img, RoboCompVision::TState & state)
{
	mutex->lock();
		state = this->state;
		ROIDetect[cam]->getROILevelPyr(level, xc, yc, w, h, img);
	mutex->unlock();
}

void Worker::getROIWholePyr(uchar cam, int xc, int yc, int w, int h, imgType & img ,RoboCompVision::TState & state)
{
	mutex->lock();
		state=this->state;
		ROIDetect[cam]->getROIWholePyr(xc, yc, w, h, img);
	mutex->unlock();
}


void Worker::getWholeROIList(uchar cam, RoboCompVision::ROIPyrVectors & roiList ,RoboCompVision::TState & state)
{
	mutex->lock();
		state=this->state;
		ROIDetect[cam]->getWholeROIList(roiList);
	mutex->unlock();
}

void Worker::getWholePyrAndROIListBothCam( imgType& imgLeft , RoboCompVision::ROIPyrVectors & roiListLeft,  imgType& imgRight , RoboCompVision::ROIPyrVectors & roiListRight,  RoboCompVision::TState & state)
{
	mutex->lock();
		state=this->state;
		ROIDetect[this->paramsVision.leftCamera]->getWholePyr(imgLeft);
		ROIDetect[this->paramsVision.leftCamera]->getWholeROIList(roiListLeft);
		ROIDetect[this->paramsVision.rightCamera]->getWholePyr(imgRight);
		ROIDetect[this->paramsVision.rightCamera]->getWholeROIList(roiListRight);
	mutex->unlock();


}

void Worker::getWholePyrBothCam( imgType& imgLeft, imgType& imgRight, RoboCompVision::TState & state)
{
	mutex->lock();
		state=this->state;
		ROIDetect[this->paramsVision.leftCamera]->getWholePyr(imgLeft);
		ROIDetect[this->paramsVision.rightCamera]->getWholePyr(imgRight);
	mutex->unlock();


}

bool Worker::getWholePyrAndROIList( std::string cam, imgType &img , RoboCompVision::ROIPyrVectors &roiList, RoboCompVision::TState &state)
{
  QMutexLocker locker(mutex);
	state=this->state;
	uint index = this->indexOfCamera( cam );
	if (index == -1 ) return false;
	ROIDetect[index]->getWholePyr(img);
	ROIDetect[index]->getWholeROIList(roiList);
	return true;
}


/*
	for(int i  = 0; i < this->num_match; ++i)
  	{
		if( innerModel->fundamental.getDistToEpipolar(QPoint( keysDer[match_buf[i][1]].x , keysDer[match_buf[i][1]].y),
						QPoint(keysIzq[match_buf[i][0]].x , keysIzq[match_buf[i][0]].y)) < 5)
		{
			visorR->drawEllipse( QPoint( keysDer[match_buf[i][1]].x , keysDer[match_buf[i][1]].y), s, Qt::red);
			visorL->drawEllipse( QPoint( keysIzq[match_buf[i][0]].x , keysIzq[match_buf[i][0]].y),  s, Qt::red);
			visorR->drawLine( QLine( keysDer[match_buf[i][1]].x , keysDer[match_buf[i][1]].y , keysIzq[match_buf[i][0]].x - 340 , keysIzq[match_buf[i][0]].y),Qt::yellow);
			visorL->drawLine( QLine( keysIzq[match_buf[i][0]].x , keysIzq[match_buf[i][0]].y , keysDer[match_buf[i][1]].x + 340 , keysDer[match_buf[i][1]].y),Qt::yellow);
			//key1 in the first image matches with key2 in the second image
		}
	}*/
	
	
	// void Worker::GPUSift(unsigned char *imgI, unsigned char *imgD)
// {
// 	static int numDAnt=0, numIAnt = 0;
// 	int numD, numI;
// 
// 	RoboCompVision::ROIPyrVectors list = ROIDetect[0]->getCurrentList();
// 	QVector<SiftGPU::SiftKeypoint> roisI;
// 	LoadKeyPoints( roisI , list);
// 	
// 	this->sift->SetKeypointList( roisI.size(), &roisI[0]);		
// 	if( this->sift->RunSIFT(this->paramsVision.width,this->paramsVision.height, imgI, GL_LUMINANCE,GL_UNSIGNED_BYTE))
// 	{
// 		numI = this->sift->GetFeatureNum();
// 		//qDebug() << "Num sift left " << numI;
// 		this->sift->GetFeatureVector(&keysIzq[0], &descIzq[0]);
// 	}
// 	else
// 		qDebug() << "Worker::GPUSift - Error in GPUSift";
// 	
// 	QVector<SiftGPU::SiftKeypoint> roisD;
// 	ROIDetect[1]->getWholeROIList( list );
// 	LoadKeyPoints( roisD , list );
// 	
// 	this->sift->SetKeypointList( roisD.size(), &roisD[0]);		
// 	if( this->sift->RunSIFT(this->paramsVision.width,this->paramsVision.height, imgD, GL_LUMINANCE,GL_UNSIGNED_BYTE))
// 	{
// 		numD = this->sift->GetFeatureNum();
// 		//get result
// 		this->sift->GetFeatureVector(&keysDer[0], &descDer[0]);
// 	}
// 	else
// 		qDebug() << "Worker::GPUSift - Error in GPUSift";
// 
// 	//Match between cameras
// 	matcher->VerifyContextGL();
// 	matcher->SetDescriptors(0, numI, &descIzq[0]); //image 2
// 	matcher->SetDescriptors(1, numD, &descDer[0]); //image 1
// 	num_match = matcher->GetSiftMatch(numD, match_buf);
// 	
// 	///Copiarlos al TRoi
// 
// //  	float h[3][3];
// //  	innerModel->getFundamental(h);
// 
// // 	matcher->SetFeatureLocation(0, &keysIzq[0]);
// // 	matcher->SetFeatureLocation(1, &keysDer[0]);
// // 	num_match = matcher->GetGuidedSiftMatch(numI, match_buf, NULL, h);
// //
// 
// //	qDebug() << "Num match " << num_match;
// 
// 	//enumerate all the feature matches
// // 	for(int i  = 0; i < num_match; ++i)
// //  	{
// //  		int s = keysDer[match_buf[i][1]].s;
// // 		if (s <30 and s>5 )
// // 		{
// // 			if( innerModel->fundamental.getDistToEpipolar(QPoint( keysDer[match_buf[i][1]].x , keysDer[match_buf[i][1]].y),
// // 						QPoint(keysIzq[match_buf[i][0]].x , keysIzq[match_buf[i][0]].y)) < 5)
// // 			{
// //
// // 				visorR->drawEllipse( QPoint( keysDer[match_buf[i][1]].x , keysDer[match_buf[i][1]].y), s, Qt::red);
// // 				visorL->drawEllipse( QPoint( keysIzq[match_buf[i][0]].x , keysIzq[match_buf[i][0]].y),  s, Qt::red);
// // 				visorR->drawLine( QLine( keysDer[match_buf[i][1]].x , keysDer[match_buf[i][1]].y , keysIzq[match_buf[i][0]].x - 340 , keysIzq[match_buf[i][0]].y),Qt::yellow);
// // 				visorL->drawLine( QLine( keysIzq[match_buf[i][0]].x , keysIzq[match_buf[i][0]].y , keysDer[match_buf[i][1]].x + 340 , keysDer[match_buf[i][1]].y),Qt::yellow);
// // 				//key1 in the first image matches with key2 in the second image
// //  			}
// //  		}
// //  	}
// 
//  	//	Monocular temporal Matching
// //  	matcher->VerifyContextGL();
// // 	matcher->SetDescriptors(0, numD, &descDer[0]); //image 1
// // 	matcher->SetDescriptors(1, numDAnt, &descDerAnt[0]); //image 2
// // 	int numMatchTD = matcher->GetSiftMatch(numD, match_buf);
// // 	for (int i=0; i< numMatchTD; i++)
// // 	{
// //
// // 	}
// // 	matcher->VerifyContextGL();
// // 	matcher->SetDescriptors(0, numI, &descIzq[0]); //image 1
// // 	matcher->SetDescriptors(1, numIAnt, &descIzqAnt[0]); //image 2
// // 	int numMatchTI = matcher->GetSiftMatch(numI, match_buf);
// // 	descDerAnt = descDer;
// // 	descIzqAnt = descIzq;
// // 	numDAnt = numD;
// // 	numIAnt = numI;
// 
// 
// }
