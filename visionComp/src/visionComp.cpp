//////////////////////////////////////////////////////
// Robolab component model prototype
//
//
// Robolab 2006
//////////////////////////////////////////////////////

#include <QtCore>
#include <QtGui>

#include <Ice/Ice.h>
#include <Ice/Application.h>
#include <IceUtil/IceUtil.h>

#include <rapplication/rapplication.h>
#include "config.h"

#ifdef SERVER
#include "visionI.h"
#endif

#include <Camera.h>

// User includes here

// Namespaces
using namespace std;
using namespace RoboComp;
using namespace RoboCompCamera;

class VisionComp : public RoboComp::Application
{
	private:
		TConfigParams configParams;
		void initialize();
	
	public:
		virtual int run(int, char*[]);
};

void VisionComp::initialize()
{
	configGetInt( "Vision.PyramidLevels", configParams.pyramidLevels , VISION_PYRLEVELS_DEFAULT_VALUE );
	configGetInt( "Vision.Radius", configParams.radius , VISION_RADIUS_DEFAULT_VALUE );
	
}
int VisionComp::run(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);// NON-GUI application
	int status=EXIT_SUCCESS;

	CameraPrx camera_proxy;
	string proxy;

	initialize();

	// Camera proxy creation
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

		camera_proxy = CameraPrx::uncheckedCast( communicator()->stringToProxy( proxy ) );
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

	// Now you can use server_proxy as local object
	Worker *worker= new Worker( camera_proxy , configParams );
	try
	{
		// Server adapter creation and publication
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("VisionComp");
		VisionI *visionI = new VisionI( worker );
		adapter->add(visionI, communicator()->stringToIdentity("vision"));
		adapter->activate();

		cout << SERVER_FULL_NAME " started" << endl;
		a.exec();
		status = EXIT_SUCCESS;
	}
	catch(const Ice::Exception& ex)
	{
		status = EXIT_FAILURE;
		cout << "[" << PROGRAM_NAME << "]: Exception raised on main thread: " << endl;
		cout << ex;
	}

	return status;
}

int main(int argc, char* argv[])
{
	bool hasConfig = false;
	string arg;
	VisionComp app;

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
