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

/****************************************************************************
#     v4l2capture.cpp - V4L2 Image grabber class.                           #
#                                                                           #
#     Copyright (C) 2005 2006 Laurent Pinchart &&  Michel Xhaard            #
#                                                                           #
# This program is free software; you can redistribute it and/or modify      #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation; either version 2 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with this program; if not, write to the Free Software               #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA #
#                                                                           #
****************************************************************************/
#ifndef WORKER_H
#define WORKER_H

#include <QtCore>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <logpolar/lpolar.h>

/* Fixed point arithmetic */
#define FIXED Sint32
#define FIXED_BITS 16
#define TO_FIXED(X) (((Sint32)(X))<<(FIXED_BITS))
#define FROM_FIXED(X) (((Sint32)(X))>>(FIXED_BITS))
#define NB_BUFFER 4
#define DHT_SIZE 432

#ifndef V4L2_CID_BACKLIGHT_COMPENSATION
#define V4L2_CID_BACKLIGHT_COMPENSATION	(V4L2_CID_PRIVATE_BASE+0)
#define V4L2_CID_POWER_LINE_FREQUENCY	(V4L2_CID_PRIVATE_BASE+1)
#define V4L2_CID_SHARPNESS		(V4L2_CID_PRIVATE_BASE+2)
#define V4L2_CID_HUE_AUTO		(V4L2_CID_PRIVATE_BASE+3)
#define V4L2_CID_FOCUS_AUTO		(V4L2_CID_PRIVATE_BASE+4)
#define V4L2_CID_FOCUS_ABSOLUTE		(V4L2_CID_PRIVATE_BASE+5)
#define V4L2_CID_FOCUS_RELATIVE		(V4L2_CID_PRIVATE_BASE+6)
#define V4L2_CID_EXPOSURE_AUTO		(V4L2_CID_PRIVATE_BASE+10)
#define V4L2_CID_EXPOSURE_ABSOLUTE		(V4L2_CID_PRIVATE_BASE+11)


#define V4L2_CID_POWER_LINE_FREQUENCY_DISABLED 0
#define V4L2_CID_POWER_LINE_FREQUENCY_50HZ 1
#define V4L2_CID_POWER_LINE_FREQUENCY_60HZ 2
#endif

#ifndef DEBUG
// #define DEBUG
#endif

#ifdef USE_IPP
#include <ipp.h>
#else
#include <ippWrapper.h>
#endif

#include <qlog/qlog.h>
#include <CameraBus.h>
#include <CommonHead.h>
#include <CommonBehavior.h>
#include <JointMotor.h>
#include <DifferentialRobot.h>

#define MAX_CAMERAS 5
#define MAX_WIDTH 800
#define MAX_HEIGHT 600


using namespace RoboCompCameraBus;


struct vdIn {
    int fd;
    char *videodevice;
    char *status;
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers rb;
    void *mem[NB_BUFFER];
    unsigned char *framebuffer;
    int isstreaming;
    int grabmethod;
    int width;
    int height;
    int fps;
    int formatIn;
    int formatOut;
    int framesizeIn;
};

class Worker : public QThread
{
Q_OBJECT
public:
	Worker(RoboCompJointMotor::JointMotorPrx head_, RoboCompDifferentialRobot::DifferentialRobotPrx base_);
	~Worker();
	bool init( );
	
//CameraBus
	void getImage(const string& camera, const RoboCompCameraBus::Format& format, RoboCompCameraBus::Image& image);
	void getSyncImages(const RoboCompCameraBus::CameraList& cameraList, const RoboCompCameraBus::Format& format, bool all,RoboCompCameraBus::ImageList& imagelist);
	void getBaseState (RoboCompDifferentialRobot::TBaseState &bState_);
	void getHeadState (RoboCompCommonHead::THeadState &hState_);
//CommonBehavior
	void setPeriod(int period);
	int getPeriod();
	bool setParams(RoboCompCommonBehavior::ParameterList _params);
	void killYourSelf();
	
signals:
	void kill();
	
private:
	bool uvcGrab();
	void run();  //Thread
	void resizeImage(const RoboCompCameraBus::Format &sformat,uchar *source[],const RoboCompCameraBus::Format &dformat,uchar *destination[],int n=1);
	void YUVtoRGB(int width,int height,uchar *source[],uchar *destination[],int n=1);
	void YUVtoGray (int width,int height,uchar *source[], uchar *destination[],int n=1);
	void YUVtoRGBPtr(int width,int height,uchar *source[],uchar *destination[],int n=1);
	void mirror(const RoboCompCameraBus::Format &format,uchar *image,bool horizontal,bool vertical);
	void transformImages(int n,const RoboCompCameraBus::Format& format,RoboCompCameraBus::Image &image);
	
	/**
	 *    Utility class for printing current frame rate.
	 */
	inline void printFPS( )
	{
		static int fps=0;
		static QTime ti(0,0,0);
		static QTime tt=QTime::currentTime();
		static QTime foo(0,0,0);
		if ((++fps % 50) == 0)
		{
			uint32_t e=ti.restart();
			if (e)
			{
				int v = 50000 / e;
				qDebug() << "CamaraComp Fps: " << v << "Elapsed:" << foo.addMSecs( tt.elapsed()).toString("hh:mm:ss:zzz");
			}
		}
	}	
public:
	RoboCompCameraBus::BusParams busparams;		 ///*<params structure holding camera bus params
	RoboCompCameraBus::CameraParamsList cameraParamsList;
	RoboCompCameraBus::CameraList cameraList;
	RoboCompCameraBus::Format cameraFormat;
private:
	vdIn vd[MAX_CAMERAS];
	int Period;
	QMutex *mu;                                    ///*< mu mutex for double buffer management
	bool _finished;                                ///*< _finished for sending the thread to sleep when no work
	bool sleeped;
	QTime reloj;                                   ///*< Reloj for timeStamps
	std::string timeStamp;                         ///*< timeStamp for timeStamping the images
	
	
	RoboCompCameraBus::FormatNotAvailable ex;
	RoboCompDifferentialRobot::DifferentialRobotPrx base;                  ///*<base proxy to BaseComp
	RoboCompDifferentialRobot::TBaseState  bState,bStateAfter, bStateBefore;             ///*<bState structure maintained by BaseComp holding base instantaneous kinematics
	RoboCompJointMotor::JointMotorPrx head;        ///*<head  proxy to JointMotorComp
	RoboCompJointMotor::MotorStateMap map;
	RoboCompJointMotor::MotorList motorList;	 ///*<params structure maintained by JointMotor to hold a list of motor names
	RoboCompCommonHead::THeadState hState,hStateAfter, hStateBefore;        ///*<hState structure maintained by JointMotor holding head instantaneous kinematics
	RoboCompCommonHead::THeadState motorStateMap;  ///*<params structure maintained by JointMotor to return a list of motor states
	
	
	uchar *imgBufferGrabA[MAX_CAMERAS];
	uchar *imgBufferGrabB[MAX_CAMERAS];
	uchar *imgBufferGrabPtr[MAX_CAMERAS];
	uchar *imgBufferSour[MAX_CAMERAS];
	uchar *imgBufferTransformA[MAX_CAMERAS];
	uchar *imgBufferTransformB[MAX_CAMERAS];
	uchar *pSour[MAX_CAMERAS];
	uchar *pDes[MAX_CAMERAS];
	uchar *buf;
};

#endif
