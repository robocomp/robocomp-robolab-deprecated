/****************************************************************************
#     basehandler.h - Definition of BaseHandler class.                      #
#     Copyright (C) 2008 Robolab-UEx                                        #
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

#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <math.h>
#include <ipp.h>
#include <QtCore>
#include <Ice/Ice.h>
#include <Base.h>


#include <q4serialport/q4serialport.h>

// Timeouts
#define COMMAND_RESPONSE_TIMEOUT    100
#define COMMAND_FREE_MOTORS_CHECK_INTERVAL  3000
#define COMMAND_FREE_MOTORS_TIMEOUT         5000

// Number of command digits for padding
#define COMMAND_SINGLE_VALUE_PADDING 3
#define COMMAND_DOUBLE_VALUE_PADDING 6

// Set Base Speed commands: adv (mm/s) rot(rads/seg)  C0:SAaasBbb:0*
#define MOTION_BASE_COMMAND_STRING "C0:"
#define MOTION_BASE_COMMAND_CRC    "0*"
// Set Right Wheel Speed: Motor motion commands
#define MOTION_RIGHT_COMMAND_STRING "A1:"
#define MOTION_RIGHT_COMMAND_CRC    "1*"
// Set Left Wheel Speed: Motor motion commands
#define MOTION_LEFT_COMMAND_STRING "B1:"
#define MOTION_LEFT_COMMAND_CRC    "1*"
// Set Both Wheels Speed: Motor motion commands C1:SAaasBbb:1*
#define MOTION_COMMAND_STRING "C1:"
#define MOTION_COMMAND_CRC    "1*"
// Set motors state command
#define MOTION_MOTORS_STATE_COMMAND_STRING  "C9:"
#define MOTION_MOTORS_STATE_COMMAND_CRC     "9*"
// Motors States
#define MOTORS_STATE_NORMAL 0
#define MOTORS_STATE_FREE   1
#define MOTORS_STATE_BREAK  2
// Get Base data command: Reductora, Radio, Separación entre ruedas
#define BASEDATA_GET_COMMAND_STRING "D0:"
#define BASEDATA_GET_COMMAND_CRC    "0*"
#define BASEDATA_DATA_SIZE   3+2+1+2+1+3+2
#define BASEDATA_GETGEAR_DATASIZE 2
#define BASEDATA_GETRADIO_DATASIZE 2
#define BASEDATA_GETBASELINE_DATASIZE 3
// Odometer commands
#define ODOMETER_RESET_STRING "D1:"
#define ODOMETER_RESET_CRC "1*"
#define ODOMETER_GETPOSWHEELS_STRING "D2:"
#define ODOMETER_GETPOSWHEELS_CRC "2*"
// 3 bytes per parameter
#define ODOMETER_GETPOSWHEELS_DATA_SIZE   10 
#define ODOMETER_GETSPEEDWHEELS_STRING "D3:"
#define ODOMETER_GETSPEEDWHEELS_CRC "1*"
#define ODOMETER_GETSPEEDWHEELS_DATA_SIZE   3
#define ODOMETER_GETPOSBASE_STRING "E1:"
#define ODOMETER_GETPOSBASE_CRC "1*"
#define ODOMETER_GETPOSBASE_DATA_SIZE  10 
// Odometer commands Version binario
#define ODOMETER_GETPOSWHEELS_B_STRING "D4:"
#define ODOMETER_GETPOSWHEELS_B_CRC "4*"
#define ODOMETER_GETPOSWHEELS_B_DATA_SIZE   4

//Encoder steps per turn HP
#define ENCODER_STEPS 2000

//Timer state update period
#define BASIC_PERIOD 100
#define BASIC_FREQ  (1000./BASIC_PERIOD) 
#define POSBASE_PERIOD 100

//Sholud be read from config
//Max acc in mm/sg
#define MAX_ACCEL_ADV 60.
//Max acc in rads/sg
#define MAX_ACCEL_ROT .2

#include <QString> /// XXX

#include <QMat/QMatAll>
#include <QMat/qvec.h>
#include <QMat/qmovingrobot.h>

using namespace RMat;
using namespace std;

//! Handler for the mobile robot Robex. This object interacts with the Robex-AVR, a microcontroller based device that controls the motors of the robots using PID loops
/**
	@author SyNKRo <synkro1982@gmail.com>
*/

class BaseHandler : public QObject
{
Q_OBJECT
public: /// XXX
	unsigned char stringEnviado[200], stringRecibido[200];
	int numEnviado, numRecibido;
public:
	BaseHandler( QString, QObject *_parent = NULL );
	~BaseHandler();


	QMutex *mutex;
	void initMutex(QMutex *m);
//	bool TARGET_ACTIVE;
	bool setMotorsState(int);
	bool stopBase();
	RoboCompBase::TBaseState getBaseState();
	bool getSpeedBase(float &, float &) {return false; }; //Motors speeds in radians/sg
	bool setSpeedBase(float adv, float rot);
	bool getPosBase(float &x, float &z, float &alfa);
	bool setPosBase(int x, int y, float alfa, float adv, float rot, bool angleCorrection);
	bool getPosWheels(int &izq, int &der);
	bool setPosWheels(int izq, int der);
	bool getSpeedWheels(float &izq, float &der);
	bool setSpeedWheels(float izq, float der);
	// Odometer related slots
	bool resetOdometer();
	void setOdometer( const RoboCompBase::TBaseState &state );
	void stopSetPosBase();

private:

	//Structure that holds mechanical parameters
	struct TMech
	{
		int wheelRadius;
		int axisLength;
		int encoderSteps;
		int gearRatio;
	};
	TMech mechParams;

	struct TGoal
	{
		RMat::QVec goal;
		float alfaGoal;	
		float finalAng;
		float maxAdv;
		float maxRot;
		bool active;
		bool angleCorrection;
		float alfaError;
		float alfaErrorDer;
		float alfaErrorAnt;
		float distError;
		float distErrorDer;
		float distErrorAnt;
		bool firstTime;
	};

	QTimer BasePosTimer;
	QTimer BaseControlTimer;
	QTimer ExecutorTimer;
	QSerialPort MotionDevice;
	QTimer *MotorsStateTimer;                           // Timer for set motors state
	QTimer setPosBaseTimer;
	QTime LastCmdTimestamp;                             // Timestamp of last command received
	bool AreMotorsActivated;
// 	bool AreMotorsStopped;
	QMutex device_mutex;
	float distAngular(float a,float b);
	float PiTo2Pi(float);
	QString generateCommand( QString, int, unsigned int, QString );
	bool sendCommand( QString , char*, int);
	bool ascii2dec(char*, int, int&);	
	QTime time;
	RoboCompBase::TBaseState bState;	
	TGoal cTarget;
	bool getBaseData( TMech &);
	bool accelerationControl;
	MovingRobot MR;
	float ADV_GOAL, ROT_GOAL;                           //Reference speed for timer activated executor
	int lant,rant;                                      //t-1 values of wheels pos used in computePosBase. ResetOdometer zeroes them		

protected slots:
	void computePosBase();
	void controlPosBase();
	void executor();

public slots:
	bool checkMotorsState();

};

#endif
