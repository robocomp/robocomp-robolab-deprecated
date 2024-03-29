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
/** \mainpage RoboComp::genericComp
 *
 * \section intro_sec Introduction
 *
 * The genericComp component...
 *
 * \section interface_sec Interface
 *
 * genericComp interface...
 *
 * \section install_sec Installation
 *
 * \subsection install1_ssec Software depencences
 * genericComp ...
 *
 * \subsection install2_ssec Compile and install
 * cd genericComp
 * <br>
 * cmake . && make
 * <br>
 * To install:
 * <br>
 * sudo make install
 *
 * \section guide_sec User guide
 *
 * \subsection config_ssec Configuration file
 *
 * <p>
 * The configuration file genericComp/etc/config ...
 * </p>
 *
 * \subsection execution_ssec Execution
 *
 * Just: "${PATH_TO_BINARY}/genericComp --Ice.Config=${PATH_TO_CONFIG_FILE}"
 *
 * \subsection running_ssec Once running
 *
 * ...
 *
 */
// QT includes
#include <QtCore>
#include <QtGui>

// ICE includes
#include <Ice/Ice.h>

#include <Ice/Application.h>

#include <rapplication/rapplication.h>
#include <qlog/qlog.h>
// View the config.h file for config options like
// QtGui, etc...
#include "config.h"
#include "genericmonitor.h"
#include "genericworker.h"
#include "specificworker.h"
#include "specificmonitor.h"
#include "commonbehaviorI.h"
#include <dialogmanagerI.h>

// Includes for remote proxy example
// #include <Remote.h>
#include <ui_guiDlg.h>
#include <Speech.h>
#include <Tracker.h>
#include <Gesture.h>
#include <ASR.h>
#include <FaceDetector.h>
#include <JointMotor.h>


// User includes here

// Namespaces
using namespace std;
using namespace RoboCompCommonBehavior;
using namespace RoboCompDialogManager;
using namespace RoboCompSpeech;
using namespace RoboCompTracker;
using namespace RoboCompGesture;
using namespace RoboCompASR;
using namespace RoboCompFaceDetector;
using namespace RoboCompJointMotor;


class dialogManagerComp : public RoboComp::Application
{
private:
	// User private data here

	void initialize();
	MapPrx mprx;

public:
	virtual int run(int, char*[]);
};

void dialogManagerComp::initialize()
{
	// Config file properties read example
	// configGetString( PROPERTY_NAME_1, property1_holder, PROPERTY_1_DEFAULT_VALUE );
	// configGetInt( PROPERTY_NAME_2, property1_holder, PROPERTY_2_DEFAULT_VALUE );
}
int dialogManagerComp::run(int argc, char* argv[])
{
#ifdef USE_QTGUI
	QApplication a(argc, argv);  // GUI application
#else
	QCoreApplication a(argc, argv);  // NON-GUI application
#endif
	int status=EXIT_SUCCESS;

	// Remote server proxy access example
	// RemoteComponentPrx remotecomponent_proxy;
	SpeechPrx speech_proxy;
TrackerPrx tracker_proxy;
GesturePrx gesture_proxy;
ASRPrx asr_proxy;
FaceDetectorPrx facedetector_proxy;
JointMotorPrx jointmotor_proxy;


	string proxy;

	// User variables


	initialize();

	// Remote server proxy creation example
	// try
	// {
	// 	// Load the remote server proxy
	//	proxy = getProxyString("RemoteProxy");
	//	remotecomponent_proxy = RemotePrx::uncheckedCast( communicator()->stringToProxy( proxy ) );
	//	if( !remotecomponent_proxy )
	//	{
	//		rInfo(QString("Error loading proxy!"));
	//		return EXIT_FAILURE;
	//	}
	//catch(const Ice::Exception& ex)
	//{
	//	cout << "[" << PROGRAM_NAME << "]: Exception: " << ex << endl;
	//	return EXIT_FAILURE;
	//}
	//rInfo("RemoteProxy initialized Ok!");
	// 	// Now you can use remote server proxy (remotecomponent_proxy) as local object
	
 	try
	{
		// Load the remote server proxy
		proxy = communicator()->getProperties()->getProperty("SpeechProxy");
		cout << "[" << PROGRAM_NAME << "]: Loading [" << proxy << "] proxy at (" << "SpeechProxy" << ")..." << endl;
		if( proxy.empty() )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy config! Check config file for missing of incorrect proxies" << endl;
			return EXIT_FAILURE;
		}
		speech_proxy = SpeechPrx::uncheckedCast( communicator()->stringToProxy( proxy ) );
		if( !speech_proxy )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy!" << endl;
			return EXIT_FAILURE;		
		}
	}
	catch(const Ice::Exception& ex)
	{
		cout << "[" << PROGRAM_NAME << "]: Exception: " << ex << endl;
		return EXIT_FAILURE;
	}
	cout << "SpeechProxy initialized Ok!" << endl;
	mprx["SpeechProxy"] = (::IceProxy::Ice::Object*)(&speech_proxy);
 	try
	{
		// Load the remote server proxy
		proxy = communicator()->getProperties()->getProperty("TrackerProxy");
		cout << "[" << PROGRAM_NAME << "]: Loading [" << proxy << "] proxy at (" << "TrackerProxy" << ")..." << endl;
		if( proxy.empty() )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy config! Check config file for missing of incorrect proxies" << endl;
			return EXIT_FAILURE;
		}
		tracker_proxy = TrackerPrx::uncheckedCast( communicator()->stringToProxy( proxy ) );
		if( !tracker_proxy )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy!" << endl;
			return EXIT_FAILURE;		
		}
	}
	catch(const Ice::Exception& ex)
	{
		cout << "[" << PROGRAM_NAME << "]: Exception: " << ex << endl;
		return EXIT_FAILURE;
	}
	cout << "TrackerProxy initialized Ok!" << endl;
	mprx["TrackerProxy"] = (::IceProxy::Ice::Object*)(&tracker_proxy);
 	try
	{
		// Load the remote server proxy
		proxy = communicator()->getProperties()->getProperty("GestureProxy");
		cout << "[" << PROGRAM_NAME << "]: Loading [" << proxy << "] proxy at (" << "GestureProxy" << ")..." << endl;
		if( proxy.empty() )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy config! Check config file for missing of incorrect proxies" << endl;
			return EXIT_FAILURE;
		}
		gesture_proxy = GesturePrx::uncheckedCast( communicator()->stringToProxy( proxy ) );
		if( !gesture_proxy )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy!" << endl;
			return EXIT_FAILURE;		
		}
	}
	catch(const Ice::Exception& ex)
	{
		cout << "[" << PROGRAM_NAME << "]: Exception: " << ex << endl;
		return EXIT_FAILURE;
	}
	cout << "GestureProxy initialized Ok!" << endl;
	mprx["GestureProxy"] = (::IceProxy::Ice::Object*)(&gesture_proxy);
 	try
	{
		// Load the remote server proxy
		proxy = communicator()->getProperties()->getProperty("ASRProxy");
		cout << "[" << PROGRAM_NAME << "]: Loading [" << proxy << "] proxy at (" << "ASRProxy" << ")..." << endl;
		if( proxy.empty() )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy config! Check config file for missing of incorrect proxies" << endl;
			return EXIT_FAILURE;
		}
		asr_proxy = ASRPrx::uncheckedCast( communicator()->stringToProxy( proxy ) );
		if( !asr_proxy )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy!" << endl;
			return EXIT_FAILURE;		
		}
	}
	catch(const Ice::Exception& ex)
	{
		cout << "[" << PROGRAM_NAME << "]: Exception: " << ex << endl;
		return EXIT_FAILURE;
	}
	cout << "ASRProxy initialized Ok!" << endl;
	mprx["ASRProxy"] = (::IceProxy::Ice::Object*)(&asr_proxy);
 	try
	{
		// Load the remote server proxy
		proxy = communicator()->getProperties()->getProperty("FaceDetectorProxy");
		cout << "[" << PROGRAM_NAME << "]: Loading [" << proxy << "] proxy at (" << "FaceDetectorProxy" << ")..." << endl;
		if( proxy.empty() )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy config! Check config file for missing of incorrect proxies" << endl;
			return EXIT_FAILURE;
		}
		facedetector_proxy = FaceDetectorPrx::uncheckedCast( communicator()->stringToProxy( proxy ) );
		if( !facedetector_proxy )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy!" << endl;
			return EXIT_FAILURE;		
		}
	}
	catch(const Ice::Exception& ex)
	{
		cout << "[" << PROGRAM_NAME << "]: Exception: " << ex << endl;
		return EXIT_FAILURE;
	}
	cout << "FaceDetectorProxy initialized Ok!" << endl;
	mprx["FaceDetectorProxy"] = (::IceProxy::Ice::Object*)(&facedetector_proxy);
 	try
	{
		// Load the remote server proxy
		proxy = communicator()->getProperties()->getProperty("JointMotorProxy");
		cout << "[" << PROGRAM_NAME << "]: Loading [" << proxy << "] proxy at (" << "JointMotorProxy" << ")..." << endl;
		if( proxy.empty() )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy config! Check config file for missing of incorrect proxies" << endl;
			return EXIT_FAILURE;
		}
		jointmotor_proxy = JointMotorPrx::uncheckedCast( communicator()->stringToProxy( proxy ) );
		if( !jointmotor_proxy )
		{
			cout << "[" << PROGRAM_NAME << "]: Error loading proxy!" << endl;
			return EXIT_FAILURE;		
		}
	}
	catch(const Ice::Exception& ex)
	{
		cout << "[" << PROGRAM_NAME << "]: Exception: " << ex << endl;
		return EXIT_FAILURE;
	}
	cout << "JointMotorProxy initialized Ok!" << endl;
	mprx["JointMotorProxy"] = (::IceProxy::Ice::Object*)(&jointmotor_proxy);
	
	GenericWorker *worker = new SpecificWorker(mprx);
	//Monitor thread
	GenericMonitor *monitor = new SpecificMonitor(worker,communicator());
	QObject::connect(monitor,SIGNAL(kill()),&a,SLOT(quit()));
	QObject::connect(worker,SIGNAL(kill()),&a,SLOT(quit()));
	monitor->start();
	
	if ( !monitor->isRunning() )
		return status;
	try
	{
		// Server adapter creation and publication
		Ice::ObjectAdapterPtr adapterCommonBehavior = communicator()->createObjectAdapter("CommonBehavior");
		CommonBehaviorI *commonbehaviorI = new CommonBehaviorI(monitor );
		adapterCommonBehavior->add(commonbehaviorI, communicator()->stringToIdentity("commonbehavior"));
		adapterCommonBehavior->activate();
		// Server adapter creation and publication
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("dialogManagerComp");
		DialogManagerI *dialogmanager = new DialogManagerI(worker);
		adapter->add(dialogmanager, communicator()->stringToIdentity("dialogmanager"));
		
		adapter->activate();

		cout << SERVER_FULL_NAME " started" << endl;

		// User defined QtGui elements ( main window, dialogs, etc )

#ifdef USE_QTGUI
		//ignoreInterrupt(); // Uncomment if you want the component to ignore console SIGINT signal (ctrl+c).
		a.setQuitOnLastWindowClosed( true );
#endif
		// Run QT Application Event Loop
		a.exec();
		status = EXIT_SUCCESS;
	}
	catch(const Ice::Exception& ex)
	{
		status = EXIT_FAILURE;

		cout << "[" << PROGRAM_NAME << "]: Exception raised on main thread: " << endl;
		cout << ex;

#ifdef USE_QTGUI
		a.quit();
#endif
	}

	return status;
}

int main(int argc, char* argv[])
{
	bool hasConfig = false;
	string arg;
	dialogManagerComp app;

	// Search in argument list for --Ice.Config= argument
	for (int i = 1; i < argc; ++i)
	{
		arg = argv[i];
		if ( arg.find ( "--Ice.Config=", 0 ) != string::npos )
			hasConfig = true;
	}

	if ( hasConfig )
		return app.main( argc, argv );
	else
		return app.main(argc, argv, "config"); // "config" is the default config file name
}