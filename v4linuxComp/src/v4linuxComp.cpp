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
/** \mainpage RoboComp::v4linuxComp
 *
 * \section intro_sec Introduction
 *
 * The v4linuxComp component...
 *
 * \section interface_sec Interface
 *
 * v4linuxComp interface...
 *
 * \section install_sec Installation
 *
 * \subsection install1_ssec Software depencences
 * v4linuxComp ...
 *
 * \subsection install2_ssec Compile and install
 * cd v4linuxComp
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
 * The configuration file v4linuxComp/etc/config ...
 * </p>
 *
 * \subsection execution_ssec Execution
 *
 * Just: "${PATH_TO_BINARY}/v4linuxComp --Ice.Config=${PATH_TO_CONFIG_FILE}"
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
#include "monitor.h"
#include "worker.h"
#include "commonbehaviorI.h"
#include "camerabusI.h"



// Includes for remote proxy example
// #include <Remote.h>


// User includes here

// Namespaces
using namespace std;
using namespace RoboCompCommonBehavior;
using namespace RoboCompCameraBus;



class v4linuxComp : public RoboComp::Application
{
private:
	// User private data here

	void initialize();

public:
	virtual int run(int, char*[]);
};

void v4linuxComp::initialize()
{
	// Config file properties read example
	// configGetString( PROPERTY_NAME_1, property1_holder, PROPERTY_1_DEFAULT_VALUE );
	// configGetInt( PROPERTY_NAME_2, property1_holder, PROPERTY_2_DEFAULT_VALUE );
}

int v4linuxComp::run(int argc, char* argv[])
{
#ifdef USE_QTGUI
	QApplication a(argc, argv);  // GUI application
#else
	QCoreApplication a(argc, argv);  // NON-GUI application
#endif
	int status=EXIT_SUCCESS;

	// Remote server proxy access example
	// RemoteComponentPrx remotecomponent_proxy;
	RoboCompJointMotor::JointMotorPrx head_proxy;
	RoboCompDifferentialRobot::DifferentialRobotPrx base_proxy;

	string proxy;

	// User variables


	initialize();

	// Remote server proxy creation example
	try
	{
		// Load the remote server proxy
		head_proxy = RoboCompJointMotor::JointMotorPrx::uncheckedCast( communicator()->stringToProxy( getProxyString("JointMotorProxy") ) );
		rInfo("JointMotorProxy initialized Ok!");
	}
	catch(const Ice::Exception& ex)
	{
		cout << "[" << PROGRAM_NAME << "]: Exception: " << ex << endl;
		return EXIT_FAILURE;
	}
	
	try{
		// Load the remote server proxy
		base_proxy = RoboCompDifferentialRobot::DifferentialRobotPrx::uncheckedCast( communicator()->stringToProxy( getProxyString("DifferentialRobotProxy") ) );
		rInfo("DifferentialRobotProxy initialized Ok!");
	}
	catch(const Ice::Exception& ex)
	{
		cout << "[" << PROGRAM_NAME << "]: Exception: " << ex << endl;
		return EXIT_FAILURE;
	}
	
	
	
	// 	// Now you can use remote server proxy (remotecomponent_proxy) as local object



	Worker *worker = new Worker(head_proxy,base_proxy);
	//Monitor thread
	Monitor *monitor = new Monitor(worker,communicator());
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
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("CameraBus");
		CameraBusI *camerabusI = new CameraBusI(worker );
		adapter->add(camerabusI, communicator()->stringToIdentity("camerabus"));

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
	v4linuxComp app;

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
