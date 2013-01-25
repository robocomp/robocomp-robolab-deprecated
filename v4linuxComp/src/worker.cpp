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

#include "worker.h"

int isv4l2Control(struct vdIn *vd, int control, struct v4l2_queryctrl *queryctrl);
int enum_frame_intervals(int dev, __u32 pixfmt, __u32 width, __u32 height);
int enum_frame_sizes(int dev, __u32 pixfmt);

Worker::Worker(RoboCompJointMotor::JointMotorPrx head_, RoboCompDifferentialRobot::DifferentialRobotPrx base_)
{
#ifdef USE_IPP
	ippSetNumThreads(1);
#endif
	
	buf = (uchar*)malloc(MAX_WIDTH*MAX_HEIGHT*3);

	head=head_;
	base=base_;
	_finished=false;
	mu = new QMutex(QMutex::Recursive);
}


Worker::~Worker()
{
	for (int i=0; i<MAX_CAMERAS; ++i)
	{
		if ( vd[i].isstreaming )
		{
			int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			ioctl ( vd[i].fd, VIDIOC_STREAMOFF, &type );
			vd[i].isstreaming = 0;
		}

		free(vd[i].framebuffer);
		vd[i].framebuffer = NULL;
		free(vd[i].videodevice);
		free(vd[i].status);
		vd[i].videodevice = NULL;
		vd[i].status = NULL;
	}
}

///Common Behavior
void Worker::killYourSelf()
{
	rDebug("Killing myself");
	emit kill();
	exit(1);
}
/**
* \brief Change compute period
* @param per Period in ms
*/
void Worker::setPeriod(int p)
{
	rDebug("Period changed"+QString::number(p));
	Period = p;
}

bool Worker::setParams(RoboCompCommonBehavior::ParameterList _params)
{
	busparams.mode = RoboCompCameraBus::YUV422Mode;
	busparams.numCameras = QString::fromStdString(_params["V4linux.NumCameras"].value).toInt();
	///check numcameras menor que MAX_CAMERAS
	if( busparams.numCameras < 1 or busparams.numCameras > MAX_CAMERAS)
		return false;
	busparams.FPS = QString::fromStdString(_params["V4linux.FPS"].value).toInt();
	busparams.width = QString::fromStdString(_params["V4linux.Width"].value).toInt();
	busparams.height = QString::fromStdString(_params["V4linux.Height"].value).toInt();
	busparams.talkToBase = QString::fromStdString(_params["V4linux.TalkToBase"].value).contains("true");;
	busparams.talkToCommonHead = QString::fromStdString(_params["V4linux.TalkToCommonHead"].value).contains("true");
	QStringList deviceList = QString::fromStdString(_params["V4linux.Device"].value).split(",", QString::SkipEmptyParts);

	cameraFormat.width = busparams.width;
	cameraFormat.height = busparams.height;
	cameraFormat.modeImage = RoboCompCameraBus::YUV422;
	cameraFormat.size = cameraFormat.width * cameraFormat.height;
	
	RoboCompCameraBus::CameraParams camera;
	cameraList.resize(busparams.numCameras);
	cameraParamsList.resize(busparams.numCameras);
	busparams.devices.resize(busparams.numCameras);
	for (int i=0; i<busparams.numCameras; i++)
	{
		busparams.devices[i] = deviceList.at(i).toStdString();
		std::string s = QString::number(i).toStdString();
		camera.name = _params["V4linux.Params_" + s +".name"].value; 
		camera.busId = QString::fromStdString(_params["V4linux.Params_"+ s +".busId"].value).toShort();
		camera.invertedH = QString::fromStdString(_params["V4linux.Params_"+ s +".invertedH"].value).contains("true");
		camera.invertedV = QString::fromStdString(_params["V4linux.Params_"+ s +".invertedV"].value).contains("true");
		camera.focalX = QString::fromStdString(_params["V4linux.Params_"+ s +".focalX"].value).toInt();
		camera.focalY = QString::fromStdString(_params["V4linux.Params_"+ s +".focalY"].value).toInt();
		camera.saturation = QString::fromStdString(_params["V4linux.Params_"+ s +".saturation"].value).toInt();
		camera.lineFreq = QString::fromStdString(_params["V4linux.Params_"+ s +".linefreq"].value).toInt();
		cameraList[i] = camera.name;
		cameraParamsList[i] = camera;
	}
	//Reserve space for buffering
	for(int i=0;i<busparams.numCameras;i++)
	{
		imgBufferGrabA[i] = (uchar*)malloc(MAX_WIDTH*MAX_HEIGHT*3);
		imgBufferGrabB[i] = (uchar*)malloc(MAX_WIDTH*MAX_HEIGHT*3);
		imgBufferTransformA[i] = (uchar*)malloc(MAX_WIDTH*MAX_HEIGHT*3);
		imgBufferTransformB[i] = (uchar*)malloc(MAX_WIDTH*MAX_HEIGHT*3);
		imgBufferGrabPtr[i] = imgBufferTransformB[i];
	}
	this->start();
	return true;
}


bool Worker::init()
{
	rDebug("Initiating V4l2Capture...");
	
	/// We shold be checking params before!!!
	if (busparams.devices.size() == 0 ) qFatal ( "V4L2Capture::init() fatal error: device is NULL" );
	if (busparams.width == 0 || busparams.height == 0 )
	{
		qFatal ("V4L2Capture::init() fatal error: Frame size is 0");
		return -1;
	}
	int grabmethod = 1; // mmap
	int format = V4L2_PIX_FMT_YUYV;
	
	for (int i=0; i<busparams.numCameras; ++i)
	{
		qDebug() << "Configuring camera:" << i;
		memset(&vd[i], 0, sizeof(struct vdIn));

		vd[i].videodevice = (char *)calloc(busparams.devices[i].size()+1, sizeof(char));
		vd[i].status = (char *)calloc(1, 100*sizeof(char));
		//Esto debería hacerse con una expresión regular
		snprintf(vd[i].videodevice, 12, "%s", busparams.devices[i].c_str());
		printf ( "V4L2Capture::init() info: current video device %s \n", vd[i].videodevice );
		vd[i].width = busparams.width;
		vd[i].height = busparams.height;
		vd[i].formatIn = format;
		vd[i].grabmethod = grabmethod;
		vd[i].fps = busparams.FPS;

		if ((vd[i].fd = open(vd[i].videodevice, O_RDWR)) == -1 )
			qFatal ( "V4L2Capture::init() fatal error: Opening V4L interface" );

		memset(&vd[i].cap, 0, sizeof(struct v4l2_capability));

		if (ioctl(vd[i].fd, VIDIOC_QUERYCAP, &vd[i].cap) < 0)
			qFatal ( "V4L2Capture::init() fatal error: Unable to query device %s ", vd[i].videodevice );

		if ((vd[i].cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)==0)
			qFatal ( "V4L2Capture::init() fatal error: video capture not supported %s ", vd[i].videodevice );

		if (vd[i].grabmethod)
		{
			if (!(vd[i].cap.capabilities & V4L2_CAP_STREAMING))
				qFatal ( "V4L2Capture::init() fatal error: %s does not support streaming", vd[i].videodevice );
		}
		else
		{
			if (!(vd[i].cap.capabilities & V4L2_CAP_READWRITE) )
				qFatal ( "V4L2Capture::init() fatal error: %s does not support read i/o ", vd[i].videodevice );
		}

		struct v4l2_fmtdesc fmt;
		memset(&fmt, 0, sizeof(fmt));
		fmt.index = 0;
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		while (ioctl(vd[i].fd, VIDIOC_ENUM_FMT, &fmt) == 0)
		{
			fmt.index++;
			printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",fmt.pixelformat & 0xFF, (fmt.pixelformat >> 8) & 0xFF, 	(fmt.pixelformat >> 16) & 0xFF, (fmt.pixelformat >> 24) & 0xFF,	fmt.description);
			if (enum_frame_sizes(vd[i].fd, fmt.pixelformat) != 0)
				printf("  Unable to enumerate frame sizes.\n");
		}

		// set format in
		memset (&vd[i].fmt, 0, sizeof(struct v4l2_format));
		vd[i].fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vd[i].fmt.fmt.pix.width = vd[i].width;
		vd[i].fmt.fmt.pix.height = vd[i].height;
		vd[i].fmt.fmt.pix.pixelformat = vd[i].formatIn;
		vd[i].fmt.fmt.pix.field = V4L2_FIELD_ANY;

		if (ioctl(vd[i].fd, VIDIOC_S_FMT, &vd[i].fmt) < 0)
			qFatal ( "V4L2Capture::init() fatal error: Unable to set format through VIDIOC_S_FMT" );

		if ( (vd[i].fmt.fmt.pix.width!=(uint)vd[i].width) || (vd[i].fmt.fmt.pix.height!=(uint)vd[i].height) )
			qFatal ( "V4L2Capture::init() fatal error: Size %dx%d is not available. Suggested %dx%d", vd[i].width, vd[i].height, vd[i].fmt.fmt.pix.width, vd[i].fmt.fmt.pix.height);

		/* set framerate */
//		struct v4l2_streamparm* setfps;
//		setfps=(struct v4l2_streamparm *) calloc(1, sizeof(struct v4l2_streamparm));
//		memset(setfps, 0, sizeof(struct v4l2_streamparm));
//		setfps->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//		setfps->parm.capture.timeperframe.numerator=1;
//		setfps->parm.capture.timeperframe.denominator=30;
//		if( ioctl(vd[i].fd, VIDIOC_S_PARM, setfps) < 0 )
//				qFatal ( "V4L2Capture::init() fatal error: Unable to set frame rate through VIDIOC_S_PARM" );

		// request buffers
		memset(&vd[i].rb, 0, sizeof(struct v4l2_requestbuffers));
		vd[i].rb.count = NB_BUFFER;
		vd[i].rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vd[i].rb.memory = V4L2_MEMORY_MMAP;
		if (ioctl(vd[i].fd, VIDIOC_REQBUFS, &vd[i].rb) < 0)
			qFatal ( "V4L2Capture::init() fatal error: Unable to allocate buffers through VIDIOC_REQBUFS" );

		// map the buffers
		for (int ii=0; ii<NB_BUFFER; ii++)
		{
			memset(&vd[i].buf, 0, sizeof(struct v4l2_buffer));
			vd[i].buf.index = ii;
			vd[i].buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			vd[i].buf.memory = V4L2_MEMORY_MMAP;
			if (ioctl(vd[i].fd, VIDIOC_QUERYBUF, &vd[i].buf ) < 0)
				qFatal ( "V4L2Capture::init() fatal error: Unable to query buffer through VIDIOC_QUERYBUF. Error number %d ", errno );
			vd[i].mem[ii] = mmap(0, vd[i].buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, vd[i].fd, vd[i].buf.m.offset );
			if (vd[i].mem[ii] == MAP_FAILED)
				qFatal ( "V4L2Capture::init() fatal error: Unable to map the buffer through mmap. Error number: %d ",errno );
		}

		// Queue the buffers.
		for (int ii = 0; ii < NB_BUFFER; ++ii)
		{
			memset(&vd[i].buf, 0, sizeof(struct v4l2_buffer));
			vd[i].buf.index = ii;
			vd[i].buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			vd[i].buf.memory = V4L2_MEMORY_MMAP;

			if (ioctl(vd[i].fd, VIDIOC_QBUF, &vd[i].buf) < 0)
				qFatal ( "V4L2Capture::init() fatal error: Unable to queue buffers through VIDIOC_QBUF. Error number: %d",errno );
		}

		// alloc a temp buffer to reconstruct the pict
		vd[i].framesizeIn = ( vd[i].width * vd[i].height << 1 );
		switch ( vd[i].formatIn )
		{
		case V4L2_PIX_FMT_MJPEG:
				qFatal("MJPEG not supported");
// 			if ((vd[i].tmpbuffer=(unsigned char *)calloc(1, (size_t)vd[i].framesizeIn)) == 0)
// 				qFatal ( "V4L2Capture::init() fatal error: not enough memory to allocate vd[i].tmpbuffer" );
// 			vd[i].framebuffer = (unsigned char *)calloc(1, (size_t)vd[i].width*(vd[i].height + 8)*2);
			break;
		case V4L2_PIX_FMT_YUYV:
			vd[i].framebuffer = (unsigned char *)calloc(1, ( size_t ) vd[i].framesizeIn);
			break;
		case V4L2_PIX_FMT_GREY:
			vd[i].framebuffer = (unsigned char *)calloc(1, ( size_t ) vd[i].framesizeIn);
			break;
		default:
			printf("V4LCapture::init() V4L2 error: Format not recognized!!\n");
			return false;
			break;
		}

		if (vd[i].framebuffer == 0)
			qFatal ( "V4L2Capture::init() fatal error: not enough memory to allocate vd[i].framebuffer" );

		int control;
		struct v4l2_control control_s;
		struct v4l2_queryctrl queryctrl;
		int err;

		// Set the power line frequency value
		control = V4L2_CID_POWER_LINE_FREQUENCY;
		control_s.id = control;
		if (isv4l2Control(&vd[i], control, &queryctrl) < 0) return false;
		if (cameraParamsList[i].lineFreq == 60) control_s.value =  V4L2_CID_POWER_LINE_FREQUENCY_60HZ;
		else control_s.value =  V4L2_CID_POWER_LINE_FREQUENCY_50HZ;
		if ((err = ioctl(vd[i].fd, VIDIOC_S_CTRL, &control_s)) < 0)
			qFatal("V4LCapture::init() V4L2 error: ioctl set_light_frequency_filter error\n");
		printf("CameraComp: Power line frequency: %dHz\n", cameraParamsList[i].lineFreq);

		// Set the saturation value
		control = V4L2_CID_SATURATION;
		control_s.id = control;
		if (isv4l2Control(&vd[i], control, &queryctrl) < 0) return false;
		control_s.value = cameraParamsList[i].saturation;
		if ((err = ioctl(vd[i].fd, VIDIOC_S_CTRL, &control_s)) < 0)
			qFatal("V4LCapture::init() V4L2 error: ioctl set_saturation error\n");
		printf("CameraComp: Saturation: %d\n", cameraParamsList[i].saturation);


	// Set the auto exposure off for max speed
		control = V4L2_CID_EXPOSURE_AUTO;
		control_s.id = control;
//		if (isv4l2Control(&vd[i], control, &queryctrl) < 0) return false;
//		control_s.value = 8;
// 		if ((err = ioctl(vd[i].fd, VIDIOC_S_CTRL, &control_s)) < 0)
// 			qFatal("V4LCapture::init() V4L2 error: ioctl set_exposureOff error\n");
// 		printf("CameraComp: AutoExposure: %d\n", 1);
//
// 	// Set the auto exposure off for max speed
 //		control = V4L2_CID_EXPOSURE_ABSOLUTE;
 //		control_s.id = control;
 //		if (isv4l2Control(&vd[i], control, &queryctrl) < 0) return false;
 //		control_s.value = 200;
 //		if ((err = ioctl(vd[i].fd, VIDIOC_S_CTRL, &control_s)) < 0)
 //			qFatal("V4LCapture::init() V4L2 error: ioctl set_exposureOff error\n");
 //		printf("CameraComp: AutoExposure: %d\n", 900);

	}
	return true;
}


#define HEADERFRAME1 0xaf
bool Worker::uvcGrab( ){
	static bool bufferA=true;
	
	if (busparams.talkToCommonHead==true){
		try{
			head->getAllMotorState(map);
			hStateBefore.motorsState = map;
			hStateBefore.isMoving = map["neck"].isMoving or map["tilt"].isMoving or map["leftPan"].isMoving or map["rightPan"].isMoving;
		}
		catch (const Ice::Exception& ex){
			rDebug("Error reading CommonHead state");
			return false;
		}
	}
	if (busparams.talkToBase==true){
		try{
			base->getBaseState (bStateBefore);
		}
		catch (const Ice::Exception& ex)
		{
			rDebug("Error reading Base state");
			return false;
		}
	}
	
	for (int i=0; i<busparams.numCameras; ++i)
	{
		if (vd[i].isstreaming == false)
		{
			int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			if (ioctl(vd[i].fd, VIDIOC_STREAMON, &type) < 0)
				qFatal ( "Worker fatal error: Unable to start capture using VIDIOC_STREAMON: Error number: %d", errno );
			else
				vd[i].isstreaming = 1;
		}

		memset(&vd[i].buf, 0, sizeof(struct v4l2_buffer));
		vd[i].buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vd[i].buf.memory = V4L2_MEMORY_MMAP;

		if (ioctl(vd[i].fd, VIDIOC_DQBUF, &vd[i].buf) < 0)
			qFatal("Worker fatal error: Unable to dequeue buffer using VIDIOC_DQBUF: Error number: %d", errno);

// 		mu->lock();
			if (vd[i].buf.bytesused > (uint)vd[i].framesizeIn)
				memcpy(imgBufferGrabPtr[i], vd[i].mem[vd[i].buf.index], (size_t) vd[i].framesizeIn);
			else
				memcpy(imgBufferGrabPtr[i], vd[i].mem[vd[i].buf.index], (size_t) vd[i].buf.bytesused);
// 		mu->unlock();

		if (ioctl(vd[i].fd, VIDIOC_QBUF, &vd[i].buf) < 0)
			qFatal ( "Worker fatal error: Unable to requeue buffer using VIDIOC_QBUF: Error number: %d", errno );
	}
	//* Get kinematic information from base and head to attach to frame
	if (busparams.talkToCommonHead==true)
	{
		try
		{
			head->getAllMotorState(map);
			hStateAfter.motorsState = map;
			hStateAfter.isMoving = map["neck"].isMoving or map["tilt"].isMoving or map["leftPan"].isMoving or map["rightPan"].isMoving;
		}
		catch (const Ice::Exception& ex)
		{
			rDebug("Error reading JointMotor state");
			return false;
		}
	}
	if (busparams.talkToBase==true)
	{
		try{
			base->getBaseState (bStateAfter);
		}
		catch (const Ice::Exception& ex)
		{
			rDebug("Error reading Base state");
			return false;
		}
	}
	mu->lock();
		bState=bStateAfter;
		hState=hStateAfter;
		for(RoboCompCommonHead::dmotorsState::const_iterator it = hStateAfter.motorsState.begin(); it != hStateAfter.motorsState.end(); ++it)
		{
			if (it->second.pos != hStateBefore.motorsState[it->first].pos)
				hState.motorsState[it->first].isMoving=true;
		}
		for(int i=0;i<busparams.numCameras;i++)
		{
			if(bufferA)
			{
				imgBufferGrabPtr[i] = imgBufferGrabA[i];
				imgBufferSour[i] = imgBufferGrabB[i];
			}
			else
			{
				imgBufferGrabPtr[i] = imgBufferGrabB[i];
				imgBufferSour[i] = imgBufferGrabA[i];
			}
		}
		bufferA = !bufferA;
	mu->unlock();
	return true;
}


/**
* \brief Thread method
*/
void Worker::run()
{
	init();
	while (!_finished)
	{
		if( uvcGrab() == false)
			qFatal ( "Wroker:run() -> Fatal error: uvcgrab() returned error" );
		printFPS();
	}
}

///
///Accessors for public interface
///
void Worker::getSyncImages(const RoboCompCameraBus::CameraList& _cameraList, const RoboCompCameraBus::Format& format, bool all,RoboCompCameraBus::ImageList& imagelist)
{
	int n=0;
	if(all)
	{
		mu->lock();
			for(int i=0;i<busparams.numCameras;i++)
			{
				memcpy (imgBufferTransformA[i], imgBufferSour[i] ,(size_t) vd[i].framesizeIn );
				pSour[i] = imgBufferTransformA[i];
				pDes[i] = imgBufferTransformB[i];
			}
			n = busparams.numCameras;
		mu->unlock();
	}
	else
	{
		uint i = 0;
		int j=0;
		while(i < _cameraList.size())
		{
			j = 0;
			while(_cameraList[i] != cameraList[j] and j < busparams.numCameras )
				j++;
			if(_cameraList[i] == cameraList[j])
			{
				mu->lock();
					memcpy (imgBufferTransformA[i], imgBufferSour[i] ,(size_t) vd[j].framesizeIn );
					pSour[i] = imgBufferTransformA[i];
					pDes[i] = imgBufferTransformB[i];
				mu->unlock();
			}
			else
			{
				qWarning("No existe esa camara");
				ex.what = std::string("Exception: CameraBus:: camera:")+_cameraList[j]+std::string(" not found");
				throw ex;
				return;
			}
		}
		n = _cameraList.size();
	}
	
	RoboCompCameraBus::Image image;
	transformImages(n,format,image);
	imagelist.resize(n);
	for(int i=0;i<n;i++)
	{
		imagelist[i].frmt = image.frmt;
		imagelist[i].timeStamp = image.timeStamp;
		imagelist[i].data.resize(image.frmt.size);
		memcpy(&imagelist[i].data[0],pSour[i],image.frmt.size);
	}
}

void Worker::getImage(const string& camera, const RoboCompCameraBus::Format& format, RoboCompCameraBus::Image& image)
{
	int i = 0;
	while(cameraList[i] != camera and i < busparams.numCameras-1)
		i++;
	
	if(cameraList[i] != camera)
	{
		qWarning("No existe esa camara");
		ex.what = std::string("Exception: CameraBus:: camera:")+camera+std::string(" not found");
		throw ex;
		return;
	}
	
	mu->lock();
		memcpy (imgBufferTransformA[0], imgBufferSour[i] ,(size_t) vd[i].framesizeIn );
		pSour[0] = imgBufferTransformA[0];
		pDes[0] = imgBufferTransformB[0];
	mu->unlock();
	
	transformImages(1,format,image);
	image.data.resize(image.frmt.size);
	memcpy(&image.data[0],pSour[0],image.frmt.size);
}
/**
 * \brief Return Base position 
 * @return TBaseState Struct contains base position compute by odometry
*/
void Worker::getBaseState(RoboCompDifferentialRobot::TBaseState &bState_)
{
	mu->lock();
		bState_ = bState;
	mu->unlock();	
}
/**
 * \brief Return Head position 
 * @return THeadState Struct contains head position
*/
void Worker::getHeadState (RoboCompCommonHead::THeadState &hState_)
{
	mu->lock();
		hState_ = hState;
	mu->unlock();
}

///
///Private methods
///
void Worker::transformImages(int n,const RoboCompCameraBus::Format& format,RoboCompCameraBus::Image &image)
{
	uint8_t *temp;
	RoboCompCameraBus::Format ftemp;
	ftemp.width = busparams.width;
	ftemp.height = busparams.height;
	ftemp.modeImage = format.modeImage;
	if(format.modeImage != RoboCompCameraBus::YUV422)
	{
		if(format.modeImage == RoboCompCameraBus::GRAY8)
			YUVtoGray(ftemp.width,ftemp.height,pSour,pDes,n);
		else
		{
			if(format.modeImage == RoboCompCameraBus::RGB888Packet)
				YUVtoRGB(ftemp.width,ftemp.height, pSour, pDes,n);
			else
			{
				if(format.modeImage == RoboCompCameraBus::RGB888Planar)
					YUVtoRGBPtr(ftemp.width,ftemp.height,pSour,pDes,n);
			}
		}
		for(int i=0;i<n;i++)
		{
			temp = pSour[i];
			pSour[i] = pDes[i];
			pDes[i] = temp;
		}
	}
	if (ftemp.width != format.width or ftemp.height != format.height)
	{
		resizeImage(ftemp,pSour,format,pDes,n);
		for(int i=0;i<n;i++)
		{
			temp = pSour[i];
			pSour[i] = pDes[i];
			pDes[i] = temp;
		}
	}
	for(int i=0;i<n;i++)	
	{
		if(cameraParamsList[i].invertedH != false or cameraParamsList[i].invertedV != false)
			mirror(format,pSour[i],cameraParamsList[i].invertedH,cameraParamsList[i].invertedV);
	}
	
	int size = format.width*format.height;
	if (format.modeImage == RoboCompCameraBus::RGB888Packet or format.modeImage == RoboCompCameraBus::RGB888Planar)
		size *= 3;
	if (format.modeImage == RoboCompCameraBus::YUV422)
		size *= 2;

	image.timeStamp = reloj.elapsed();
	image.frmt = format;
	image.frmt.size = size;
}




void Worker::resizeImage(const RoboCompCameraBus::Format &sformat,uchar *source[],const RoboCompCameraBus::Format &dformat,uchar *destination[],int n)
{
	
	if(sformat.height == dformat.height and sformat.width == dformat.width )
	{
		for(int i=0;i<n;i++)
			destination[i] = source[i];
	}
	else
	{
		IppiSize srcSize = {sformat.width,sformat.height};
		IppiRect srcRoi = {0,0,sformat.width,sformat.height};
		IppiRect dstRoi = {0,0,dformat.width,dformat.height};
		if(dformat.modeImage == RoboCompCameraBus::GRAY8)
		{
			for(int i=0;i<n;i++)
				ippiResizeSqrPixel_8u_C1R(source[i],srcSize,srcSize.width,srcRoi,destination[i],dformat.width,dstRoi,(float)dformat.width/(float)sformat.width,(float)dformat.height/(float)sformat.height,0,0,IPPI_INTER_LINEAR,buf);	
		}
		else
		{
			if(dformat.modeImage == RoboCompCameraBus::RGB888Packet or dformat.modeImage == RoboCompCameraBus::RGB888Planar)
				for(int i=0;i<n;i++)
					ippiResizeSqrPixel_8u_C3R(source[i],srcSize,sformat.width*3,srcRoi,destination[i],dformat.width*3,dstRoi,(float)dformat.width/(float)sformat.width,(float)dformat.height/(float)sformat.height,0,0,IPPI_INTER_LINEAR,buf);
			else
			{
				IppiSize dstSize = {dformat.width,dformat.height };
 				if(dformat.modeImage == RoboCompCameraBus::YUV422)
 					for(int i=0;i<n;i++)
 						ippiResizeYUV422_8u_C2R(source[i],srcSize,sformat.width*2,srcRoi,destination[i],dformat.width*2,dstSize,(float)dformat.width/(float)sformat.width,(float)dformat.height/(float)sformat.height,IPPI_INTER_LINEAR);
			}
		}
	}
}

void Worker::YUVtoRGB(int width,int height,uchar *source[],uchar *destination[],int n)
{
	IppiSize imgSize = {width,height};
	for(int i=0;i<n;i++)
		ippiYUV422ToRGB_8u_C2C3R (source[i], width*2, destination[i], width*3 ,imgSize);
}

void Worker::YUVtoGray (int width,int height,uchar *source[],uchar *destination[],int n)
{
	uchar *s,*d;
	for(int i=0;i<n;i++)
	{
		s = source[i];
		d = destination[i];
		for (int j=0; j<width*height; j++)
			d[j] = s[j*2];
	}
}
void Worker::YUVtoRGBPtr(int width,int height,uchar *source[],uchar *destination[],int n)
{
	IppiSize isize = {width,height};
	int size = width*height;
	Ipp8u *dest,*sour;
	for(int i=0;i<n;i++)
	{
		Ipp8u *planos[3] = {destination[i]+size, destination[i]+ ( size*2 ), destination[i] + ( 3*size ) };
		dest = destination[i];
		sour = source[i];
		for (int j=0; j<size; j++)
			dest[j] = sour[j*2];
		ippiYUV422ToRGB_8u_C2C3R(source[i], width*2, buf, width*3, isize);
		ippiCopy_8u_C3P3R(buf, width*3, planos, width, isize);
	}
}


void Worker::mirror(const RoboCompCameraBus::Format &format,uchar *image,bool horizontal,bool vertical)
{
	IppiAxis flip;
	IppiSize roiSize = {format.width,format.height};
	if(horizontal and vertical)
		flip = ippAxsBoth;
	else
	{
		if(vertical)
			flip = ippAxsVertical;
		if(horizontal)
			flip = ippAxsHorizontal;
	}
	
	if(format.modeImage == RoboCompCameraBus::GRAY8)
		ippiMirror_8u_C1IR(image, format.width, roiSize, flip);
	else
	{
		if(format.modeImage == RoboCompCameraBus::RGB888Packet or format.modeImage == RoboCompCameraBus::RGB888Planar)
			ippiMirror_8u_C3IR(image, format.width*3, roiSize, flip);
		else
		{
			ex.what = std::string("Exception: CameraBus:: YUV422 mirroring not supported");
			throw ex;
		}
	}
}




///auxiliary
/* return >= 0 ok otherwhise -1 */
int isv4l2Control(struct vdIn *vd, int control, struct v4l2_queryctrl *queryctrl)
{
	int err =0;
	queryctrl->id = control;
	err = ioctl(vd->fd, VIDIOC_QUERYCTRL, queryctrl);
	if (err < 0)
	{
		perror("ioctl querycontrol error");
	}
	else if (queryctrl->flags & V4L2_CTRL_FLAG_DISABLED)
	{
		printf("control %s disabled\n", (char *) queryctrl->name);
	}
	else if (queryctrl->flags & V4L2_CTRL_TYPE_BOOLEAN)
	{
		return 1;
	}
	else if (queryctrl->type & V4L2_CTRL_TYPE_INTEGER)
	{
		return 0;
	}
	else {
		printf("contol %s unsupported\n", (char *) queryctrl->name);
		return 0;
	}
	return -1;
}

int enum_frame_intervals(int dev, __u32 pixfmt, __u32 width, __u32 height)
{
	int ret;
	struct v4l2_frmivalenum fival;

	memset(&fival, 0, sizeof(fival));
	fival.index = 0;
	fival.pixel_format = pixfmt;
	fival.width = width;
	fival.height = height;
	printf("\tTime interval between frame: ");
	while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMEINTERVALS, &fival)) == 0) {
		if (fival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
				printf("%u/%u, ",
						fival.discrete.numerator, fival.discrete.denominator);
		} else if (fival.type == V4L2_FRMIVAL_TYPE_CONTINUOUS) {
				printf("{min { %u/%u } .. max { %u/%u } }, ",
						fival.stepwise.min.numerator, fival.stepwise.min.numerator,
						fival.stepwise.max.denominator, fival.stepwise.max.denominator);
				break;
		} else if (fival.type == V4L2_FRMIVAL_TYPE_STEPWISE) {
				printf("{min { %u/%u } .. max { %u/%u } / "
						"stepsize { %u/%u } }, ",
						fival.stepwise.min.numerator, fival.stepwise.min.denominator,
						fival.stepwise.max.numerator, fival.stepwise.max.denominator,
						fival.stepwise.step.numerator, fival.stepwise.step.denominator);
				break;
		}
		fival.index++;
	}
	printf("\n");
	if (ret != 0 && errno != EINVAL) {
		printf("ERROR enumerating frame intervals: %d\n", errno);
		return errno;
	}

	return 0;
}
int enum_frame_sizes(int dev, __u32 pixfmt)
{
	int ret;
	struct v4l2_frmsizeenum fsize;

	memset(&fsize, 0, sizeof(fsize));
	fsize.index = 0;
	fsize.pixel_format = pixfmt;
	while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMESIZES, &fsize)) == 0) {
		if (fsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
			printf("{ discrete: width = %u, height = %u }\n",
					fsize.discrete.width, fsize.discrete.height);
			ret = enum_frame_intervals(dev, pixfmt,
					fsize.discrete.width, fsize.discrete.height);
			if (ret != 0)
				printf("  Unable to enumerate frame sizes.\n");
		} else if (fsize.type == V4L2_FRMSIZE_TYPE_CONTINUOUS) {
			printf("{ continuous: min { width = %u, height = %u } .. "
					"max { width = %u, height = %u } }\n",
					fsize.stepwise.min_width, fsize.stepwise.min_height,
					fsize.stepwise.max_width, fsize.stepwise.max_height);
			printf("  Refusing to enumerate frame intervals.\n");
			break;
		} else if (fsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
			printf("{ stepwise: min { width = %u, height = %u } .. "
					"max { width = %u, height = %u } / "
					"stepsize { width = %u, height = %u } }\n",
					fsize.stepwise.min_width, fsize.stepwise.min_height,
					fsize.stepwise.max_width, fsize.stepwise.max_height,
					fsize.stepwise.step_width, fsize.stepwise.step_height);
			printf("  Refusing to enumerate frame intervals.\n");
			break;
		}
		fsize.index++;
	}
	if (ret != 0 && errno != EINVAL) {
		printf("ERROR enumerating frame sizes: %d\n", errno);
		return errno;
	}
	return 0;
}

