/** \file cenitalComp.cpp
*
* Robolab component model prototype
*
* Robolab 2006
*
**/
// QT includes

#include <QtGui>

// ICE includes
#include <Ice/Ice.h>
#include <Ice/Application.h>

#include <rapplication/rapplication.h>

// View the config.h file for config options like
// enable/disable server, QtGui, etc...
#include "config.h"

#include "worker.h"
#ifdef SERVER
// Interface implementation
#include "cenitalI.h"
#endif

// Includes for remote proxy example
#include <Camera.h>

// User includes here

// Namespaces
using namespace std;
using namespace RoboCompCenital;

class CenitalComp : public RoboComp::Application
{
private:
	// User private data here

	void initialize();

public:
	virtual int run(int, char*[]);
};

void CenitalComp::initialize()
{
	// Config file properties read example
	// configGetString( PROPERTY_NAME_1, property1_holder, PROPERTY_1_DEFAULT_VALUE );
	// configGetInt( PROPERTY_NAME_2, property1_holder, PROPERTY_2_DEFAULT_VALUE );
}

int CenitalComp::run(int argc, char* argv[])
{
#ifdef USE_QTGUI
	QApplication a(argc, argv);  // GUI application
#else
	QCoreApplication a(argc, argv);  // NON-GUI application
#endif
	int status=EXIT_SUCCESS;

	 RoboCompCamera::CameraPrx camera_proxy;
	 string proxy;

	// User variables


	initialize();

	 try
    {
        // Load the remote server proxy
        proxy = communicator()->getProperties()->getProperty( "CameraProxy" );
        cout << "[" << PROGRAM_NAME << "]: Loading [" << proxy << "] proxy at '" << "CameraProxy" << "'..." << endl;
        if( proxy.empty() ) 
        {
            cout << "[" << PROGRAM_NAME << "]: Error loading proxy config!" << endl;
            return EXIT_FAILURE;
        }
    
        camera_proxy = RoboCompCamera::CameraPrx::uncheckedCast( communicator()->stringToProxy( proxy ) );
        if( !camera_proxy )
        {
            cout << "[" << PROGRAM_NAME << "]: Error loading proxy!" << endl;
            return EXIT_FAILURE;
        }
    }

    catch(const Ice::Exception& ex)
    {
        cout << "[" << PROGRAM_NAME << "]: Exception: " << ex;
        return EXIT_FAILURE;
    }	


	Worker *worker = new Worker( camera_proxy);
	try
	{
#ifdef SERVER
		// Server adapter creation and publication
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("CenitalComp");
		CenitalI *cenitalI = new CenitalI( worker );
		adapter->add(cenitalI, communicator()->stringToIdentity("cenital"));
		adapter->activate();

		cout << SERVER_FULL_NAME " started" << endl;
#endif

		// User defined QtGui elements ( main window, dialogs, etc )

#ifdef USE_QTGUI
		// Run QT Application Event Loop
		ignoreInterrupt(); // Ignore the ctrl-c on console
		a.setQuitOnLastWindowClosed( true );
		a.exec();
#else
		// QT EVENT LOOP SIMULATION
		while ( !interrupted() )
		{
			if ( a.hasPendingEvents() )
			{
				a.processEvents();
			}
			else
			{
				// Free the processor
				usleep( 1 );
			}
		}
#endif

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
	CenitalComp app;

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
