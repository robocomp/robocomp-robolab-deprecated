
#include "cameraDlgControl.h"

/**
* @brief Default constructor
*/
CameraDlgControl::CameraDlgControl(RoboCompCamera::CameraPrx _camera_prx)
{
//	show();
	camera_prx = _camera_prx;
	
	
	
	///Read camParams from CamaraComp
	
	try
	{	
		camParams = _camera_prx->getCamParams();
		QImage::Format format = QImage::Format_RGB888; // RGB packed
		imgSize = camParams.size * 3;
		if(camParams.mode == "MODE_640x480_MONO" or camParams.mode == "MODE_640x480_GREY" or camParams.mode == "MODE_320x240_MONO" or camParams.mode == "MODE_320x240_GREY")
		{
			format = QImage::Format_Indexed8;
			imgSize = camParams.size;
		}
		QHBoxLayout *hLayout = new QHBoxLayout();
		imgI = new QImage(camParams.width,camParams.height,format);
		frameI = new QFrame(this);
		hLayout->addWidget(frameI);
		frameI->setFixedSize(camParams.width,camParams.height);
		qvisorI = new qWorld(camParams.width,camParams.height,imgI,frameI);
		if(camParams.numCams >= 2)
		{
			imgD = new QImage(camParams.width,camParams.height,format);
			frameD = new QFrame(this);
			hLayout->addWidget(frameD);
			frameD->setFixedSize(camParams.width,camParams.height);
 			qvisorD = new qWorld(camParams.width,camParams.height,imgD,frameD);
		
		}
		//Add radio button
		rbOnoff = new QRadioButton("On/Off",this);
		QSpacerItem *hSpader = new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Fixed);
		QHBoxLayout *hLayout_2 = new QHBoxLayout();
		hLayout_2->addItem(hSpader);
		hLayout_2->addWidget(rbOnoff);
		QVBoxLayout *vLayout = new QVBoxLayout(this);
		vLayout->addItem(hLayout_2);
		vLayout->addItem(hLayout);
		
		qDebug()<<"siz"<<imgSize;
		
	      showMessage( "BasicMonitorComp - Camara parameters read ok");
	      showMessage( QString("	Focal: %1").arg(camParams.focal));
	      //showMessage( QString("	Focal: %1").arg(camParams.mode));
// 	      showMessage( QString("	Width: %1").arg(camParams.width));
// 	      showMessage( QString("	Height: %1").arg(camParams.height));
// 	      showMessage( QString("	Size: %1").arg(camParams.size));
	      showMessage( QString("	Num Cams: %1").arg(camParams.numCams));
// 	      showMessage( QString("	Left Camera code: %1").arg(camParams.leftCamera));
// 	      showMessage( QString("	Right Camera code: %1").arg(camParams.rightCamera));
// 	      showMessage( QString("	Both Cameras code: %1").arg(camParams.bothCameras));



	}
	catch(const Ice::Exception& ex)
	{
	      std::cout << ex;
	      qWarning( "	BasicMonitorCompComp: No CameraComp alive.  Continue..." );
	}
 
}
/**
* @brief Default destructor
*/
CameraDlgControl::~CameraDlgControl()
{

}

void CameraDlgControl::update()
{
	if(rbOnoff->isChecked()){
		try{
			camera_prx->getRGBPackedImage(0, imgV, hState,bState);
			memcpy(imgI->bits(), &imgV[0], imgSize);
			if(camParams.numCams >= 2)
			{
				camera_prx->getRGBPackedImage(1 ,imgV, hState,bState);
				memcpy(imgD->bits(), &imgV[0], imgSize);
				qvisorD->drawCrossHair(Qt::blue);
				qvisorD->update();
			}
			  
		}
		catch(const Ice::Exception& ex){
			showMessage("Camera:Get image failure");
		}
	}
	qvisorI->drawCrossHair(Qt::blue);
	qvisorI->update();
}






void CameraDlgControl::showMessage(const QString &message){
	emit outputMessage(message);
	qDebug()<<"mensaje"<<message;
}


