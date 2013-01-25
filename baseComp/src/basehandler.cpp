#include "basehandler.h"

BaseHandler::BaseHandler ( QString motion_device, QObject *_parent ) : QObject ( _parent )
{
	printf("BaseHandler()\n");
	// Check if device must be initialized
	if ( motion_device.isEmpty() )
	{
		qFatal ( "BaseHandler::BaseHandler -> Fatal Error in constructor: motion_device should not be empty! Exiting..." );
	}

	// Open and initialize the device
	MotionDevice.setName ( motion_device );
	MotionDevice.setBaudRate ( QSerialPort::BAUD19200);

	if ( MotionDevice.open ( QIODevice::ReadWrite ) == false )
	{
		qWarning (QString( "[10] baseComp: BaseHandler::BaseHandler -> Failed to open: " + motion_device ).toAscii());
		if (motion_device == "/dev/ttyUSB0") motion_device = "/dev/ttyUSB1";
		else if (motion_device != "/dev/ttyUSB1") return;

		motion_device = "/dev/ttyUSB0";
		MotionDevice.setName(motion_device);
		qDebug() << "[10] baseComp: BaseHandler::BaseHandler -> Trying " << motion_device ;
		if (MotionDevice.open ( QIODevice::ReadWrite ) == false)
		{
			qFatal (("[10] baseComp: BaseHandler::BaseHandler -> Fatal error, failed to open: "+motion_device).toAscii());
		}
	}

	qDebug() << "[5] baseComp: BaseHandler::BaseHandler -> Device " << motion_device << " is open";
	qDebug() << "[5] baseComp: BaseHandler::BaseHandler -> Checking that it is a Base microcontroller that can talk to BaseComp (not implemented yet)";



	setMotorsState ( MOTORS_STATE_NORMAL );
	AreMotorsActivated = true;

	//Read base mechanical data
	getBaseData ( mechParams );
	mechParams.encoderSteps = ENCODER_STEPS;  //OJO hay que leerlo de micro

	qWarning ( "BaseHandler::BaseHandler -> Setting the motors state to 'normal operation' ..." );



	time.start();
	cTarget.active=false;

	MotorsStateTimer = new QTimer ( this );
	LastCmdTimestamp = QTime::currentTime();
	MotorsStateTimer->start ( COMMAND_FREE_MOTORS_CHECK_INTERVAL );
	BasePosTimer.start ( BASIC_PERIOD );
	setPosBaseTimer.start ( POSBASE_PERIOD );
	connect ( &setPosBaseTimer, SIGNAL ( timeout() ), this, SLOT ( controlPosBase() ) );

	ADV_GOAL = 0.;
	ROT_GOAL = 0.;
	accelerationControl=true;
	//Init computePosBase class variables
	lant=rant=0;

	qWarning ( "BaseHandler::BaseHandler -> Component up and running ...OK" );

	qDebug() << "fin BaseHandler()";
}

/**
 * Class destructor
 */
BaseHandler::~BaseHandler()
{
	MotorsStateTimer->stop();
	disconnect ( MotorsStateTimer );
	delete MotorsStateTimer;
	stopBase();
	qWarning ( "[BaseHandler]: Setting the motors state to 'free' ..." );
	if ( BaseHandler::setMotorsState ( MOTORS_STATE_FREE ) )
	{
		qWarning ( "[BaseHandler]: Done." );
	}
	else
	{
		qWarning ( "[BaseHandler]: Error setting motors state! Do it manually." );
	}
}

void BaseHandler::initMutex ( QMutex *m )
{
	mutex = m;
	connect ( &BasePosTimer, SIGNAL ( timeout() ), this, SLOT ( computePosBase() ) );
	connect ( MotorsStateTimer, SIGNAL ( timeout() ), this, SLOT ( checkMotorsState() ) );
	stopBase();
	resetOdometer();
}

//Utilities
bool BaseHandler::ascii2dec ( char *buf, int ndigits, int &out )
{
	int i;
	out = 0;
	for ( i=0; i<ndigits; i++ )
	{
		if ( buf[i] < 48 || buf[i] > 57 ) return false;
		out += ( buf[i] - 48 ) * ( int ) pow ( 10,ndigits-1-i );
	}
	return true;
}

/**
 * Generate the command with the specified parameters
 *
 * @param cmd_header Command header
 * @param cmd_value Command value
 * @param cmd_value_padding Command value size (in characters)
 * @param cmd_crc Command redundance
 * @return QString The command generated
 */
QString BaseHandler::generateCommand ( QString cmd_header, int cmd_value, unsigned int cmd_value_padding, QString cmd_crc )
{
	QString value = QString::number ( cmd_value );
	QString command;

	// Command string
	command = cmd_header;

	// Zero padding
	for ( unsigned i = value.length(); i < cmd_value_padding; ++i )
		command += '0';

	// Value string
	command += value;

	// CRC string
	command += cmd_crc;

	return command;
}

/**
 * Send a command to the device
 *
 * @param cmd Command to send
 * @return true if command was sent successfully, else return false
 */
bool BaseHandler::sendCommand(QString cmd, char *buf, int totalread)
{
	char *command;
	QTime time;

	device_mutex.lock();

	cmd += 'X';
	cmd += 'X';
	numRecibido = 0;
	numEnviado = cmd.size();
	command = cmd.toLatin1().data();

	for (int o = 0; o < cmd.size(); ++o) stringEnviado[o] = (uchar)(cmd[o].toAscii());// 	stringEnviado = cmd;


	// Send command


	MotionDevice.write(command,cmd.size());
	// Wait a period between 0.5ms and 250ms for the ack to be read
	numRecibido = MotionDevice.read(buf, totalread);

/*
	printf("traffic: <comand(%d) ", cmd.size());
	for (int o = 0; o < cmd.size(); ++o) printf("%d ", (uchar)(cmd[o].toAscii()));
	printf("> <buf(%d) ", numRecibido);
	for (int o = 0; o < numRecibido; ++o) printf(" %d ", (uchar)(buf[o]));
	printf("read \n");
	printf("%d = MotionDevice.read(buf+%d, %d-%d)", numRecibido, numRecibido, totalread, numRecibido);
*/

	for (int i = 0; i < numRecibido; ++i) stringRecibido[i] = (unsigned char)buf[i];

	if ( numRecibido != totalread )
	{
		printf("[10] baseComp: Ack error: <comand(%d) ", cmd.size());
		device_mutex.unlock();
		return false;
	}
	else
	{
		for (int o=0; o<numRecibido; ++o) stringRecibido[o] = buf[o]; // stringRecibido = buf; /// XXX
		device_mutex.unlock();
		return true;
	}


}

/**
 * Read mechanical data from Robot. Private method for initialization from base microcontroller
 * @return
 */
bool BaseHandler::getBaseData ( TMech &m )
{
	char buf[128];
	int i=0;

	QString cmd = QString ( BASEDATA_GET_COMMAND_STRING BASEDATA_GET_COMMAND_CRC );
	if ( !sendCommand ( cmd, buf, BASEDATA_DATA_SIZE ) ) return false;

	// Read the received data
	if ( buf[i] != 'D' || buf[++i] != '0' || buf[++i] != ':' ) return false;
	i++;
	ascii2dec ( buf+i,BASEDATA_GETGEAR_DATASIZE,m.gearRatio );
	i += BASEDATA_GETGEAR_DATASIZE +1;  // : added
	ascii2dec ( buf+i,BASEDATA_GETRADIO_DATASIZE,m.wheelRadius );
	i += BASEDATA_GETRADIO_DATASIZE +1;  // : added
	ascii2dec ( buf+i,BASEDATA_GETBASELINE_DATASIZE,m.axisLength );


	qDebug();
	qDebug() << "[1] baseComp: READING MECHANICAL PARAMETERS FROM ROBOT";
	qDebug() << "[1] baseComp: Gear Ratio:" << m.gearRatio;
	qDebug() << "[1] baseComp: Wheel Radius:" << m.wheelRadius;
	qDebug() << "[1] baseComp: Axis Length:" << m.axisLength;

	return true;
}


/**
 * Set the base speed in rads/sg to be called in Executor
 *
 * @param adv New base speed en mm/sg
 * @param rot New base rot speed en rads/sg
 *
 */
bool BaseHandler::setSpeedBase ( float adv , float rot )
{
	accelerationControl=true;
	ADV_GOAL = adv;
	ROT_GOAL = rot;
	// setSpeedWheels ( ( ( adv- ( rot*mechParams.axisLength ) /2. ) /mechParams.wheelRadius ), ( ( adv+ ( rot*mechParams.axisLength ) /2. ) /mechParams.wheelRadius ) );

	return true;
}

void BaseHandler::executor()
{
	float adv=0.,rot=0.;

	if ( ( ADV_GOAL - bState.adv ) > MAX_ACCEL_ADV )
		adv = bState.adv + MAX_ACCEL_ADV;
	else
	{
		if ( ( bState.adv - ADV_GOAL ) > MAX_ACCEL_ADV )
			adv = bState.adv - MAX_ACCEL_ADV;
		else
			adv = ADV_GOAL;
	}

	if ( ( ROT_GOAL - bState.rot ) > MAX_ACCEL_ROT )
		rot = bState.rot + MAX_ACCEL_ROT;
	else
	{
		if ( ( bState.rot - ROT_GOAL ) > MAX_ACCEL_ROT )
			rot = bState.rot - MAX_ACCEL_ROT;
		else
			rot = ROT_GOAL;
	}


	if( fabs(bState.adv-ADV_GOAL)>.5 or fabs(bState.rot-ROT_GOAL)>0.01){
		setSpeedWheels ( ( ( adv- ( rot*mechParams.axisLength ) /2. ) /mechParams.wheelRadius ), ( ( adv+ ( rot*mechParams.axisLength ) /2. ) /mechParams.wheelRadius ) );
		//qDebug()<<"acelerando"<<"avance"<<bState.adv<<ADV_GOAL<<"rotacion"<<bState.rot<<ROT_GOAL;

	}
	else{
		setSpeedWheels ( ( ( adv- ( rot*mechParams.axisLength ) /2. ) /mechParams.wheelRadius ), ( ( adv+ ( rot*mechParams.axisLength ) /2. ) /mechParams.wheelRadius ) );
		accelerationControl=false;
	}

}

/**
 * Set wheels speed in rads/sg
 *
 * @param left left wheel speed 3 digits (unidades décimas centésimas sin coma)
 * @param left right wheel speed 3 digits (unidades décimas centésimas sin coma)
 *
 */
bool BaseHandler::setSpeedWheels ( float left, float right )
{
	QString cmd;
	bool status;
	char buf[128]; //dummy

	if ( MotionDevice.isOpen() )
	{
		if ( AreMotorsActivated == false )
		{
			if ( setMotorsState ( MOTORS_STATE_NORMAL ) )
			{
				AreMotorsActivated = true;
			}
			else
			{
				qWarning ( "[10] baseComp: BaseHandler: Warning, motors state could not be changed to 'normal operation'" );
				return false;
			}
		}

		cmd.resize ( 12 );
		cmd[0]='C';
		cmd[1]='1';
		cmd[2]=':';
		if ( left < 0 )
			cmd[3]='-';
		else
			cmd[3] = '+';
		int leftI = abs ( ( int ) rint ( left * 100 ) ) +1000;//1XXX

		QString Lvalue = QString::number ( leftI );
		cmd[4]=Lvalue[1];
		cmd[5]=Lvalue[2];
		cmd[6]=Lvalue[3];
		if ( right < 0 )
			cmd[7]='-';
		else cmd[7] = '+';
		int rightI = abs ( ( int ) rint ( right *100 ) ) +1000;//1XXX
		QString Rvalue = QString::number ( rightI );

		cmd[8]=Rvalue[1];
		cmd[9]=Rvalue[2];
		cmd[10]=Rvalue[3];
		cmd[11]='1';
		cmd[12]='*';

		LastCmdTimestamp.restart();

		status = sendCommand ( cmd , buf, 3 );

		return status;
	}
	return false;
}

/**
 * Set the break motor's state
 *
 * @param state New state ( MOTORS_STATE_NORMAL, MOTORS_STATE_FREE, MOTORS_STATE_BREAK )
 * @return true if command was executed successfully, else return false
 */
bool BaseHandler::setMotorsState ( int state )
{
	QString cmd;
	char buf[128]; //dummy

	cmd = MOTION_MOTORS_STATE_COMMAND_STRING;
	switch ( state )
	{
		case MOTORS_STATE_NORMAL:
			cmd += "00";
			break;//"00"; break;--->cambiar para que empiecen a moverse
		case MOTORS_STATE_FREE:
			cmd += "22";
			break;
		case MOTORS_STATE_BREAK:
			cmd += "11";
			break;
		default:
			return false;
	}
	cmd += MOTION_MOTORS_STATE_COMMAND_CRC;
	return sendCommand ( cmd , buf, 3 );
}

/**
 * Check the motors timeout/state
 *
 */
bool BaseHandler::checkMotorsState()
{
	QMutexLocker lock (mutex);
	if ( bState.isMoving==false && AreMotorsActivated && ( COMMAND_FREE_MOTORS_TIMEOUT > LastCmdTimestamp.elapsed())  )
	{
		if ( setMotorsState ( MOTORS_STATE_FREE ) )
		{
			AreMotorsActivated = false;
			qDebug() << "[0] baseComp: Motors state changed to free!";
			return true;
		}
		else
		{
			qWarning ( "[10] baseComp: [BaseHandler]: Warning, motors state could not be changed to 'free'" );
			return false;
		}
	}
	return false;
}

/**
 * Reset odometer
 *
 * @return true if command was sended successfully, else return false
 */
bool BaseHandler::resetOdometer()
{
	char buf[128];
	QString cmd;
	int pl,pr,i;

	stopBase();
	
	for (i=0; i<5; i++)
	{
		cmd = QString(ODOMETER_RESET_STRING ODOMETER_RESET_CRC);
		if ( sendCommand(cmd, buf, 3))
		{
			getPosWheels(pl, pr);
			if (pl==0 and pr==0) 
			{
				qDebug() << "Odometer reset OK"; 
				lant=rant=0;     //reset computePosBase ant variables to make speeds cero
				MR.init();
				ADV_GOAL = 0.;
				ROT_GOAL = 0.;
				bState.adv=0.;
				bState.rot=0.;
				break;
			}
		}
		else
		{
			qDebug() << "[10] baseComp: Error reseting odometer Try: " << i;
		}
	}
	
	if (i<5) return true;
	else return false;
}

/**
 * Set odometer to a specified state
 *
 */
void BaseHandler::setOdometer ( const RoboCompBase::TBaseState & state )
{
	MR.setRT ( state.x, state.z, state.alfa );
	qDebug() << "[3] baseComp: BaseHandler::setOdometer() -> bState set to: " << state.x << state.z << state.alfa;
}

/**
 * Stops base
 * @return true if sucess
 */
bool BaseHandler::stopBase()
{
	if ( setSpeedBase ( 0. , 0. ) )
	{
		cTarget.active=false;
		cTarget.firstTime=false;
		setPosBaseTimer.stop();
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * Return current base state
 * @return
 */
RoboCompBase::TBaseState BaseHandler::getBaseState()
{
	return bState;
}

/**
 * Read wheels position as measured by encoders
 * @param steps rotated by each wheel since last reset
 * @return true if command was sended successfully, else return false
 */
bool BaseHandler::getPosWheels ( int &izq, int &der )
{
	char buf[128];
	int i=0;
	QString cmd;
	int *aux;
	uchar temp[4];

	cmd = QString ( ODOMETER_GETPOSWHEELS_B_STRING ODOMETER_GETPOSWHEELS_B_CRC );
	if (!sendCommand(cmd, buf, 13))
	{
		return false;
	}
	// Read the received data
	//Check control chars
	if ( buf[i] != 'D' || buf[++i] != '4' || buf[++i] != ':' ) return false;
	i++; //Adelantamos el puntero para que empieze en los datos

	temp[0] = buf[i];
	temp[1] = buf[i+1];
	temp[2] = buf[i+2];
	temp[3] = buf[i+3];
	aux = ( int* ) temp;
	der = *aux;
	temp[0] = buf[i+4];
	temp[1] = buf[i+5];
	temp[2] = buf[i+6];
	temp[3] = buf[i+7];

	aux = ( int* ) temp;
	izq = *aux;

	return true;
}
/**
 * Read wheel speed
 * @param degrees/sg instantaneous speed of each wheel
 * @return true if command was sended successfully, else return false
 */
bool BaseHandler::getSpeedWheels ( float &izq, float &der )
{
	char buf[128];
	int i=0,read=0,iz,de;
	QString cmd;

	cmd = QString ( ODOMETER_GETSPEEDWHEELS_STRING ODOMETER_GETSPEEDWHEELS_CRC );
	if ( !sendCommand ( cmd, buf,read ) ) return false;

	// Read the received data
	//Check control chars
	if ( buf[i] != 'D' || buf[++i] != '3' || buf[++i] != ':' ) return false;

	i++; //Adelantamos el puntero para que empieze en los datos
	ascii2dec ( buf+i,ODOMETER_GETSPEEDWHEELS_DATA_SIZE,iz );
	i += ODOMETER_GETSPEEDWHEELS_DATA_SIZE;
	ascii2dec ( buf+i,ODOMETER_GETSPEEDWHEELS_DATA_SIZE,de );

	//Comprobar que en el final del buffer van los caracteres de control
	izq = iz;
	der= de;
	return true;
}

/**
 * Read base pos from last reset measured in the middle of front axis.
 * @param x position in mm along transversal robot axis since last reset
 * @param y position in mm along longitudinal robot axis since last reset
 * @param alfa degrees rotated counterclockwise since last reset.
 * @return true if command was sended successfully, else return false
 */
bool BaseHandler::getPosBase ( float &x, float &z, float &alfa )
{
	x = bState.x;
	z=bState.z;
	alfa=bState.alfa;
	return true;
}

/**
 * Set wheels position in rads
 * @param izq
 * @param der
 * @return
 */
bool BaseHandler::setPosWheels ( int izq, int der )
{
	//Not implemented
	izq=der=0;
	return false;
}

/**
 * Command to take the Robot to a (x, y) position in world's reference system and ends with alfa orientation
 * The robot turns towards x,y, advances and finally turns alfa to stop with desired orientation.
 * Max advance velocity and rotational velocity is defined by rot and adv.
 * It starts a closed-loop control.
 * @param x mm
 * @param y mm
 * @param alfa in rads
 * @param adv mm/s
 * @return rot rads/sg
 */

///NEED A COMPUTED TRAJECTORY INITIALIZED HERE AND FOLLOWED IN control slot
bool BaseHandler::setPosBase ( int x, int z, float alfa, float adv, float rot, bool angleCorrection )
{
	RMat::QVec localGoal ( 3 );
	localGoal ( 0 ) = ( RMat::T ) x;
	localGoal ( 2 ) = ( RMat::T ) z;
	cTarget.goal = localGoal;
	qDebug() << "[3] baseComp: Setting target:";
	cTarget.goal.print ( "" );
	cTarget.alfaGoal = alfa;
	cTarget.maxAdv = adv;					//stores max rot and adv speed for local mission
	cTarget.maxRot = rot;
	cTarget.angleCorrection = angleCorrection;
	cTarget.distErrorDer = 0.;
	cTarget.distErrorAnt = 0.;
	cTarget.alfaErrorDer = 0.;
	cTarget.alfaErrorAnt = 0.;
	cTarget.active = true;
	cTarget.firstTime=true;
	setPosBaseTimer.start(POSBASE_PERIOD);
	return true;
}


// Max rot speed in rads/sg and adv speed in DE MOMENTO EN RADS/SG.
#define MAX_ADV_SPEED 50.
//300.
#define MIN_ADV_SPEED 4.
#define MAX_ROT_SPEED .2
//Min error threshold
#define MIN_ROT_ERR 0.1
#define MIN_ADV_ERR 10
#define MIN_ADV_ERR2 50

void BaseHandler::stopSetPosBase(){
	setPosBaseTimer.stop();
	cTarget.active=false;
	stopBase();
}

/**
 * Slot for closed-loop aproximation to current target
 */
void BaseHandler::controlPosBase()
{
	float giro, avance,alfaError,alfaFinalError,distError,factor;
	bool mov=true;
	if ( cTarget.active == true )
	{
		
		//Calculamos la distancia euclid. al objetivo como el vector que va desde el robot al objetivo en el SR del robot.
		RMat::QVec localGoal = MR.worldToRobot ( cTarget.goal );
		alfaError = MR.angleTo ( localGoal );
		distError  = sqrt( pow(localGoal(0),2) + pow( localGoal(2),2));

		alfaFinalError = cTarget.alfaGoal -MR.getAlfa();
		qDebug()<<"alfaError"<<alfaError<<"distError"<<distError<<"alfaFinalError"<<alfaFinalError;

		if ( cTarget.firstTime )
		{	//aproximacion al punto
			if ( distError < MIN_ADV_ERR )
			  cTarget.firstTime=false;
			else
			{
				giro = 0.6 * alfaError;
				factor = pow(((M_PI/2. - fabs(alfaError))/ (M_PI/2.) ),6.);
				avance = distError * factor;
			}
		}
		else
		{	//giro en punto final

			if( !cTarget.angleCorrection || fabs(alfaFinalError) < MIN_ROT_ERR )
				mov=false;
			else
			{
				giro = 0.4 * alfaFinalError;
				avance=0.;
			}
		}
		if(mov)
		{
			if (fabs(giro) < 0.01) 
				giro = 0;
			if (fabs(giro) > cTarget.maxRot)
			{
				if(giro>0)
					giro = cTarget.maxRot;
				else
					giro = -cTarget.maxRot;
			}
			if( avance > cTarget.maxAdv) 
				avance = cTarget.maxAdv;
			qDebug() << "[4] baseComp:  giro" << giro << "avance" << avance;
			setSpeedBase ( avance,giro );
		}
		else
		{
			qDebug() << "[5] baseComp: Goal achieved";
			stopBase();
			cTarget.active = false;
			setPosBaseTimer.stop();
		}
	}
}

///Internal slot for computing base position from wheels position

void BaseHandler::computePosBase()
{
	static QTime elapsedTime = QTime::currentTime();
	static float temp = ( M_PI * mechParams.wheelRadius ) / ( mechParams.gearRatio*mechParams.encoderSteps ); //Sacar esto de aquí


	int pl,pr;
	float dl,dr,incA=0.,al=0.;
	static int contError=0;
	float elapsedT;

	bool readError = false;

	if ( getPosWheels ( pl,pr ) == true )
	{
		elapsedT=elapsedTime.elapsed();
		elapsedTime.restart();
		dl = float ( pl-lant );
		dr = float ( pr-rant );
		lant = pl;
		rant = pr;
	
		//Compute increments through robot kinematics
		//Avance en mm's
		incA = ( dl + dr ) *temp ;
		//Rotación realizada en radianes
		al = atan2 ( ( dl - dr ) * 2. * temp, mechParams.axisLength );

		MR.updateRT ( al, incA );

		//Update state struct
		mutex->lock();
// 		bState.adv = incA * 1000. / elapsedT; //mm/sg
// 		bState.rot = al * 1000. / elapsedT;   //rads/sg
		bState.adv = incA;
		bState.rot = al;

		bState.isMoving = fabs(bState.adv) > 0.5 or fabs(bState.rot) > 0.05;
		bState.x = MR.getTr() ( 0,0 );  //TRes[0];
		bState.z = MR.getTr() ( 2,0 );  //TRes[2];
		bState.alfa = MR.getAlfa(); //atan2(RMov[2],RMov[0]);
		if(accelerationControl)
			executor();
		mutex->unlock();

		//elapsedTime.restart();
	}
	else
	{
		qDebug() << "Can't get wheels position";
		readError = true;
	}

	if (readError)
	{
		printf("(%d)  ", MotionDevice.portConfig.c_iflag&ICRNL);
		qDebug() << "ERRORRRRRR: ";
		printf("\tSent     ");
		for (int o = 0; o < numEnviado; ++o) printf(" %d ", stringEnviado[o]);
		printf("\n");
		printf("\tReceived ");
		for (int o = 0; o < numRecibido; ++o) printf(" %d ", stringRecibido[o]);
		printf("\n");
		//Something to log in a logger
		qDebug() << "readError";
	}
}



